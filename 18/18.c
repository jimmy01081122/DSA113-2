#include <stdio.h>
/*
這個排序的過程就是：
1. **從左到右遍歷陣列，每次考慮 `A[i]` 作為「新插入的數字」**。
2. **向左移動，讓 `A[i]` 插入到前面「已排序區間」的適當位置**（但最多只能進行 `B` 次移動）。
3. **確保左側的數據保持遞減排序（從大到小）**。
*/
// 全域變數：記錄已使用的 back-move 次數
int back_moves = 0;
int N,B; // 限制最大 back-move 次數

// 插入較大的數字（Insert-Bigger）
void Insert_Bigger(int A[], int m) {
    int data = A[m]; // 取出要插入的數字
    int i = m - 1;   // 從前一個元素開始比較
    //back move limit
    // 進行「回推移動」，但受到 B 限制
    while (i >= 0 && A[i] < data && back_moves < B) {
        A[i + 1] = A[i];  // 將數字向後推移
        i--;
        back_moves++;  // 記錄 back-move 次數
        //printf("back_moves = %d\n", back_moves);
    }

    A[i + 1] = data; // 插入正確位置
}

// 反向插入排序（Insertion-Sort）
void Insertion_Sort(int A[]) {
    // 設定 back-move 限制
    back_moves = 0;  // 初始化 back-move 計數

    for (int i = 1; i < N; i++) {
        Insert_Bigger(A, i); // 執行 Insert-Bigger
        /*
        printf("A[%d] = %d\n", i+2, A[i+2]);
        printf("A[%d] = %d\n", i+1, A[i+1]);
        printf("A[%d] = %d\n", i, A[i]);
        printf("A[%d] = %d\n", i-1, A[i-1]);
        if (i = 2)  {
            printf("A[%d] = %d\n", i-1, A[i-1]);
            printf("A[%d] = %d\n", i-2, A[i-2]);
        }
        if (i>=3){
            printf("A[%d] = %d\n", i-1, A[i-1]);
            printf("A[%d] = %d\n", i-2, A[i-2]);
            printf("A[%d] = %d\n", i-3, A[i-3]);
        }
            */
        if (back_moves >= B) break; // 若達到 back-move 限制，則提前停止
    }
}

int main() {
    scanf("%d %d", &N, &B);  // 讀取 N（陣列長度）與 B（back-move 限制）

    int A[N];
    for (int i = 0; i < N; i++) {
        scanf("%d", &A[i]);  // 讀取陣列元素
    }

    Insertion_Sort(A);  // 執行反向插入排序

    // 輸出結果
    printf("The array is");
    for (int i = 0; i < N; i++) {
        printf(" %d", A[i]);
    }
    printf(" after %d back-moves.\n", back_moves);

    return 0;
}
