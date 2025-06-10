#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "9.h"

// 全域變數：頭、尾、以及下一個節點ID (由 9.h 宣告)
struct Node* head = NULL;
struct Node* tail = NULL;
int next_node_id = 1;

// 額外增加一個全域變數來記錄串列長度
static int list_size = 0;

// ----------------------------
// 基礎 XOR 函式：Insert_After, Remove_Here, Reverse
// ----------------------------

// 在 node (其前驅為 prev) 之後插入新節點
void Insert_After(struct Node* node, struct Node* prev) {
    if (!node) {
        // 若呼叫時 node 為 NULL，表示整個串列原本為空
        // 這種情況通常在 type_1() 裏已處理
        // 可依需求自行增補
        return;
    }

    // 1. 找出 node 原本的後繼 next
    struct Node* nxt = Next_Node(node, prev);

    // 2. 建立新節點 newNode，使其 neighbors = XOR(node, nxt)
    struct Node* newNode = New_XOR_Node(
        (struct Node*)((uintptr_t)node ^ (uintptr_t)nxt)
    );

    // 3. 若 nxt 存在，更新 nxt->neighbors：將原本的 node 替換成 newNode
    if (nxt) {
        nxt->neighbors = (struct Node*)(
            (uintptr_t)nxt->neighbors ^ (uintptr_t)node ^ (uintptr_t)newNode
        );
    }

    // 4. 更新 node->neighbors：將原本的 nxt 替換成 newNode
    node->neighbors = (struct Node*)(
        (uintptr_t)node->neighbors ^ (uintptr_t)nxt ^ (uintptr_t)newNode
    );

    // 5. 若 node 原本是 tail，則 newNode 成為新的 tail
    if (node == tail) {
        tail = newNode;
    }

    // 串列長度 +1
    list_size++;
}

// 移除 node (其前驅為 prev)
void Remove_Here(struct Node* node, struct Node* prev) {
    if (!node) return;

    // 1. 找出 node 的後繼 next
    struct Node* nxt = Next_Node(node, prev);

    // 2. 若要刪除的是 head 或 tail，需要更新
    if (node == head) {
        head = nxt;
    }
    if (node == tail) {
        tail = prev;
    }

    // 3. 更新 prev->neighbors：將 node 替換成 nxt
    if (prev) {
        prev->neighbors = (struct Node*)(
            (uintptr_t)prev->neighbors ^ (uintptr_t)node ^ (uintptr_t)nxt
        );
    }

    // 4. 更新 nxt->neighbors：將 node 替換成 prev
    if (nxt) {
        nxt->neighbors = (struct Node*)(
            (uintptr_t)nxt->neighbors ^ (uintptr_t)node ^ (uintptr_t)prev
        );
    }

    // 5. 釋放 node
    free(node);

    // 串列長度 -1
    list_size--;
}

// 反轉 [begin, end]，其外部為 prev (前) 與 next (後)
void Reverse(struct Node* prev, struct Node* begin, struct Node* end, struct Node* next) {
    // 若區段無效或單一節點，就不需要反轉
    if (!begin || !end || (begin == end)) return;

    // 1. prev->neighbors：原本指向 begin，現在要指向 end
    if (prev) {
        prev->neighbors = (struct Node*)(
            (uintptr_t)prev->neighbors ^ (uintptr_t)begin ^ (uintptr_t)end
        );
    }

    // 2. begin->neighbors：原本包含 prev，現在要改為 next
    begin->neighbors = (struct Node*)(
        (uintptr_t)begin->neighbors ^ (uintptr_t)prev ^ (uintptr_t)next
    );

    // 3. end->neighbors：原本包含 next，現在要改為 prev
    end->neighbors = (struct Node*)(
        (uintptr_t)end->neighbors ^ (uintptr_t)next ^ (uintptr_t)prev
    );

    // 4. next->neighbors：原本指向 end，現在要指向 begin
    if (next) {
        next->neighbors = (struct Node*)(
            (uintptr_t)next->neighbors ^ (uintptr_t)end ^ (uintptr_t)begin
        );
    }

    // 若包含 head 或 tail，需要更新
    if (begin == head) {
        head = end;
    }
    if (end == tail) {
        tail = begin;
    }
}

