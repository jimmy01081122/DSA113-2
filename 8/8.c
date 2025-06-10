#include <stdio.h>
#include <string.h>

#define MAXN 10        // n 的最大值(玩家數)
#define MAXM 256       // m 的最大值(賓果盤大小)

// 為了快速標記某位玩家的某個數字(1..m*m)在賓果盤上的位置，
// 我們會用一個結構來存 row 與 col。
typedef struct {
    int row;
    int col;
} Position;

int main(void) {
    int n, m;
    scanf("%d %d", &n, &m);

    // 用來存玩家姓名
    char names[MAXN][65];

    // 用來存每個玩家的賓果盤資料 (player i 的第 r 行、第 c 列)
    // board[i][r][c]
    static int board[MAXN][MAXM][MAXM];

    // pos[i][x] 代表第 i 位玩家對應到「數字 x」時，該數字在此玩家盤上的 (row, col)
    // 由於題目說數字在 1..(m*m) 之間，所以陣列長度開到 m*m+1 (方便用 x 直接當索引)
    static Position pos[MAXN][MAXM*MAXM + 1];

    // 用來記錄各玩家對應行、列、兩條對角線已經「被叫到」的個數
    // rowCount[i][r] 表示第 i 位玩家的第 r 行，已標記多少格
    // colCount[i][c] 表示第 i 位玩家的第 c 列，已標記多少格
    // diagCount1[i]  表示左上到右下對角線已標記多少格
    // diagCount2[i]  表示右上到左下對角線已標記多少格
    static int rowCount[MAXN][MAXM], colCount[MAXN][MAXM];
    static int diagCount1[MAXN], diagCount2[MAXN];

    // 讀入每位玩家的資訊 (姓名 + m 行的 bingo 盤)
    for(int i = 0; i < n; i++){
        // 讀姓名
        scanf("%64s", names[i]);

        // 讀入 m 行，每行 m 個數字
        for(int r = 0; r < m; r++){
            for(int c = 0; c < m; c++){
                int x;
                scanf("%d", &x);
                board[i][r][c] = x;

                // 紀錄「此玩家的數字 x 在哪一行、哪一列」
                pos[i][x].row = r;
                //printf("pos[%d][%d].row = %d\n", i, x, r);
                pos[i][x].col = c;
                //printf("pos[%d][%d].col = %d\n", i, x, c);
            }
        }
    }

    // 讀入遊戲中會被依序呼叫的 m*m 個數字
    int calls = m * m;
    static int calledNumbers[MAXM * MAXM];
    for(int i = 0; i < calls; i++){
        scanf("%d", &calledNumbers[i]);
        //printf("calledNumbers[%d] = %d\n", i, calledNumbers[i]);
    }

    // 用來記錄是否已經有人 Bingo，以及在第幾次呼叫時出現最早的 Bingo
    int firstBingoCallIndex = -1;  // 還沒產生 Bingo 時，用 -1 表示
    // 用來標記各玩家是否在「某次呼叫」達成了 bingo
    // 若 bingoTime[i] == -1 表示尚未 bingo；否則表示該玩家在第幾次呼叫完成
    static int bingoTime[MAXN];
    for(int i = 0; i < n; i++){
        bingoTime[i] = -1;
        //printf("bingoTime[%d] = %d\n", i, bingoTime[i]);
    }

    // 依序處理被叫到的號碼
    for(int k = 0; k < calls; k++){
        int number = calledNumbers[k];  // 第 k 次呼叫的數字(0-based)
        //printf("number = %d\n", number);
        // 對所有玩家做「標記」並檢查是否造成 bingo
        for(int i = 0; i < n; i++){
            // 如果這個玩家已經在之前就達成 bingo，就不用再檢查
            if(bingoTime[i] != -1) {
                continue;
            }
/////////////////////////////////////////////////////////////////////////////
            // 找到此號碼在玩家 i 的位置 (r, c)
            // pos[i][x] 代表第 i 位玩家對應到「數字 x」時，該數字在此玩家盤上的 (row, col)
            int r = pos[i][number].row;
            //printf("r = %d\n", r);
            int c = pos[i][number].col;
            //printf("c = %d\n", c);
            // 上述給row col鎖定了該玩家的該號碼的位置
            //printf("board[%d][%d][%d] = %d\n", i, r, c, board[i][r][c]);
            // 把該行、該列、以及是否在對角線上的計數都 +1
            rowCount[i][r]++;
            //printf("rowCount[%d][%d] = %d\n", i, r, rowCount[i][r]);
            //如果 rowCount[i][r] 累積到 m，代表「這一行所有格子都被標記了」，也就達成整行 Bingo
            colCount[i][c]++;
            //printf("colCount[%d][%d] = %d\n", i, c, colCount[i][c]);
            //如果 colCount[i][r] 累積到 m，代表「這一列所有格子都被標記了」，也就達成整行 Bingo
            //左上到右下
            if(r == c){
                diagCount1[i]++;
            }
            //右上到左下
            if(r + c == m - 1){
                diagCount2[i]++;
            }
// k 在這裡是第幾次呼叫
            // 檢查這個動作是否使玩家 i 的某行、某列或某對角線全部被標記 (== m)
            if(rowCount[i][r] == m ||
               colCount[i][c] == m ||
               diagCount1[i] == m  ||
               diagCount2[i] == m){
                // 此玩家在呼叫次數 (k+1) 達成 bingo
                bingoTime[i] = k + 1; // (k+1) 表示「第幾個號碼被叫到」(1-based)
                //printf("bingoTime[%d] = %d\n", i, bingoTime[i]);
                //printf("第幾次 k= %d\n", k);
                // 如果目前沒有人達到過 bingo 或者這是第一個達到的
                // 就更新 firstBingoCallIndex
                if(firstBingoCallIndex == -1) {
                    firstBingoCallIndex = k + 1;
                }
            }
        }

        // 一旦有人在這一次呼叫之後達成 bingo，
        // 而且 firstBingoCallIndex == (k+1)，表示已經知道最早的 bingo 出現在第 k+1 次，
        // 就可以結束 (不需要再看後續的號碼)
        if(firstBingoCallIndex == (k + 1)) {
            //printf("firstBingoCallIndex = %d\n", firstBingoCallIndex);
            break;
        }
    }

    // 輸出結果:
    // 根據題意，若有人在 firstBingoCallIndex 時達成，
    // 就要依照玩家出現的順序，輸出「玩家編號(1-based) 與 達成的呼叫次數」。
    // 若題目只要最早那一次達成的玩家們，可只印 bingoTime == firstBingoCallIndex 的那些玩家。

    for(int i = 0; i < n; i++){
        if(bingoTime[i] == firstBingoCallIndex) {
            // i+1 表示玩家的編號(1-based)，firstBingoCallIndex 是達成的呼叫次數
            //printf("%d ", bingoTime[i]);
            printf("%d ", calledNumbers[bingoTime[i]-1]);
            break;

        }
        
    }
    for(int i = 0; i < n; i++){
        if(bingoTime[i] == firstBingoCallIndex) {
            // i+1 表示玩家的編號(1-based)，firstBingoCallIndex 是達成的呼叫次數
            printf("%s ", names[i]);

        }

        
    }
    printf("\n");
    

    return 0;
}
