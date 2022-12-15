#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*things to take care of:
Associativity
Precedence
Parentheses- When we come across '(', when we come across ')', when we come across the operator after ')'*/
//Implementation of Stack 
struct Node {
    int value;
    struct Node *prev;
};


struct Node* CreateNode(int value) {
    struct Node *new_Node = (struct Node *) malloc(sizeof(struct Node));
    new_Node->value = value;
    new_Node->prev = NULL;
    return new_Node;
}

bool isEmpty(struct Node *S) {
    if(S == NULL) {
        return true;
    }
    return false;
}

int Top(struct Node *S) {
    return S->value;
}

struct Node* CreateEmptyStack() {
    return NULL;
}

struct Node* Push(int x, struct Node* S) {
    struct Node *new_top = CreateNode(x);
    new_top->prev = S;
    S = new_top;
    return S;
}

struct Node* Pop(struct Node* S) {
    struct Node *temp = S;
    S = S->prev;
    free(temp);
    return S;
}

//algorithm for expression evaluation

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

int execute_operation(int n1, int n2, char operator) {
    if(operator == '+') {
        return n1 + n2;
    }
    else if(operator == '-') {
        return n1 - n2;
    }
    else if(operator == '*') {
        return n1 * n2;
    }
    else if(operator == '/') {
        return n1 / n2;
    }
    else if(operator == '^') {
        return pow(n1, n2);
    }
}

int evaluate_expression(struct Node *operator_stack, struct Node *number_stack) {
    int num = 0, multiplier = 1, num1 = 0, num2 = 0;
    char curr = '\0';

    operator_stack = Push('#', operator_stack);
    
    scanf("%c", &curr);

    while(curr != '\n') {
        printf("curr at beg = %c\n", curr);

        if(curr <= '9' && curr >= '0') {
            num = num * 10 + (int)(curr - '0');
        }

        else {
            if(curr == '(') {
                operator_stack = Push(curr, operator_stack);
            }
            else if(curr == ')') {
                while(Top(operator_stack) != '(') {
                    num2 = num;
                    num1 = Top(number_stack);
                    number_stack = Pop(number_stack);
                    number_stack = Push(execute_operation(num1, num2, Top(operator_stack)), number_stack);
                    operator_stack = Pop(operator_stack);
                }
                printf("Top should be ( : %c\n", Top(operator_stack));
                operator_stack = Pop(operator_stack);
                operator_stack = Push(curr, operator_stack);
            }

            else {
                if(Top(operator_stack) == ')') {
                    operator_stack = Pop(operator_stack);
                }
                else {
                    number_stack = Push(num, number_stack);
                    num = 0;
                }


                if(curr == '+' || curr == '*') {
                    while(Priority_Inside_Stack(Top(operator_stack)) >= Priority_Outside_Stack(curr)) { //could be 'if' also I think
                        //checking based on left associativity
                        num2 = Top(number_stack);
                        number_stack = Pop(number_stack);
                        num1 = Top(number_stack);
                        number_stack = Pop(number_stack);
                        number_stack = Push(execute_operation(num1, num2, Top(operator_stack)), number_stack);
                        operator_stack = Pop(operator_stack);
                    }
                    operator_stack = Push(curr, operator_stack);
                }
                else {
                    while(Priority_Inside_Stack(Top(operator_stack)) > Priority_Outside_Stack(curr)) {
                        //checking based on right associativity
                        num2 = Top(number_stack);
                        number_stack = Pop(number_stack);
                        num1 = Top(number_stack);
                        number_stack = Pop(number_stack);
                        number_stack = Push(execute_operation(num1, num2, Top(operator_stack)), number_stack);
                        operator_stack = Pop(operator_stack);
                    }
                    operator_stack = Push(curr, operator_stack);
                }
            }
        }
        scanf("%c", &curr);
        if(number_stack != NULL)
            printf("Top num = %d\nTop Op = %c\n", Top(number_stack), Top(operator_stack));
        printf("curr at end= %c\n\n", curr);
    }
    if(Top(operator_stack) != ')') {
        number_stack = Push(num, number_stack);
    }
    else {
        operator_stack = Pop(operator_stack);
    }

    while(Top(operator_stack) != '#') {
        num2 = Top(number_stack);
        number_stack = Pop(number_stack);
        num1 = Top(number_stack);
        number_stack = Pop(number_stack);
        number_stack = Push(execute_operation(num1, num2, Top(operator_stack)), number_stack);
        operator_stack = Pop(operator_stack);
    }

    return Top(number_stack);
}

int main() {
    struct Node *operator_stack = CreateEmptyStack();
    struct Node *number_stack = CreateEmptyStack();

    printf("%d", evaluate_expression(operator_stack, number_stack));


    return 0;
}



// int main() {
//     // struct Node *operator_stack = CreateEmptyStack();
//     // struct Node *number_stack = CreateEmptyStack();
//     struct Node *test = CreateEmptyStack();

//     int n;
//     scanf("%d", &n);

//     while(n != 0) {
//         test = Push(n, test);
//         scanf("%d", &n);
//     }
//     printf("Top : %d\n", Top(test));
//     if(isEmpty(test)) {
//         printf("yes1\n");
//     }

//     while(test != NULL) {
//         printf("%d\n", Top(test));
//         test = Pop(test);
//     }
//     if(isEmpty(test)) {
//         printf("yes2");
//     }
//     return 0;
// }