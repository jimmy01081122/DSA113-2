#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

// Segment structure: represents a contiguous block of diamonds.
// A segment stores an arithmetic progression of diamond values.
// For j = 0 ... cnt-1, the j-th diamond in the segment has value: a + d*j.
// For a constant segment, d==0.
typedef struct {
    uint64_t a;   // starting value
    uint64_t cnt; // number of diamonds in the segment
    uint64_t d;   // difference; if d==0, all diamonds equal a; if d>0, they are distinct.
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

// Merge adjacent segments if they are “mergeable”.
// Two segments are mergeable if they have the same d and the second segment’s first value equals
// the first segment’s last value plus d.
static void mergeSegments() {
    if(segCount == 0) return;
    size_t i = 0;
    for(size_t j = 1; j < segCount; j++){
        Segment *prev = &segs[i];
        Segment *curr = &segs[j];
        // For constant segments, merge if same value.
        if(prev->d == 0 && curr->d == 0 && prev->a == curr->a) {
            prev->cnt += curr->cnt;
        }
        // For arithmetic segments (d>0) merge if curr->a equals prev->a + prev->d * prev->cnt.
        else if(prev->d > 0 && curr->d > 0 && curr->a == prev->a + prev->d * prev->cnt) {
            prev->cnt += curr->cnt;
        }
        else {
            i++;
            if(i != j) segs[i] = segs[j];
        }
    }
    segCount = i + 1;
}

// Binary search for the segment whose range might contain value v.
// Each segment covers [a, a + d*(cnt-1)]. (For constant segments d==0.)
static int binarySearch(uint64_t v) {
    int low = 0, high = (int)segCount - 1;
    while(low <= high) {
        int mid = (low + high) / 2;
        uint64_t segLow = segs[mid].a;
        uint64_t segHigh;
        if(segs[mid].d == 0) {
            segHigh = segs[mid].a;
        } else {
            segHigh = segs[mid].a + segs[mid].d * (segs[mid].cnt - 1);
        }
        if(v < segLow)
            high = mid - 1;
        else if(v > segHigh)
            low = mid + 1;
        else
            return mid;
    }
    return -1; // not found
}

// Insert a new segment at position pos.
static void insertSegment(size_t pos, Segment seg) {
    ensureSegCap(segCount + 1);
    // shift segments to the right
    for (size_t i = segCount; i > pos; i--) {
        segs[i] = segs[i - 1];
    }
    segs[pos] = seg;
    segCount++;
}

// Remove segments in the range [l, r) (l inclusive, r exclusive).
static void removeSegments(size_t l, size_t r) {
    if(r <= l) return;
    for(size_t i = r; i < segCount; i++){
        segs[i - (r - l)] = segs[i];
    }
    segCount -= (r - l);
}

// opType1: remove all diamonds with value < v, output total removed, then add N copies of diamond v.
static void opType1(uint64_t N, uint64_t v) {
    uint64_t removedCount = 0;
    size_t pos = 0;
    // Remove segments completely if their maximum value is < v.
    while(pos < segCount) {
        uint64_t segMax;
        if(segs[pos].d == 0) {
            segMax = segs[pos].a;
        } else {
            segMax = segs[pos].a + segs[pos].d * (segs[pos].cnt - 1);
        }
        if(segMax < v) {
            removedCount += segs[pos].cnt;
            pos++;
        } else {
            break;
        }
    }
    // Remove segments [0, pos)
    if(pos > 0) {
        removeSegments(0, pos);
    }
    // For the new first segment (if any) that might partly have values < v:
    if(segCount > 0) {
        Segment *seg = &segs[0];
        // If even the first element is >= v, do nothing.
        if(seg->a < v) {
            // For constant segment (d==0): all values are seg->a (< v) so remove entire segment.
            if(seg->d == 0) {
                removedCount += seg->cnt;
                removeSegments(0,1);
            } else {
                // For arithmetic segment: find smallest index j (0-indexed) such that a + d*j >= v.
                // Solve: j >= (v - a + d - 1) / d.
                uint64_t j = (v - seg->a + seg->d - 1) / seg->d;
                if(j > seg->cnt) j = seg->cnt; // safety
                removedCount += j;
                // Update the segment: remove first j elements.
                seg->a += seg->d * j;
                seg->cnt -= j;
                if(seg->cnt == 0) {
                    removeSegments(0,1);
                }
            }
        }
    }
    // Output the number removed.
    printf("%" PRIu64 "\n", removedCount);
    // Insert new diamonds: a constant segment with value v.
    // Find proper insertion position (keep sorted order by a).
    size_t insertPos = 0;
    while(insertPos < segCount && segs[insertPos].a <= v) {
        insertPos++;
    }
    Segment newSeg = { .a = v, .cnt = N, .d = 0 };
    insertSegment(insertPos, newSeg);
    // Try to merge adjacent segments.
    mergeSegments();
}

// opType2: query the multiplicity of p.
static void opType2(uint64_t p) {
    int idx = binarySearch(p);
    if(idx == -1) {
        printf("0\n");
        return;
    }
    Segment seg = segs[idx];
    if(seg.d == 0) {
        // All diamonds in this segment equal seg->a.
        printf("%" PRIu64 "\n", seg.cnt);
    } else {
        // In an arithmetic segment (d>0) every diamond is unique.
        // Check if p is exactly one of them.
        uint64_t offset = p - seg.a;
        if(offset % seg.d == 0 && offset / seg.d < seg.cnt)
            printf("1\n");
        else
            printf("0\n");
    }
}

// opType3: transform all diamonds by their rank.
// Let total = total diamonds, and for each diamond at overall ascending rank j (1-indexed),
// update its value to: new_value = (old_value) + (M - total + j).
// For a segment covering positions [L, L+cnt-1] with (a, d), new segment becomes:
//    new_a = a + (M - total + L)
//    new_d = d + 1
static void opType3(uint64_t M) {
    if(segCount == 0) return;
    uint64_t total = 0;
    for(size_t i = 0; i < segCount; i++){
        total += segs[i].cnt;
    }
    uint64_t prefix = 0;
    for(size_t i = 0; i < segCount; i++){
        uint64_t L = prefix + 1; // overall ascending rank for first diamond in seg i.
        // Update segment i:
        segs[i].a = segs[i].a + (M - total + L);
        segs[i].d = segs[i].d + 1; // each diamond's gap increases by 1.
        prefix += segs[i].cnt;
    }
    // After transformation the segments are still in sorted order.
    // It is possible that adjacent segments now become contiguous and mergeable.
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
