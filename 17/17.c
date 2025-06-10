#include <stdio.h>
#include <stdlib.h>
/*
1. get_min_index()

2.  在 arr[l] 到 arr[N-1] 之間找最小值。
    若 compare_count >= B，則立即輸出並終止程式。
    每次比較 (if (arr[i] < arr[m])) 都會增加 compare_count。
    selection_sort()
    (repeat)~~~~

3.  每次選擇 min_idx，並交換 arr[i] 和 arr[min_idx]。
    當 B 未達上限時，最終輸出完整排序結果。 
*/
int B,N;  // 全域變數：陣列大小與比較次數上限
int compare_count = 0; // 記錄比較次數

// 取得最小值的索引
int get_min_index(int arr[], int l,int B) {
    int m = l;
    for (int i = l + 1; i < N; i++) {
        if (compare_count>= B) { 
            // 如果已經達到比較次數限制 B，則立即輸出並終止程式
            printf("The array is");
            for (int j = 0; j < N; j++) {
                printf(" %d", arr[j]);
            }
            printf(" after %d comparisons.\n", B);
            exit(0);
        }

       

        if (arr[i] < arr[m]) {
            m = i;
           // printf("arr[%d] = %d < arr[%d] = %d\n", i, arr[i], m, arr[m]);
        }
        compare_count++; 
    }
    return m;
}

// 選擇排序函式
void selection_sort(int arr[]) {
    if (N == 1) {
        printf("The array is %d after 0 comparisons.\n", arr[0]);
        return;
    }
    
    for (int i = 0; i < N - 1; i++) {
        int min_idx = get_min_index(arr, i ,B); // 取得當前最小值索引

        // 交換 arr[i] 和 arr[min_idx]
        if (i != min_idx) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }

    // 若未達 B，則輸出最終排序結果
    printf("The array is");
    for (int i = 0; i < N; i++) {
        printf(" %d", arr[i]);
    }
    printf(" after %d comparisons.\n", compare_count);
}

// 主函式
int main() {
    // 讀取 N 和 B
    scanf("%d %d", &N, &B);

    int arr[N]; // 宣告陣列

    // 讀取陣列數值
    for (int i = 0; i < N; i++) {
        scanf("%d", &arr[i]);
    }

    // 執行選擇排序
    selection_sort(arr);

    return 0;
}
