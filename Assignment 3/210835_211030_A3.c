#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//the below code is for the implementation of stack

struct Node{
    int val;
    struct Node *prev;
};

void Push(int n, struct Node **head)
{
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->val=n;
    temp->prev=*head;
    *head=temp;
}

int Pop(struct Node** head)
{
        struct Node* temp = *head;
        int v= (*head)->val;
        *head=(*head)->prev;
        free(temp);
        return v;
}

int Top(struct Node* head)
{
    return head->val;
}

int Size(struct Node* head)
{
       int c=0;
       struct Node* temp = head;
       while(temp!=NULL)
       {
           c++;
           temp=temp->prev;
       }
       return c;
}


//algorithm for computing the result of arithmetic expression is given below

/*We can divide what operations have to be done based on the newest token into broadly 5 categories:
1. When scanning digits, the number is stored in num which will subsequently be pushed to the number_stack
2. When scanning operators, we will compare its priority with the top of the stack and execute operations that we can
from the stack. After that we will push the newest operator on the stack.
3. When we have scanned a '(', all we have to do is push it onto the stack.
4. When we scan a ')' this implies that we must now execute all operations that haven't been executed with its
corresponding '('. and then push this ')' onto the operator stack
5. When the Top of the operator stack is ')' that means that we have just scanned the next operator that succeded
a ')' in the expression. 
eg. 5+3*(10-4)/2 : in this case when we have scanned the '/', the top of the operator stack will be a ')'. 
All that we have to do is Push the '/' onto the operator stack.
*/

//Function to calculate the priority of the elements present in the stack (to check whether they should be executed)
int Priority_Inside_Stack(char op) {
    if(op == '#' || op == '(') {
        return 0;
    }
    else if(op == '+' || op == '-') {
        return 1;
    }
    else if(op == '*' || op == '/') {
        return 2;
    }
    else { //when op is '^'
        return 3;
    }
}

//Function to calculate the priority of the newest scanned operator that has not been pushed to the stack yet
int Priority_Outside_Stack(char op) {
    if(op == '+' || op == '-') {
        return 1;
    }
    else if(op == '*' || op == '/') {
        return 2;
    }
    else if(op == '^') {
        return 3;
    }
    else { //when op is '('
        return 4;
    }
}

/*When we find that an operation should be performed, we call this function that computes the result of that
single operation and pushes the result onto the number_stack*/
void execute_single_operation(struct Node **operators, struct Node **numbers) {
    //this will execute a single operation of the form num1 (operator) num2 where operator can be any valid operator and then push the result
    //num3 on the number stack.
    int num1, num2, num3 = 0;
    char op;

    num2 = Pop(numbers);
    num1 = Pop(numbers);
    op = Pop(operators);

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
    Push(num3, numbers);
}

//computes and returns the result of the entire arithmetic expression
int compute_result(int str_len) {
    //given the length of the arithmetic expression (stored in str_len), we will compute the entire expression using a single scan by maintaining     2 stacks : 1 for the numbers and 1 for the operators. The expression has been evaluated while giving proper attention to associativity &         precedence as defined in the problem statement.
    char curr; //stores the newest scanned character
    int num = 0;
    //as the digits of a number are scanned character by character, num stores the value of the number and when we come across the next operator,     we know that our number has now been completely scanned so we push num onto the stack of numbers
    struct Node *number_stack = NULL;
    struct Node *operator_stack = NULL;
    Push('#',&operator_stack);
    //'#' will be used as the bottom of the stack. We will know that we have completed the evaluation of the expression if 
    //Top(operator_stack) =='#' i.e. all operators (as they appeared in the expression) that were pushed have also been executed (and popped)

    for(int i=0; i<str_len; i++) {
        scanf("%c", &curr);

        if(curr <= '9' && curr >= '0') { //if character is a digit
            num = num*10 + (int)(curr - '0');
        }

        else { //if character is an operator
            if(Top(operator_stack) == ')') {
                //this means that in the previous iteration of the loop, we had executed all operations inside a parantheses. Now we have come                   across the next operator, so we will Pop the ')' and then push the new operator (done on line number 170)
                Pop(&operator_stack);
            }
            else if(curr != '(') {
                    Push(num, &number_stack);
                    num = 0; //num reassigned to zero so that it can now store the next number appearing in the expression
            }
                    
            if(curr == ')') { //parantheses are being closed so we need to execute all operations appearing inside the ( )
                while(Top(operator_stack) != '(') {
                    execute_single_operation(&operator_stack, &number_stack);
                }
                Pop(&operator_stack);
            } else if(curr == '+' || curr == '*') {
                while(Priority_Inside_Stack(Top(operator_stack)) >= Priority_Outside_Stack(curr)) {
                    //>= ensures that the '+' and '*' operators are treated as left associative
                    execute_single_operation(&operator_stack, &number_stack);
                }
            } else {
                //> ensures that the operators will be evaluated from right to left in the expression (right associative)
                while(Priority_Inside_Stack(Top(operator_stack)) > Priority_Outside_Stack(curr)) {
                    execute_single_operation(&operator_stack, &number_stack);
                }
            }
            Push(curr, &operator_stack);
        }
    }

    if(Top(operator_stack) == ')') { //the expression was terminated with a ')'
        Pop(&operator_stack);
    }
    else { //the expression was terminated with a number that was stored in num but wasn't pushed onto the number stack
        Push(num, &number_stack);
    }

    while(Top(operator_stack) != '#') { //executing all remaining operations
        execute_single_operation(&operator_stack, &number_stack);
    }

    return Top(number_stack); //the number remaining in the number stack is the result of the arithmetic expression
}

int main() {
    int str_len;
    scanf("%d ", &str_len);

    printf("%d", compute_result(str_len));
    return 0;
}