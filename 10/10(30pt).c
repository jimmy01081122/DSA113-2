#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

// 若環境有 __int128 可用，也可能需要，但此處用 64-bit (long long 或 uint64_t) 即可

// 資料結構：value -> count
// 這裡用一個簡單的動態陣列 + 排序 + 二分搜尋 (當成 key-value pairs) 來示範。
// 實際上可改用平衡樹 (例如紅黑樹) 或 hash map + sorting trick。
//
// Node: (value, count)
typedef struct {
    uint64_t value;
    uint64_t count;
} Node;

// 全域動態陣列
static Node *arr = NULL;
static size_t arrSize = 0;  // 目前有多少個不同 value
static size_t arrCap = 0;   // 容量

// 擴充 capacity
static void ensureCap(size_t need) {
    if (need <= arrCap) return;
    size_t newCap = (arrCap == 0) ? 4 : arrCap * 2;
    while (newCap < need) newCap *= 2;
    arr = (Node*)realloc(arr, newCap * sizeof(Node));
    arrCap = newCap;
}

// 以 value 為依據的比較函式 (ascending)
static int cmpValue(const void* a, const void* b) {
    const Node* na = (const Node*)a;
    const Node* nb = (const Node*)b;
    if (na->value < nb->value) return -1;
    if (na->value > nb->value) return 1;
    return 0;
}

