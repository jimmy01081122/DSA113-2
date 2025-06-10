#include <stdio.h>
#include <math.h>
// input : array size N, target value key, N integers
// output : index of key in the array
// 二分搜尋函數
// l: 左邊界索引, r: 右邊界索引
void Bin_Search(int a[], int key, int l, int r) {
    while (l <= r) {
        printf("Searching %d in range [%d, %d].\n", key, l, r); // 輸出當前搜尋範圍
        //floor() 是 C 語言的數學函數，用來返回小於等於輸入值的最大整數（即向下取整）
        //它屬於 math.h 標頭檔案中的函數，因此在使用 floor() 之前，需要包含 #include <math.h>。
        int m = (int)floor((l + r) / 2); // 計算中間索引 (需使用 floor)
        //printf("Pivot index = %d\n", m);
        //array 由0開始計算所以要先-1才能取得正確的值
        //printf("Pivot value = %d\n", a[m-1]);
        //printf("Key = %d\n", key);
        //find the pivot of middle index
        if (a[m - 1] == key) { 
        // 題目陣列是從 1 開始計數，所以索引要調整
            printf("Found at index %d.\n", m);
            return;
        } else if (a[m - 1] > key) {
            //printf("Searching left half.\n");
            r = m - 1; // 往左邊搜尋
        } else {
            //printf("Searching right half.\n");
            l = m + 1; // 往右邊搜尋
        }
    }

    // 當 `l > r` 時，代表找不到
    printf("Searching %d in range [%d, %d].\n", key, l, r);
    printf("Not found.\n");
}

int main() {
    int N, key;
    scanf("%d %d", &N, &key); // 讀取 N（陣列大小）與 key（目標值）

    int a[N]; // 建立陣列
    for (int i = 0; i < N; i++) {
        scanf("%d", &a[i]); // 讀取陣列元素
    }

    Bin_Search(a, key, 1, N); // 呼叫二分搜尋函數，注意索引從 1 開始
    return 0;
}
