#include "1.h"
/*******************************************************
這題目是要我們判斷公司中兩個員工之間的關係，使用樹狀結構來表示上下級關係。

關係定義如下：
relation(員工A, 員工B) 
下屬關係（回傳1）：如果員工A可以通過boss指針找到員工B
主管關係（回傳2）：如果員工B是員工A的下屬
同事關係（回傳3）：如果員工A和B有共同的最高主管
無關係  （回傳4）：如果以上關係都不存在
 ******************************************************/
int relation(Employee *employee1, Employee *employee2) {
    //same name
    int a ;    
    if (employee1 == employee2) {
        a = 0;
        return 4;
    }
      // 檢查employee1是否為employee2的下屬
    Employee *current = employee1; // 先給一個pointer指向employee1
    // struct 內 employee1->boss 會指向上司
    // 如果employee1->boss == employee2，代表employee1是employee2的下屬
    // 一直往上找，直到找到boss
    while (current != current->boss) {
        if (current->boss == employee2) {
            return 1; // employee1是employee2的下屬
            a = 1;
        }
        current = current->boss;
    }//執行結束後，current會指向A的最高主管
    
    // same logic for employee2
    Employee *current2 = employee2;
    while (current2 != current2->boss) {
        if (current2->boss == employee1) {
            a = 1;
            return 2; // employee1是employee2的上司
        }
        current2 = current2->boss;
    }//執行結束後，current2會指向B的最高主管
    if (current == current2) {
        a = 1;
        return 3; // employee1和employee2有共同的最高主管
    }
    if (employee1 == employee2||a == 0) {
        
        return 4;
    }
    
}