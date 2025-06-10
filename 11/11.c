#include <stdio.h>
#include <stdlib.h>
#include <limits.h>  // 提供 CHAR_BIT，用於計算 log2
#include <stdbool.h> // 提供布林型別

#define MAX_N 1000001  // 最大支援的操作數 +1（因為從 index=1 開始建 heap）

// 定義一個結構來存放工作資訊：優先權與 job_id
typedef struct {
    unsigned int priority; // 工作的優先權
    unsigned int job_id;   // 工作的識別號
} Job;

// 用陣列實作 Min-Max Heap，heap[0] 不使用，從 index=1 開始
Job heap[MAX_N];
int heap_size = 0; // 當前堆積中有幾個工作

// 交換 heap 中第 i 和第 j 筆工作的資料
void swap_jobs(int i, int j) {
    Job temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

// 取得一個正整數的 floor(log2(x))
// 使用 __builtin_clz：計算前導 0 的個數，效率比 log2 快
int floor_log2(unsigned int i) {
    if (i == 0) return -1; // index 為 0 不合法
    return (sizeof(unsigned int) * CHAR_BIT - 1) - __builtin_clz(i);
}

// 判斷某個節點的層數是否是「min 層」（偶數層）
// min 層維護的是「最小值」性質；max 層維護「最大值」
int is_min_level(int i) {
    int level = floor_log2(i); // 取得層數
    return (level % 2 == 0);   // 層數是偶數則為 min 層
}

// 從 min 層的節點往上調整（grandparent-based bubble up）
void bubble_up_min(int i) {
    int gp = i / 4; // grandparent index
    while (gp >= 1 && heap[i].priority < heap[gp].priority) {
        swap_jobs(i, gp);
        i = gp;
        gp = i / 4;
    }
}

// 從 max 層的節點往上調整（grandparent-based bubble up）
void bubble_up_max(int i) {
    int gp = i / 4;
    while (gp >= 1 && heap[i].priority > heap[gp].priority) {
        swap_jobs(i, gp);
        i = gp;
        gp = i / 4;
    }
}

// 一般插入節點時的 bubble up 判斷邏輯（會自動判斷是 min 還是 max 層）
void bubble_up(int i) {
    int p = i / 2;
    if (p == 0) return; // 已到根節點

    if (is_min_level(i)) {
        // 若 priority 比 parent 大，應屬 max 層 → swap 上去後進行 max 調整
        if (heap[i].priority > heap[p].priority) {
            swap_jobs(i, p);
            bubble_up_max(p);
        } else {
            bubble_up_min(i); // 否則照 min 層向上調整
        }
    } else {
        // 若 priority 比 parent 小，應屬 min 層 → swap 後進行 min 調整
        if (heap[i].priority < heap[p].priority) {
            swap_jobs(i, p);
            bubble_up_min(p);
        } else {
            bubble_up_max(i); // 否則照 max 層向上調整
        }
    }
}

// trickle_down：從 index i 開始往下維護 min-max heap 性質
void trickle_down(int i) {
    while (1) {
        int current_is_min = is_min_level(i);
        int m = i; // 最佳候選節點位置
        int first_child = 2 * i;
        int right_child = first_child + 1;

        // --- 找出最優秀的子節點（min 或 max）---
        int best_child = -1;
        if (first_child <= heap_size) {
            best_child = first_child;
            if (right_child <= heap_size) {
                // 比較左右子節點大小
                if (current_is_min) {
                    if (heap[right_child].priority < heap[first_child].priority) {
                        best_child = right_child;
                    }
                } else {
                    if (heap[right_child].priority > heap[first_child].priority) {
                        best_child = right_child;
                    }
                }
            }

            // 若最佳子節點比目前節點更優，就暫時記錄為 m
            if (current_is_min) {
                if (heap[best_child].priority < heap[m].priority) m = best_child;
            } else {
                if (heap[best_child].priority > heap[m].priority) m = best_child;
            }
        }

        // --- 找出最佳的孫節點（最多四個）---
        int first_grandchild = 4 * i;
        int best_grandchild = -1;
        for (int k = 0; k < 4; ++k) {
            int gc_idx = first_grandchild + k;
            if (gc_idx > heap_size) break; // 越界則停止

            if (best_grandchild == -1) {
                best_grandchild = gc_idx;
            } else {
                if (current_is_min) {
                    if (heap[gc_idx].priority < heap[best_grandchild].priority)
                        best_grandchild = gc_idx;
                } else {
                    if (heap[gc_idx].priority > heap[best_grandchild].priority)
                        best_grandchild = gc_idx;
                }
            }
        }

        // 若孫子比子節點更優，更新最佳節點位置
        if (best_grandchild != -1) {
            if (current_is_min) {
                if (heap[best_grandchild].priority < heap[m].priority) m = best_grandchild;
            } else {
                if (heap[best_grandchild].priority > heap[m].priority) m = best_grandchild;
            }
        }

        if (m == i) break; // 若 m 還是自己，表示符合 heap 性質，不需下沉

        // 執行交換
        swap_jobs(i, m);

        // 若 m 是孫子，需多做一次與父親的性質檢查
        bool m_is_grandchild = (m >= first_grandchild && first_grandchild <= heap_size);
        if (m_is_grandchild) {
            int parent_m = m / 2;
            if (current_is_min) {
                if (heap[m].priority > heap[parent_m].priority)
                    swap_jobs(m, parent_m);
            } else {
                if (heap[m].priority < heap[parent_m].priority)
                    swap_jobs(m, parent_m);
            }
        }

        i = m; // 繼續從新的位置往下檢查
    }
}

// 插入新工作
void add_job(unsigned int job_id, unsigned int priority) {
    if (heap_size >= MAX_N - 1) return; // 檢查是否爆滿
    heap[++heap_size].job_id = job_id;
    heap[heap_size].priority = priority;
    bubble_up(heap_size);
    printf("%d jobs waiting\n", heap_size);
}

// 移除最大值的工作
void complete_job() {
    if (heap_size == 0) {
        printf("no job in queue\n");
        return;
    }

    int max_idx;
    if (heap_size == 1) max_idx = 1;
    else if (heap_size == 2) max_idx = 2;
    else max_idx = (heap[2].priority > heap[3].priority) ? 2 : 3;

    Job max_job = heap[max_idx];
    printf("job %u with priority %u completed\n", max_job.job_id, max_job.priority);

    if (heap_size == 1) {
        heap_size--;
        return;
    }

    heap[max_idx] = heap[heap_size--];
    trickle_down(max_idx);
}

// 移除最小值（root）
void drop_job() {
    if (heap_size == 0) {
        printf("no job in queue\n");
        return;
    }

    Job min_job = heap[1];
    printf("job %u with priority %u dropped\n", min_job.job_id, min_job.priority);

    if (heap_size == 1) {
        heap_size--;
        return;
    }

    heap[1] = heap[heap_size--];
    trickle_down(1);
}

// 主程式：處理輸入與操作
int main() {
    int N;
    if (scanf("%d", &N) != 1) return 1;

    for (int k = 0; k < N; ++k) {
        int type;
        if (scanf("%d", &type) != 1) return 1;
        if (type == 1) {
            unsigned int job_id, priority;
            if (scanf("%u %u", &job_id, &priority) != 2) return 1;
            add_job(job_id, priority);
        } else if (type == 2) {
            complete_job();
        } else if (type == 3) {
            drop_job();
        }
    }
    return 0;
}
