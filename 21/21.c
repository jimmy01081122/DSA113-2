#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 4096

// 定義 stack 結構
typedef struct {
    char data[MAX_LEN];
    int top;
} CharStack;

typedef struct {
    long long data[MAX_LEN];
    int top;
} NumStack;

// 初始化 stack
void initCharStack(CharStack *s) {
    s->top = -1;
}
void initNumStack(NumStack *s) {
    s->top = -1;
}

// 操作 stack 的基本功能
void pushChar(CharStack *s, char c) {
    s->data[++(s->top)] = c;
}
char popChar(CharStack *s) {
    return s->data[(s->top)--];
}
char peekChar(CharStack *s) {
    return s->data[s->top];
}
int isEmptyChar(CharStack *s) {
    return s->top == -1;
}

void pushNum(NumStack *s, long long v) {
    s->data[++(s->top)] = v;
}
long long popNum(NumStack *s) {
    return s->data[(s->top)--];
}

// 定義運算子優先順序
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    return 0;
}

// 將中序轉為後序
void infixToPostfix(char *infix, char *postfix) {
    CharStack s;
    initCharStack(&s);
    int i = 0, j = 0;

    while (infix[i]) {
        if (isdigit(infix[i])) {
            // 處理多位數字
            while (isdigit(infix[i])) {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = '#'; // 數字結束用 '#' 區隔
        } else if (infix[i] == '(') {
            pushChar(&s, infix[i++]);
        } else if (infix[i] == ')') {
            while (!isEmptyChar(&s) && peekChar(&s) != '(') {
                postfix[j++] = popChar(&s);
            }
            popChar(&s); // 移除左括號 '('
            i++;
        } else {
            // 遇到運算子：先把比它優先的先彈出
            while (!isEmptyChar(&s) && peekChar(&s) != '(' &&
                   precedence(peekChar(&s)) >= precedence(infix[i])) {
                postfix[j++] = popChar(&s);
            }
            pushChar(&s, infix[i++]);
        }
    }

    // 剩下的運算子通通彈出
    while (!isEmptyChar(&s)) {
        postfix[j++] = popChar(&s);
    }

    postfix[j] = '\0';
}

// 執行後序運算
long long evalPostfix(char *postfix) {
    NumStack s;
    initNumStack(&s);
    int i = 0;
    while (postfix[i]) {
        if (isdigit(postfix[i])) {
            // 將多位數轉為整數
            long long num = 0;
            while (isdigit(postfix[i])) {
                num = num * 10 + (postfix[i++] - '0');
            }
            pushNum(&s, num);
            if (postfix[i] == '#') i++; // 跳過分隔符號
        } else {
            long long n2 = popNum(&s);
            long long n1 = popNum(&s);
            long long res;
            switch (postfix[i]) {
                case '+': res = n1 + n2; break;
                case '-': res = n1 - n2; break;
                case '*': res = n1 * n2; break;
                case '/': res = n1 / n2; break;
                case '%': res = n1 % n2; break;
            }
            pushNum(&s, res);
            i++;
        }
    }
    return popNum(&s);
}

int main() {
    char line[MAX_LEN];
    for (int i = 0; i < 3; i++) {
        scanf("%s", line);
        char postfix[MAX_LEN];
        infixToPostfix(line, postfix);
        long long val = evalPostfix(postfix);

        // 輸出 postfix，移除分隔符號 '#'
        for (int j = 0; postfix[j]; j++) {
            if (postfix[j] != '#') putchar(postfix[j]);
        }
        printf("=%lld\n", val);
    }
    return 0;
}
