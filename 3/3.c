#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FOOD 100
#define MAX_INGREDIENTS 10
#define MAX_LEN 1000

// 儲存食物名稱及其成分
char food[MAX_FOOD][MAX_LEN]; // 食物名稱
/*food[0] = "Pizza";
food[1] = "Burger";
food[2] = "Pasta";
*/
char ingredients[MAX_FOOD][MAX_INGREDIENTS][MAX_LEN]; // 成分名稱
/*
ingredients[0][0] = "cheese";
ingredients[0][1] = "tomato";
ingredients[0][2] = "dough";
ingredients[i][j] 二維陣列
ingredients[[i][j][number] 三維陣列
ingredients[MAX_FOOD][MAX_INGREDIENTS][MAX_LEN]
     └── 每一種食物有一個 [MAX_INGREDIENTS] x [MAX_LEN] 的成分表
*/
int ingredient_count[MAX_FOOD]; // 每種食物的成分數量
/*
ingredient_count[0] = 3; // Pizza 有 3 種成分
ingredient_count[1] = 3; // Burger 有 3 種成分
ingredient_count[2] = 4; // Pasta 有 4 種成分
*/
int food_count; // 食物數量

// qsort (double pointer)
int compare(const void *a, const void *b) {
    return strcmp((char *)a, (char *)b);
}

// 找出食物在陣列中的索引
int find_food_index(char *name) {
    if (food_count == 0) return -1; // 防止 food[] 未初始化
    //吃進來的name是要找的食物名稱
    for (int i = 0; i < food_count; i++) {
        if (strcmp(food[i], name) == 0) {
            return i;
        }
    }
    //找不到
    return -1;
}
//return 索引號碼
//找到了找交集
// 處理兩種食物的交集
void find_common_ingredients(char *food1, char *food2) {
    int idx1 = find_food_index(food1);
    //先找到食物在陣列中的索引
    int idx2 = find_food_index(food2);
    //找到第二個食物在陣列中的索引
    if (idx1 == -1 || idx2 == -1)
    //如果有沒在陣列中找到的食物
    {
        printf("nothing\n");
        return 0;
    }

    // 取出兩個食物的成分
    char (*ingredients1)[MAX_LEN] = ingredients[idx1];
    char (*ingredients2)[MAX_LEN] = ingredients[idx2];
    //各自有幾種成分
    /*******************************************
    ingredient_count[0] = 3; // Pizza 有 3 種成分
    ingredient_count[1] = 3; // Burger 有 3 種成分
    ingredient_count[2] = 4; // Pasta 有 4 種成分
    *********************************************/
    int count1 = ingredient_count[idx1];
    int count2 = ingredient_count[idx2];

    // **排序後使用雙指針找交集**
    //void qsort(void *base, size_t nitems, size_t size, 
    // int (*compar)(const void *, const void *));
    //base -- 指向要排序的陣列的第一個元素的指標。
    //nitems -- 陣列中的元素個數。
    //size -- 每個元素的大小，以位元組為單位。
    //compar -- 用來比較元素的函數。
    qsort(ingredients1, count1, sizeof(ingredients1[0]), compare);
    qsort(ingredients2, count2, sizeof(ingredients2[0]), compare);
    // !!!!!!!!!!!!!注意這裡元素大小是sizeof(ingredients1[0])!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!注意這裡元素大小是sizeof(ingredients1[0])!!!!!!!!!!!!!!!    
    // !!!!!!!!!!!!!注意這裡元素大小是sizeof(ingredients1[0])!!!!!!!!!!!!!!!
    /*
    qsort 需要知道「用哪個函數來比較」，而不是「比較的結果」
        compare 本身就代表函數的位址
        qsort 會在內部自己調用這個比較函數，所以不需要我們手動傳入參數
    這就像是：
        你告訴朋友「這是比較的方法」（傳遞 compare）
        而不是「這是兩個東西比較的結果」（傳遞 compare(a, b)）
    */    
    int i = 0, j = 0, found = 0;
    while (i < count1 && j < count2) {
        int cmp = strcmp(ingredients1[i], ingredients2[j]);
        if (cmp == 0) {  // 共同成分
            if (found) printf(" "); // 非第一個輸出要加空格
            printf("%s", ingredients1[i]);
            found = 1;
            i++;
            j++;
        } else if (cmp < 0) {
            i++;
        } else {
            j++;
        }
    }

    if (!found) printf("nothing");
    printf("\n");
}

int main() {
    int query_count;

    // 讀取食物數量
    scanf("%d", &food_count);

    // 讀取每個食物及其成分
    for (int i = 0; i < food_count; i++) {
        scanf("%s %d", food[i], &ingredient_count[i]);
        for (int j = 0; j < ingredient_count[i]; j++) {
            scanf("%s", ingredients[i][j]);
        }
    }

    // 讀取查詢數量
    scanf("%d", &query_count);
    for (int i = 0; i < query_count; i++) {
        char food1[MAX_LEN], food2[MAX_LEN];
        scanf("%s %s", food1, food2);
        find_common_ingredients(food1, food2);
    }

    return 0;
}
