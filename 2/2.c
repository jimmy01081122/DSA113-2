#include <stdio.h>
#include <stdint.h>

#define SHELF_SIZE 8  // 書架最多可存 8 本書

// 讀取書本並更新書架
void read_book(uint64_t *shelf, uint8_t book_id) {
    // **複製書架內容**
    uint64_t temp_shelf = *shelf;
    uint8_t books[SHELF_SIZE] = {0}; // 用於存放當前書架上的書籍
    // ex: 0x123456789ABCDEF0
    //     0x12 0x34 0x56 0x78 0x9A 0xBC 0xDE 0xF0
    //對應到book[0]~book[7]
    int count = 0;

    // **拆解書架內容，將 8 本書存入陣列**
    for (int i = SHELF_SIZE - 1; i >= 0; i--)
    //給迴圈，查看temp store 並對應到book[i]
    
    {
        books[i] = temp_shelf & 0xFF; // 取得最低 8 bit
        temp_shelf >>= 8;  // 右移 8 bit 以取得下一本書
    }
    // books = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}
    //            0     1     2    3      4    5     6      7
    // **檢查書籍是否已經在書架上**
    int found = -1;
    for (int i = 0; i < SHELF_SIZE; i++) {
        if (books[i] == book_id) {
            found = i;
            break;
        }
    }

    // **若書已存在，則移除舊位置**
    if (found != -1) {
        for (int i = found; i < SHELF_SIZE - 1; i++) {
            books[i] = books[i + 1];  // 向左移動
        }
    } else { 
        // **若書不在書架上，需移除最左邊的書**
        for (int i = 0; i < SHELF_SIZE - 1; i++) {
            books[i] = books[i + 1];  // 向左移動
        }
    }

    // **將新書放入最右側**
    books[SHELF_SIZE - 1] = book_id;

    // **重新組合書架**
    *shelf = 0;
    for (int i = 0; i < SHELF_SIZE; i++) {
        *shelf = (*shelf << 8) | books[i];
    }
}

// **輸出書架狀態**
void print_shelf(uint64_t shelf, int type) {
    if (type == 1) {
        printf("%llu\n", shelf);  // 直接輸出 `unsigned long long int` 數值
    } else if (type == 2) {
        uint8_t books[SHELF_SIZE] = {0};
        for (int i = SHELF_SIZE - 1; i >= 0; i--) {
            books[i] = shelf & 0xFF;
            shelf >>= 8;
        }
        for (int i = 0; i < SHELF_SIZE; i++) {
            printf("%d ", books[i]);
        }
        printf("\n");
    }
}

int main() {
    int query_count;
    uint64_t shelf = 0; // 64-bit 數值，表示書架

    scanf("%d", &query_count); // 讀取查詢次數

    for (int i = 0; i < query_count; i++) {
        int command;
        scanf("%d", &command);

        if (command == 1) {
            uint8_t book_id;
            scanf("%hhu", &book_id);
            read_book(&shelf, book_id);
        } else if (command == 2) {
            int type;
            scanf("%d", &type);
            print_shelf(shelf, type);
        }
    }

    return 0;
}
