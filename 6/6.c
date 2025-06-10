#include "6.h"

void prepare_array(int buffer[], int *array[], int row, int column[]) 
{
    // Implement here
    int offset = 0; // buffer 的起始索引
    for (int i = 0; i < row; i++) {
        array[i] = buffer + offset; // 設定 array[i] 指向 buffer 中的對應位置
        offset += column[i];        // 更新 offset，指向下一行的起始位置
    }
}