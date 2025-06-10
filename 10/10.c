#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

// A segment represents a contiguous block of diamonds.
// For j = 0..(cnt-1), the j-th diamond has value = a + d * j.
// When d == 0, all diamonds equal a.
// When d == 1 (as after opType3), the diamonds are all distinct (each with count 1).
typedef struct {
    uint64_t a;    // starting value of the segment
    uint64_t cnt;  // number of diamonds in the segment
    uint64_t d;    // common difference
} Segment;

static Segment *segs = NULL;
static size_t segCount = 0;
static size_t segCap = 0;

// Ensure capacity for segments array.
static void ensureSegCap(size_t need) {
    if(need <= segCap) return;
    size_t newCap = (segCap == 0) ? 4 : segCap * 2;
    while(newCap < need) newCap *= 2;
    segs = realloc(segs, newCap * sizeof(Segment));
    segCap = newCap;
}

// Merge adjacent segments if they are mergeable:
//  - Two constant segments (d==0) with the same value.
 //  - Two arithmetic segments with d==1 if they are contiguous: i.e. if seg[i].a + seg[i].cnt == seg[i+1].a.
static void mergeSegments() {
    if(segCount == 0) return;
    size_t i = 0;
    for (size_t j = 1; j < segCount; j++) {
        Segment *prev = &segs[i];
        Segment *curr = &segs[j];
        if(prev->d == 0 && curr->d == 0 && prev->a == curr->a) {
            prev->cnt += curr->cnt;
        } else if(prev->d == 1 && curr->d == 1 && (prev->a + prev->cnt) == curr->a) {
            prev->cnt += curr->cnt;
        } else {
            i++;
            if(i != j) segs[i] = segs[j];
        }
    }
    segCount = i + 1;
}

// opType1: Remove all diamonds with value < v, output number removed,
// then add N diamonds with value v.
static void opType1(uint64_t N, uint64_t v) {
    uint64_t removedCount = 0;
    size_t pos = 0;
    // Remove whole segments whose maximum value is < v.
    while(pos < segCount) {
        uint64_t segMax = (segs[pos].d == 0) ? segs[pos].a : (segs[pos].a + segs[pos].d * (segs[pos].cnt - 1));
        if(segMax < v) {
            removedCount += segs[pos].cnt;
            pos++;
        } else {
            break;
        }
    }
    if(pos > 0) {
        // Remove segments [0, pos)
        for (size_t i = pos; i < segCount; i++) {
            segs[i - pos] = segs[i];
        }
        segCount -= pos;
    }
    // For the first remaining segment, if its first diamond is < v, remove the needed prefix.
    if(segCount > 0) {
        Segment *seg = &segs[0];
        if(seg->a < v) {
            // For constant segments, all are < v so remove entire segment.
            if(seg->d == 0) {
                removedCount += seg->cnt;
                // Remove this segment.
                for (size_t i = 1; i < segCount; i++) {
                    segs[i-1] = segs[i];
                }
                segCount--;
            } else if (seg->d == 1) { // arithmetic with d==1: values are consecutive.
                // Find how many diamonds to remove: they are from seg->a up to v-1.
                uint64_t toRemove = v - seg->a;
                if (toRemove > seg->cnt) toRemove = seg->cnt;
                removedCount += toRemove;
                seg->a += toRemove; 
                seg->cnt -= toRemove;
                if(seg->cnt == 0) {
                    for (size_t i = 1; i < segCount; i++) {
                        segs[i-1] = segs[i];
                    }
                    segCount--;
                }
            }
        }
    }
    // Output the removed count.
    printf("%" PRIu64 "\n", removedCount);
    
    // Insert N diamonds with value v (a constant segment, d==0).
    // Find insertion position to keep segments sorted by diamond value.
    size_t insertPos = 0;
    while(insertPos < segCount && segs[insertPos].a <= v) {
        insertPos++;
    }
    
    // Before inserting, check if the diamond value v already appears.
    // It can appear in a constant segment (d==0) or as the first value of an arithmetic (d==1) segment.
    // In the arithmetic case, each diamond is unique.
    if(insertPos > 0) {
        Segment *prev = &segs[insertPos-1];
        if(prev->d == 0 && prev->a == v) {
            // Simply add to that constant segment.
            prev->cnt += N;
            mergeSegments();
            return;
        } else if(prev->d == 1 && prev->a == v) {
            // The arithmetic segment at prev covers v as its first diamond.
            // Remove the first diamond from that segment (which contributes 1 occurrence of v)
            // and combine it with the new insertion.
            if(prev->cnt > 0) {
                // Remove one diamond from the front.
                prev->a += 1;
                prev->cnt -= 1;
                N += 1; // new block now has one extra diamond.
                if(prev->cnt == 0) {
                    // Remove the segment entirely.
                    for(size_t i = insertPos; i < segCount; i++) {
                        segs[i-1] = segs[i];
                    }
                    segCount--;
                    insertPos--; // adjust insertion position
                }
            }
            // Now fall through to insert the new constant segment.
        }
    }
    // Also check the segment at insertion position: if it is constant with v, merge.
    if(insertPos < segCount) {
        Segment *curr = &segs[insertPos];
        if(curr->d == 0 && curr->a == v) {
            curr->cnt += N;
            mergeSegments();
            return;
        }
    }
    // Insert a new constant segment {v, N, d==0}.
    ensureSegCap(segCount + 1);
    for (size_t i = segCount; i > insertPos; i--) {
        segs[i] = segs[i-1];
    }
    segs[insertPos].a = v;
    segs[insertPos].cnt = N;
    segs[insertPos].d = 0;
    segCount++;
    mergeSegments();
}