// 在 arr[] (已依 value 遞增排序) 裡找 key = v (二分搜尋)
// 回傳找到的 index，找不到回傳 ~pos，表示應插在 pos
static int64_t binSearch(uint64_t v) {
    int64_t left = 0, right = (int64_t)arrSize - 1;
    while (left <= right) {
        int64_t mid = (left + right) >> 1;
        if (arr[mid].value == v) {
            return mid;
        } else if (arr[mid].value < v) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ~left; // ~left 表示沒找到，應插在 left
}

// 在 arr 裡插入或更新 key = v，count += c
static void addValueCount(uint64_t v, uint64_t c) {
    int64_t pos = binSearch(v);
    if (pos >= 0) {
        // found
        arr[pos].count += c;
    } else {
        // not found
        pos = ~pos; // 插入位置
        ensureCap(arrSize + 1);
        // shift
        for (int64_t i = (int64_t)arrSize; i > pos; i--) {
            arr[i] = arr[i-1];
        }
        arr[pos].value = v;
        arr[pos].count = c;
        arrSize++;
    }
}

// 移除 key = v 若其 count = 0 則從陣列刪除
// (此題中不一定要實作此函式，但移除特定 key 可能有用)
static void removeKeyIndex(size_t idx) {
    // arr[idx].count == 0 表示該 key 要刪除
    // shift
    for (size_t i = idx; i+1 < arrSize; i++) {
        arr[i] = arr[i+1];
    }
    arrSize--;
}

// ------------------------------
// 1) 移除所有 < v 的元素 + 輸出移除個數
//    然後插入 N 個 v
// ------------------------------
static void opType1(uint64_t N, uint64_t v) {
    // 移除所有 < v
    // 找出最左邊 >= v 的位置
    // => 先用 binSearch(v)，拿到插入點 pos
    // => pos 即為第一個 >= v 的 index
    int64_t pos = binSearch(v);
    if (pos >= 0) {
        // 有相同鍵 => pos 指向它
        // 但更重要的是要找 pos 之前的 < v
        // => pos 之前所有都是 < v
    } else {
        // ~pos 為插入位置
        pos = ~pos;
    }

    // pos 是第一個 >= v
    // => [0 .. pos-1] 全部都是 < v
    uint64_t removedCount = 0;
    for (int64_t i = 0; i < pos; i++) {
        removedCount += arr[i].count;
    }

    // 移除 [0..pos-1]
    if (pos > 0) {
        // shift [pos..arrSize-1] to front
        for (int64_t i = pos; i < (int64_t)arrSize; i++) {
            arr[i - pos] = arr[i];
        }
        arrSize -= pos;
    }

    // 輸出移除個數
    printf("%" PRIu64 "\n", removedCount);

    // 然後插入 N 個 v
    addValueCount(v, N);
}

// ------------------------------
// 2) 查詢 pi => 輸出 multiplicity
// ------------------------------
static void opType2(uint64_t p) {
    int64_t pos = binSearch(p);
    if (pos >= 0) {
        printf("%" PRIu64 "\n", arr[pos].count);
    } else {
        printf("0\n");
    }
}

// ------------------------------
// 3) 將所有元素照 value 遞減排序
//    排名 j=1..|S| => s_j += (M - j + 1)
// ------------------------------
static void opType3(uint64_t M) {
    if (arrSize == 0) return;

    // 先將 arr[] 照 value 遞增 => 改為遞減較方便處理
    // 我們可以直接用 qsort + reverse，或自己寫比較函式
    // 這裡直接 qsort ascending，之後反向處理
    // (arr 可能本來就已是 ascending，但若有做 type_1 insertion 之後依舊保持 ascending)
    // => 保險起見還是重新 sort 一次
    qsort(arr, arrSize, sizeof(Node), cmpValue);

    // 現在 arr[0] 最小, arr[arrSize-1] 最大
    // 我們要從大到小 => 從 arrSize-1 向下走
    // j=1 => arrSize-1
    // j=2 => arrSize-2
    // ...
    // j=arrSize => 0
    //
    // 但每個 value 可能有 count > 1 => 這些相同 value 要分配不同 rank
    // ex: if arr[idx].count = c => 代表 c 個相同的 value
    // 這 c 個鑽石排名連續
    // rank j ~ j+c-1
    // 之後再繼續下一個 idx
    // for each diamond ranked j => diamondValue += (M - j + 1)
    //
    // => 做法: 從後面開始, 令 j=1, 逐一處理 count
    //          arr[idx].value 會被加上不同的 (M - rank + 1)
    //          但這 c 個相同 value, rank 不同 => 最終會變成 c 個不同 value
    // => 需要把它們分開?
    // => 最終我們需要在 arr[] 以 "value->count" 的方式表示新的分佈
    //
    // => 也就是, arr[idx] 可能被拆成多個(key, count=1)
    // => 這在 worst case 會讓 arrSize 膨脹, 可能到 sum of all counts (<= M)
    // => M 可能高達 10^12, 這顯然不可能顯式展開!!!
    // => 這裡是本題最難之處: 要怎麼在 type_3 後仍然用 "value->count" 來表示,
    //    但排名會導致相同 value 被拆成不同新 value
    //
    // 直覺做法: for (j=1 to totalCount) each diamond => newVal = oldVal + (M-j+1)
    // => 同樣 value 可能分散到很多 newVal
    // => newVal 會非常多
    // => 直接展開 => O(M) => 不可行
    //
    // => 需更進階的結構 (ex segment tree, or offline queries, or coordinate compression)
    // => 但此題保證 "|S| ≤ M" 但 M up to 10^12 => 可能 |S| 也很大
    // => 要在 1 sec / 2 sec 時間內做, 幾乎不可能真的展開
    //
    // 這裡給一個「理論正確但可能跑不動」的示範:
    // 1) 先把整個 multiset 展開到 array diamondVal[] (size = sum of counts)
    // 2) sort descending
    // 3) for j=0..(total-1): diamondVal[j] += (M - (j+1) + 1) = (M - j)
    // 4) 再把 diamondVal[] 重新壓回 (value->count) 形式
    //
    // 只適合 subtask (小數據) 用, 大數據必定超時/超記憶體
    //
    // 下面僅示範, 以供參考

    // 計算 total 個數
    uint64_t total = 0;
    for (size_t i = 0; i < arrSize; i++) {
        total += arr[i].count;
    }
    if (total == 0) return;

    // 展開
    // 注意 total 可能高達 10^12 => malloc 無法容納 => 此方法在大輸入下不可行
    // 這裡僅做示範 => 若要在大數據下通過, 需要更複雜做法
    uint64_t* diamonds = (uint64_t*)malloc(sizeof(uint64_t) * (size_t)total);
    if (!diamonds) {
        // 無法配置 => 直接跳過 or exit
        // 只做示範 => 真實狀況需其他方法
        return;
    }

    size_t idx = 0;
    for (int64_t i = (int64_t)arrSize - 1; i >= 0; i--) {
        // arr[i] 是一個 value(升序最後=最大) => 但我們要從大到小
        for (uint64_t c = 0; c < arr[i].count; c++) {
            diamonds[idx++] = arr[i].value;
        }
    }
    // 現在 diamonds[0..idx-1] 大到小
    // 其實上面迴圈是 "value" 遞減, 但同一 value 直接依 count append => 也是大到小

    // 逐一加上 (M - j)
    // j=0 => + (M - 0)
    // j=1 => + (M - 1)
    // ...
    // =>  diamonds[j] += (M - j)
    for (size_t j = 0; j < idx; j++) {
        // M - j 可能會超過 64-bit? => M <= 10^12, j <= 10^12 => 仍有風險
        // 需使用 128-bit or 做檢查 => 此處示範, assume 不會爆 (實際需防範)
        uint64_t addVal = (uint64_t)((long long)M - (long long)j);
        diamonds[j] += addVal;
    }

    // 重新壓回 (value->count)
    arrSize = 0; // 清空
    for (size_t j = 0; j < idx; j++) {
        // addValueCount => O(log n + shift) => 可能很慢
        // => 同樣只適用小資料
        addValueCount(diamonds[j], 1);
    }

    free(diamonds);
}

// ------------------------------
// 主程式
// ------------------------------
int main() {
    // 讀入 T, M
    // T 最多 10^6, M 最多 10^12
    int T;
    uint64_t M;
    scanf("%d %" SCNu64, &T, &M);

    // 預先給予一些 capacity
    ensureCap(1000);

    while (T--) {
        int op;
        scanf("%d", &op);
        if (op == 1) {
            // 1 Ni vi
            uint64_t Ni, vi;
            scanf("%" SCNu64 " %" SCNu64, &Ni, &vi);
            opType1(Ni, vi);
        } else if (op == 2) {
            // 2 pi
            uint64_t pi;
            scanf("%" SCNu64, &pi);
            opType2(pi);
        } else {
            // 3
            opType3(M);
        }
    }

    // 收尾
    free(arr);
    return 0;
}