// ----------------------------
// 輔助函式：雙向遍歷，取得第 k (1-based) 個節點
// 回傳該節點指標，並透過 outPrev 輸出它的前驅
// ----------------------------
static struct Node* getKthNode(int k, struct Node** outPrev) {
    // 假設 k 已保證合法 (1 <= k <= list_size)
    // 判斷從頭或尾哪邊走比較快
    if (k <= (list_size / 2)) {
        // 從 head 正向前進 k-1 步
        struct Node* curr = head;
        struct Node* prev = NULL;
        int steps = k - 1;
        while (steps--) {
            struct Node* nxt = Next_Node(curr, prev);
            prev = curr;
            curr = nxt;
        }
        if (outPrev) *outPrev = prev;
        return curr;
    } else {
        // 從 tail 反向前進 (list_size - k) 步
        int steps = list_size - k;
        struct Node* curr = tail;
        struct Node* nxt = NULL;  // 反向遍歷時 "後一個" = NULL
        while (steps--) {
            struct Node* prev = Next_Node(curr, nxt);
            nxt = curr;
            curr = prev;
        }
        // 現在 curr 是第 k 個節點
        // "前驅" 則是 Next_Node(curr, NULL?) => 需要小心
        // 其實我們可以再反向走一步還原
        // 或乾脆在反向遍歷中保留上一次
        // 這裡示範簡單做法：再走一次 Next_Node
        struct Node* prev = Next_Node(curr, nxt);
        if (outPrev) *outPrev = prev;
        return curr;
    }
}

// ----------------------------
// 七種操作 (type_0 ~ type_6)
// ----------------------------

// type_0(k): 輸出第 k 個節點的 data
int type_0(int k) {
    if (k < 1 || k > list_size) {
        printf("Index out of range\n");
        return -1;
    }
    // 取得第 k 個節點
    struct Node* node = getKthNode(k, NULL);
    if (!node) {
        printf("Index out of range\n");
        return -1;
    }
    //printf("%d\n", node->data);
    return node->data;
}

// type_1(): 在頭部插入新節點
void type_1() {
    // 若串列為空，直接建立第一個節點
    if (!head) {
        head = New_XOR_Node(NULL); 
        tail = head;
        list_size = 1;
        return;
    }

    // 若串列非空，在 head 前插一個節點
    // 這裡做法：呼叫 Insert_After(head, NULL)，再交換 data
    Insert_After(head, NULL);
    // 新節點在 head 之後
    struct Node* newNode = Next_Node(head, NULL);

    // 交換 head 與 newNode 的 data
    int tmp = head->data;
    head->data = newNode->data;
    newNode->data = tmp;
}

// type_2(k): 在第 k 個節點後插入
void type_2(int k) {
    if (k < 1 || k > list_size) {
        printf("Index out of range\n");
        return;
    }
    // 找到第 k 個節點 (及其前驅)
    struct Node* prev = NULL;
    struct Node* curr = getKthNode(k, &prev);
    if (!curr) {
        printf("Index out of range\n");
        return;
    }
    // 呼叫 Insert_After
    Insert_After(curr, prev);
}

// type_3(k): 在「倒數第 k」個節點後插入
void type_3(int k) {
    if (k < 1 || k > list_size) {
        printf("Index out of range\n");
        return;
    }
    // 倒數第 k => 正數第 (list_size - k + 1)
    int pos = list_size - k + 1;
    // 找到第 pos 個節點
    struct Node* prev = NULL;
    struct Node* curr = getKthNode(pos, &prev);
    if (!curr) {
        printf("Index out of range\n");
        return;
    }
    Insert_After(curr, prev);
}

// type_4(k): 刪除第 k 個節點
void type_4(int k) {
    if (k < 1 || k > list_size) {
        printf("Index out of range\n");
        return;
    }
    struct Node* prev = NULL;
    struct Node* curr = getKthNode(k, &prev);
    if (!curr) {
        printf("Index out of range\n");
        return;
    }
    Remove_Here(curr, prev);
}

// type_5(k): 刪除倒數第 k 個節點
void type_5(int k) {
    if (k < 1 || k > list_size) {
        printf("Index out of range\n");
        return;
    }
    int pos = list_size - k + 1;
    struct Node* prev = NULL;
    struct Node* curr = getKthNode(pos, &prev);
    if (!curr) {
        printf("Index out of range\n");
        return;
    }
    Remove_Here(curr, prev);
}

// type_6(k): 反轉「第 k 個」到「倒數第 k 個」
void type_6(int k) {
    if (k < 1 || k > (list_size+1)/2) {
        printf("Index out of range\n");
        return;
    }
    // 第 k 個 => i1
    // 倒數第 k 個 => i2 = list_size - k + 1
    int i1 = k;
    int i2 = list_size - k + 1;
    if (i1 > i2) {
        printf("Index out of range\n");
        return;
    }

    // 找出 begin, prev_of_begin
    struct Node* prev_of_begin = NULL;
    struct Node* begin = getKthNode(i1, &prev_of_begin);

    // 找出 end, prev_of_end
    struct Node* prev_of_end = NULL;
    struct Node* end = getKthNode(i2, &prev_of_end);

    if (!begin || !end) {
        printf("Index out of range\n");
        return;
    }

    // 找出 end 的後繼 next_of_end
    struct Node* next_of_end = Next_Node(end, prev_of_end);

    // 呼叫 Reverse
    Reverse(prev_of_begin, begin, end, next_of_end);
}