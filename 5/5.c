#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 20
#define INF 100

int n, m;
int edges[MAX_N][MAX_N]; // 鄰接矩陣
int bestCover[MAX_N];    // 存放最小點覆蓋
int bestSize = INF;      // 最小集合大小

// 檢查 subset 是否為有效的覆蓋集合
void checkSubset(int subset) {
    int selected[MAX_N] = {0}; 
    int temp[MAX_N], tempSize = 0;

    // 解析 subset，確定哪些點被選
    for (int i = 0; i < n; i++) {
        if (subset & (1 << i)) {
            selected[i] = 1;
            temp[tempSize++] = i;
        }
    }

    // 檢查是否覆蓋所有邊
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (edges[u][v] && !selected[u] && !selected[v]) {
                return;
            }
        }
    }

    // 更新最佳解
    if (tempSize < bestSize) {
        bestSize = tempSize;
        memcpy(bestCover, temp, sizeof(int) * tempSize);
    } else if (tempSize == bestSize) {
        // 只比較一次字典序
        int update = 0;
        for (int i = 0; i < bestSize; i++) {
            if (temp[i] < bestCover[i]) {
                update = 1;
                break;
            } else if (temp[i] > bestCover[i]) {
                break;
            }
        }
        if (update) {
            memcpy(bestCover, temp, sizeof(int) * tempSize);
        }
    }
}

int main() {
    scanf("%d %d", &n, &m);
    
     // 特殊情況：當沒有節點時，不需要輸出任何東西
     if (n == 0) {
        return 0;
    }
    memset(edges, 0, sizeof(edges));
    // 讀取所有邊
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        edges[u][v] = edges[v][u] = 1;
    }


    // 當沒有邊時，空集合就是最小點覆蓋
    if (m == 0) {
        bestSize = 0;
    } else {
        // 遍歷所有可能的點選擇
        int totalSubsets = 1 << n; // 2^n
        for (int subset = 1; subset < totalSubsets; subset++) {
            checkSubset(subset);
        }
    }

    // 輸出結果
    if (bestSize > 0 && bestSize < INF) {

        for (int i = 0; i < bestSize; i++) {
            printf("%d\n", bestCover[i]);
        }
    }
    return 0;
}