// opType2: Query the multiplicity of diamonds with value p.
static void opType2(uint64_t p) {
    uint64_t ans = 0;
    for (size_t i = 0; i < segCount; i++) {
        uint64_t start = segs[i].a;
        uint64_t end = (segs[i].d == 0) ? segs[i].a : (segs[i].a + segs[i].d * (segs[i].cnt - 1));
        if(p < start || p > end) continue;
        if(segs[i].d == 0) {
            ans += segs[i].cnt; // all diamonds equal start.
        } else { // d==1 (or any other d, but after opType3 we expect d==1)
            // In an arithmetic segment with d==1, each diamond appears once.
            ans += 1;
        }
    }
    printf("%" PRIu64 "\n", ans);
}

// opType3: Transform all diamonds using the rank transformation.
// Let total be the total number of diamonds. In descending order, the diamond at rank j (1-indexed)
// gets increased by (M - j + 1). Instead of expanding, we update each segment (which is currently in ascending order)
// using prefix sums. (This opType3 is only allowed when |S| <= M, and it is guaranteed that after transformation,
// each diamond becomes distinct (i.e. the arithmetic segments will have d==1).)
static void opType3(uint64_t M) {
    if(segCount == 0) return;
    // Compute total number of diamonds.
    uint64_t total = 0;
    for (size_t i = 0; i < segCount; i++) {
        total += segs[i].cnt;
    }
    // We must recompute the new values for each segment.
    // Our original multiset (stored in segments) is in ascending order.
    // In descending order the groups would be processed in reverse.
    // For group i (in ascending order), let prefix = sum_{j=0}^{i-1} segs[j].cnt.
    // Then in descending order, group i's diamonds have ranks from: R_start = total - prefix - segs[i].cnt + 1
    // to R_end = total - prefix.
    // In ascending order, the new values for group i will be:
    //   new_a = old_a + (M - R_end + 1)
    // and since the diamonds become consecutive (difference 1), we set d = 1.
    uint64_t prefix = 0;
    for (size_t i = 0; i < segCount; i++) {
        uint64_t c = segs[i].cnt;
        uint64_t R_end = total - prefix;
        segs[i].a = segs[i].a + (M - R_end + 1);
        segs[i].d = 1; // now each diamond is distinct.
        prefix += c;
    }
    // After transformation, adjacent segments might now be contiguous.
    mergeSegments();
}

int main(){
    int T;
    uint64_t M;
    scanf("%d %" SCNu64, &T, &M);
    // Initially, the diamond set is empty.
    segCount = 0;
    ensureSegCap(1000);
    
    while(T--){
        int op;
        scanf("%d", &op);
        if(op == 1){
            uint64_t Ni, vi;
            scanf("%" SCNu64 " %" SCNu64, &Ni, &vi);
            opType1(Ni, vi);
        }
        else if(op == 2){
            uint64_t pi;
            scanf("%" SCNu64, &pi);
            opType2(pi);
        }
        else if(op == 3){
            opType3(M);
        }
    }
    
    free(segs);
    return 0;
}
