#include <stdio.h>
#include <stdlib.h>

#define SIZE 9

// 輸入的鄰雷數表 N[r][c]：表示該格與其鄰近最多 8 格(含自身)的雷數和
static int N[SIZE][SIZE];

// 我們要尋找的解 M[r][c]：若該格有雷則為 1，否則為 0
static int M[SIZE][SIZE];

// 用於標記是否已找到解
static int foundSolution = 0;

// 計算 (r, c) 位置的周圍(含自身)雷數，並與 N[r][c] 比較
// boundary 檢查要注意避免 r-1, c-1 < 0 或 r+1, c+1 >= SIZE
int neighborSum(int r, int c) {
    int sum = 0;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE) {
                sum += M[nr][nc];
            }
        }
    }
    return sum;
}

// 檢查第 r 列是否符合 N 的條件
// 當我們要「定案」第 r 列時，需要第 (r-1) 列, r 列, (r+1) 列 都已確定或不存在
// 這裡的使用時機：每次填完第 r 列後，就檢查前一列 (r-1) 是否正確。
// 最後填到 r=9(表示 0..8 都填完)時，再檢查第 8 列。
int checkRow(int r) {
    for (int c = 0; c < SIZE; c++) {
        if (neighborSum(r, c) != N[r][c]) {
            return 0;  // 不符合
        }
    }
    return 1;  // 全部符合
}

// 印出最終的 9×9 解
void printSolution(void) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            printf("%d", M[r][c]);
            if (c < SIZE - 1) {
                printf(" ");
                
            }
        }
        printf("\n");
    }
}

// 以遞迴方式逐格嘗試放置 0 或 1
// r, c 代表當前要決定的格子(第 r 列, 第 c 行)
void solve(int r, int c) {
    // 若已找到解，直接返回 (避免多解或繼續浪費時間)
    if (foundSolution) return;

    // 如果 r == 9，代表已經填完最後一列(row 8)，檢查 row 8 是否符合
    if (r == SIZE) {
        // 檢查第 8 列是否滿足條件
        if (checkRow(SIZE - 1)) {
            foundSolution = 1;
            printSolution();
        }
        return;
    }

    // 如果 c == 9，代表第 r 列填完了，先檢查第 (r - 1) 列(若 r>0)
    // 然後繼續下一列 r+1
    if (c == SIZE) {
        // 檢查前一列
        if (r > 0) {
            if (!checkRow(r - 1)) {
                return;  // 不符合就回溯
            }
        }
        // 符合則繼續下一列
        solve(r + 1, 0);
        return;
    }

    // 嘗試放 0
    M[r][c] = 0;
    solve(r, c + 1);
    if (foundSolution) return;

    // 嘗試放 1
    M[r][c] = 1;
    solve(r, c + 1);
    if (foundSolution) return;

    // 如果兩種都沒成功，會回溯到上一層
}

int main(void) {
    // 讀入 9×9 的鄰雷數
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            scanf("%d", &N[r][c]);
        }
    }

    // 開始遞迴解
    solve(0, 0);

    // 若沒找到解則輸出 no solution
    if (!foundSolution) {
        printf("no solution\n");
    }

    return 0;
}
