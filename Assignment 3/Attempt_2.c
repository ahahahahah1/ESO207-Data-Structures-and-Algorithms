#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//implementation of stack
typedef struct Node {
    int value;
    struct Node *prev;
} Node;


Node* CreateNode(int value) {
    Node *new_Node = (Node *) malloc(sizeof(Node));
    new_Node->value = value;
    new_Node->prev = NULL;
    return new_Node;
}

bool isEmpty(Node *S) {
    if(S == NULL) {
        return true;
    }
    return false;
}

int Top(Node *S) {
    return S->value;
}

Node* CreateEmptyStack() {
    return NULL;
}

Node* Push(int x, Node* S) {
    Node *new_top = CreateNode(x);
    new_top->prev = S;
    S = new_top;
    return S;
}

Node* Pop(Node* S) {
    Node *temp = S;
    S = S->prev;
    free(temp);
    return S;
}


//algorithm for computing the result of arithmetic expression

int Priority_Inside_Stack(char op) {
    if(op == '#' || op == '(') {
        return 0;
    }
    if(op == '+' || op == '-') {
        return 1;
    }
    if(op == '*' || op == '/') {
        return 2;
    }
    if(op == '^') {
        return 3;
    }
}
int Priority_Outside_Stack(char op) {
    if(op == '+' || op == '-') {
        return 1;
    }
    if(op == '*' || op == '/') {
        return 2;
    }
    if(op == '^') {
        return 3;
    }
    if(op == '(') {
        return 4;
    }
}

void execute_single_operation(Node **operators, Node **numbers) {
    int num1, num2, num3;
    char op;

    num2 = Top(*numbers);
    *numbers = Pop(*numbers);
    num1 = Top(*numbers);
    *numbers = Pop(*numbers);
    op = Top(*operators);
    *operators = Pop(*operators);

    if(op == '+') {
        num3 = num1 + num2;
    }
    else if(op == '-') {
        num3 = num1 - num2;
    }
    else if(op == '*') {
        num3 = num1 * num2;
    }
    else if(op == '/') {
        num3 = num1 / num2;
    }
    else if(op == '^') {
        num3 = pow(num1, num2);
    }
    *numbers = Push(num3, *numbers);
}


int compute_result(int str_len) {
    char curr;
    int num = 0;
    Node *number_stack = CreateEmptyStack();
    Node *operator_stack = CreateEmptyStack();
    operator_stack = Push('#',operator_stack);

    for(int i=0; i<str_len; i++) {
        scanf("%c", &curr);

        if(curr <= '9' && curr >= '0') { //if character is a digit
            num = num*10 + (int)(curr - '0');
        }

        else { //if character is an operator
            if(Top(operator_stack) == ')') {
                operator_stack = Pop(operator_stack);
            } else if(curr == '(') {
                //nothing to be done, just Push the '(' on the stack (done on line number AJBSDABDIBIADS)
            } else {
                number_stack = Push(num, number_stack);
                num = 0; //num reassigned to zero for the next iteration where we begin the calculation of the next number

                if(curr == ')') {
                    while(Top(operator_stack) != '(') {
                        execute_single_operation(&operator_stack, &number_stack);
                    }
                    operator_stack = Pop(operator_stack);
                } else if(curr == '+' || curr == '*') {
                    while(Priority_Inside_Stack(Top(operator_stack)) >= Priority_Outside_Stack(curr)) {
                        execute_single_operation(&operator_stack, &number_stack);
                    }
                } else {
                    while(Priority_Inside_Stack(Top(operator_stack)) > Priority_Outside_Stack(curr)) {
                        execute_single_operation(&operator_stack, &number_stack);
                    }
                }
            }
            operator_stack = Push(curr, operator_stack);
        }
    }
    if(Top(operator_stack) == ')') {
        operator_stack = Pop(operator_stack);
    }
    else {
        number_stack = Push(num, number_stack);
    }

    while(Top(operator_stack) != '#') {
        execute_single_operation(&operator_stack, &number_stack);
    }

    return Top(number_stack);
}

int main() {
    int str_len;
    scanf("%d ", &str_len);

    printf("%d", compute_result(str_len));
    return 0;
}