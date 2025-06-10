#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // 需要加入 stdlib.h 來使用 malloc

typedef struct {
    int row;
    int col;
} Position;

int main(void) {
    int n, m;
    scanf("%d %d", &n, &m);

    // 動態分配玩家名稱數組
    char (*names)[65] = malloc(n * sizeof(*names));
    if (!names) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // 動態分配三維數組 board
    int ***board = malloc(n * sizeof(int **));
    if (!board) {
        free(names);
        printf("Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        board[i] = malloc(m * sizeof(int *));
        if (!board[i]) {
            // 清理之前分配的內存
            for (int j = 0; j < i; j++) {
                free(board[j]);
            }
            free(board);
            free(names);
            printf("Memory allocation failed\n");
            return 1;
        }
        for (int j = 0; j < m; j++) {
            board[i][j] = malloc(m * sizeof(int));
            if (!board[i][j]) {
                // 清理之前分配的內存
                for (int k = 0; k < j; k++) {
                    free(board[i][k]);
                }
                for (int k = 0; k < i; k++) {
                    for (int l = 0; l < m; l++) {
                        free(board[k][l]);
                    }
                    free(board[k]);
                }
                free(board);
                free(names);
                printf("Memory allocation failed\n");
                return 1;
            }
        }
    }

    // 動態分配 Position 數組
    Position **pos = malloc(n * sizeof(Position *));
    if (!pos) {
        // 清理之前分配的內存
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                free(board[i][j]);
            }
            free(board[i]);
        }
        free(board);
        free(names);
        printf("Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        pos[i] = malloc((m * m + 1) * sizeof(Position));
        if (!pos[i]) {
            // 清理之前分配的內存
            for (int j = 0; j < i; j++) {
                free(pos[j]);
            }
            free(pos);
            // ... (清理其他內存)
            return 1;
        }
    }

    // 動態分配計數數組
    int **rowCount = malloc(n * sizeof(int *));
    int **colCount = malloc(n * sizeof(int *));
    if (!rowCount || !colCount) {
        // 清理內存
        return 1;
    }
    for (int i = 0; i < n; i++) {
        rowCount[i] = calloc(m, sizeof(int));  // 使用 calloc 初始化為 0
        colCount[i] = calloc(m, sizeof(int));
        if (!rowCount[i] || !colCount[i]) {
            // 清理內存
            return 1;
        }
    }

    // 動態分配對角線計數數組
    int *diagCount1 = calloc(n, sizeof(int));
    int *diagCount2 = calloc(n, sizeof(int));
    if (!diagCount1 || !diagCount2) {
        // 清理內存
        return 1;
    }

    // ... 原有的程式邏輯 ...

    // 在程序結束前釋放所有分配的內存
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            free(board[i][j]);
        }
        free(board[i]);
        free(pos[i]);
        free(rowCount[i]);
        free(colCount[i]);
    }
    free(board);
    free(pos);
    free(rowCount);
    free(colCount);
    free(diagCount1);
    free(diagCount2);
    free(names);

    return 0;
}