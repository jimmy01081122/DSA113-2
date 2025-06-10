#include <stdio.h>
#include <stdlib.h>

#define MAXN 1000010  // 最多支援 10^6 筆資料

long long heap[MAXN];  // heap[1..heapsize]
int heapsize = 0;

// 回傳 i 的父節點位置
int Parent(int i) {
    return i / 2;
}

// 將 key 插入到位置 i，並上浮到正確位置
void Heap_Increase_Key(int i, long long key) {
    heap[i] = key;
    while (i > 1 && heap[Parent(i)] > heap[i]) {
        // swap heap[i] 和 heap[Parent(i)]
        long long temp = heap[i];
        heap[i] = heap[Parent(i)];
        heap[Parent(i)] = temp;
        i = Parent(i);
    }
}

// 插入新元素 key 到 heap
void Min_Heap_Insert(long long key) {
    heapsize++;
    heap[heapsize] = 1e18;  // 先設成 "無限大"
    Heap_Increase_Key(heapsize, key);  // 再設成 key 並往上浮動
}

int main() {
    int N;
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        long long val;
        scanf("%lld", &val);
        Min_Heap_Insert(val);  // 每一筆資料執行一次插入
    }

    // 輸出 min-heap 結果（heap[1] ~ heap[heapsize]）
    for (int i = 1; i <= heapsize; i++) {
        if (i > 1) printf(" ");
        printf("%lld", heap[i]);
    }
    printf("\n");

    return 0;
}
