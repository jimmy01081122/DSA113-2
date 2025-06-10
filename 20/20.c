#include <stdio.h>
#include <stdlib.h>

// 使用 long long 表示節點的資料值範圍
typedef long long ll;

// 定義每個節點的結構：包含 data 和 next
typedef struct {
    ll data;   // 節點中的資料值
    int next;  // 指向下一個節點的索引（0 代表 nil）
} Node;

int main() {
    int N;
    scanf("%d", &N);  // 讀入節點數量

    // 配置陣列來儲存所有節點，從 index 1 開始
    Node *nodes = (Node *)malloc((N + 1) * sizeof(Node));

    // 依序讀入每個節點的資料與 next 指標
    for (int i = 1; i <= N; i++) {
        scanf("%lld %d", &nodes[i].data, &nodes[i].next);
    }

    // 初始化烏龜（tortoise）與野兔（hare）的位置都在 head（節點 1）
    int hare = 1, tortoise = 1;
    int met = 0;  // 標記是否有遇到 cycle

    // 用來紀錄野兔走過的所有節點索引（順序）
    int *visited = (int *)malloc((N + 10) * sizeof(int));
    int visit_count = 0;

    // 開始執行 Floyd Cycle Detection Algorithm
    while (hare != 0 && nodes[hare].next != 0) {
        // 每次迴圈開始前，把當前野兔位置記錄下來
        visited[visit_count++] = hare;

        // 更新 hare（野兔）往前跳兩步
        hare = nodes[nodes[hare].next].next;

        // 更新 tortoise（烏龜）往前跳一步
        tortoise = nodes[tortoise].next;

        // 如果烏龜與野兔相遇，表示有 cycle
        if (hare == tortoise) {
            visited[visit_count++] = hare;  // 把相遇的節點也加入 visited
            met = 1;  // 記錄有遇到 cycle
            break;
        }
    }

    if (!met) {
        // 如果沒有遇到 cycle（也就是野兔跳到 null 結束）
        // 可能 hare 還不是 0，要記錄最後一次
        if (hare != 0) visited[visit_count++] = hare;

        // 印出從 head 開始，野兔跳過的所有節點資料（每兩步一次）
        printf("%lld", nodes[visited[0]].data);
        for (int i = 1; i < visit_count; i++) {
            printf(" %lld", nodes[visited[i]].data);
        }
        printf("\n");
    } else {
        // 若遇到 cycle，印出相遇前（含相遇）的所有節點資料
        printf("%lld", nodes[visited[0]].data);
        for (int i = 1; i < visit_count; i++) {
            printf(" %lld", nodes[visited[i]].data);
        }
        printf("\n");
    }

    // 釋放記憶體
    free(nodes);
    free(visited);
    return 0;
}
