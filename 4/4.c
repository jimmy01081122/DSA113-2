#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************
寫一個程式來玩文字遊戲。您的程式應該支援以下命令。

insert left x   (左插入 x)  ：在單字開頭插入字元 x。
insert right x  (右插入 x)  ：在單字末尾插入字元 x。
insert k x      (插入 k x)  : 插入字元 x 作為該單字的第 k 個字元。
delete left     (左側刪除)  ：刪除單字開頭的一個字元。
delete right    (右刪除)    ：刪除單字末尾的一個字元。
delete k                    ：從單字中刪除第 k 個字元。
其中 x 是空格以外的字符，k 是從 1 開始的數字。
***************************************/



#define MAX_LEN 5000

char word[MAX_LEN + 1] = ""; // 動態變長字串
int length = 0;  // 當前字串長度

// 插入字符 insert left x
//memove : 
// void *memmove(void *dest, const void *src, size_t n);
// dest -- 這是指向目標陣列的指標，其內容是將被複製到的內容。
// src -- 這是指向源陣列的指標，其內容要被複製。
// size_t n -- 這是要複製的字節數。
// ex: memmove(word + 1, word, length + 1);
//     word[] = "hello"
//     word[1] = 'e'
//     word[0] = 'h'
//     answer = "ehello"
void insert_left(char x) {
    if (length >= MAX_LEN) return;
    memmove(word + 1, word, length + 1);
    word[0] = x;
    length++;
}
// 插入字符 insert right x
// defalut length = 0
// char[0] = x ,char[1] = '\0'

void insert_right(char x) {
    if (length >= MAX_LEN) return;
    word[length] = x;
    length++;
    word[length] = '\0';
}

/********************************************
1. 參數說明
    k: 要插入的位置（從1開始計數）
    x: 要插入的字符
    word: 全局字符數組
    length: 當前字符串長度
2. 運作步驟
----------------
EX : 
// 1. 檢查 k 值是否合法
if (k < 1 || k > length + 1) return;
----------------
    k必須在1到length+1之間
    例如：如果字串長度是3，k可以是1,2,3,4
    假設原字串是 "ABC"，要在位置2插入'X'：
    -    word + k: 目標位置（"BC"的起始位置）
    -    word + k - 1: 源位置（"BC"的當前位置）
    -    length - (k - 1) + 1: 要移動的字符數（包括'\0'）
----------------
// 3. 插入新字符
word[k - 1] = x;
----------------
在騰出的空位插入新字符
----------------
// 4. 更新長度
length++;
----------------
3. 實際例子
假設：
----------------
原字串: "ABC" (length = 3)
要插入: 'X' 在位置 2
----------------
步驟展示：
1. 初始狀態：    "ABC\0"
2. 移動後：      "ABBC\0"  (BC向右移動)
3. 插入X：       "AXBC\0"
4. 長度+1：      length = 4
----------------
4. memmove的作用
    安全地處理重疊的記憶體區域
    確保在移動數據時不會破壞原有數據
    memmove(dest, src, size)：從src複製size個字節到dest
這就是為什麼我們使用memmove而不是簡單的循環賦值，它能確保數據移動的正確性和效率。
*********************************************************** */

void insert_k(int k, char x) {
    if (k < 1 || k > length + 1) return; 
    if (length >= MAX_LEN) return;
    // k 不合法
    memmove(word + k, word + k - 1, length - (k - 1) + 1);
    word[k - 1] = x;
    length++;
    word[length] = '\0';
}

// 刪除字符
void delete_left() {
    if (length == 0) return;
    //防呆
    memmove(word, word + 1, length);
    length--;
    word[length] = '\0';
}


void delete_right() {
    if (length == 0) return;
    word[length - 1] = '\0';
    length--;
}
void delete_k(int k) {
    if (k < 1 || k > length) return;
    memmove(word + k - 1, word + k, length - k + 1);
    length--;
    word[length] = '\0';
}
// 新增一個印出當前字串的函數
void print_current_string() {
    if (length == 0) {
        printf("Current string: (empty)\n");
    } else {
        printf("Current string: %s (length: %d)\n", word, length);
    }
}
// 找出最長連續序列
// After processing the input commands, Your program should find out all of the longest consecutive sequence
// # Leetcode : Longest Palindromic Substring(最長回文子字串) 

// 主要修正在這個函數
void find_longest_sequence() {
    if (length == 0) {
        printf("0\n");
        return;
    }

    // 清空輸出
    char longest_chars[2 * MAX_LEN + 1] = "";
    int max_count = 1;

    // 開始掃字串
    char current_char = word[0];
    int current_count = 1;

    for (int i = 1; i < length; i++) {
        if (word[i] == current_char) {
            // 同字元，持續累加
            current_count++;
            // 若大於現有 max，就更新 max，但「暫時」別馬上輸出
            if (current_count > max_count) {
                max_count = current_count;
                // 一旦有新的最大值，需要把 longest_chars 清掉
                // 因為舊的字元不再是最長
                longest_chars[0] = '\0';
            }
        } else {
            // 跑到不一樣的字元，先看這一段是否等於 max_count
            if (current_count == max_count) {
                // 加到輸出（只加一次該字元）
                char temp[5];
                sprintf(temp, "%c ", current_char);
                strcat(longest_chars, temp);
            }
            // 換新字元開始計數
            current_char = word[i];
            current_count = 1;
            // 檢查是否需要更新 max_count
            if (current_count > max_count) {
                max_count = current_count;
                longest_chars[0] = '\0';  // 清空
            }
        }
    }

    // 迴圈結束後，最後一段還要處理
    if (current_count == max_count) {
        char temp[5];
        sprintf(temp, "%c ", current_char);
        strcat(longest_chars, temp);
    }

    // 輸出
    printf("%s%d\n", longest_chars, max_count);
}

int main() {
    char command[20], x;
    int k;
    char input[50];

    while (fgets(input, sizeof(input), stdin)) {
        char action[10], position[10];
        if (sscanf(input, "%s %s %c", action, position, &x) == 3) {
            if (strcmp(action, "insert") == 0) {
                if (strcmp(position, "left") == 0) {
                    insert_left(x);
                } else if (strcmp(position, "right") == 0) {
                    insert_right(x);
                } else {
                    k = atoi(position);
                    insert_k(k, x);
                }
                //print_current_string();  // 新增
            }
        } else if (sscanf(input, "%s %s", action, position) == 2) {
            if (strcmp(action, "delete") == 0) {
                if (strcmp(position, "left") == 0) {
                    delete_left();
                } else if (strcmp(position, "right") == 0) {
                    delete_right();
                } else {
                    k = atoi(position);
                    delete_k(k);
                }
                //print_current_string();  // 新增
            }
        }
    }

    find_longest_sequence();
    return 0;
}
