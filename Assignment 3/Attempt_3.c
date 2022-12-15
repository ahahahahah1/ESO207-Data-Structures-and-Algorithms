#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node {
    int val;
    struct Node *prev;
};

struct Node* CreateNode(int value) {
    struct Node *new_Node = (struct Node *) malloc(sizeof(struct Node));
    new_Node->val = value;
    new_Node->prev = NULL;
    return new_Node;
}

bool isEmpty(struct Node *S) {
    if(S == NULL) {
        return true;
    }
    return false;
}

struct Node* Push(int x, struct Node* S) {
    struct Node *new_top = CreateNode(x);
    new_top->prev = S;
    S = new_top;
    return S;
}

struct Node* Pop(struct Node* S) {
    if(isEmpty(S)) {
        printf("!");
        return NULL;
    }
    
    printf("%d\n", S->val);
    struct Node *temp = S;
    S = S->prev;
    free(temp);
    return S;
}

int Top(struct Node *S) {
    return S->val;
}

int Size(struct Node* head)
{
    if(head==NULL)
    {
        printf("%d\n",0);
    }
    else
    {
        int c=0;
       struct Node *temp = head;
       while(temp!=NULL)
       {
           c++;
           temp=temp->prev;
       }
      return c;
   }
}

int main() {
    struct Node *S = NULL;
    int n, x, op;
    scanf("%d", &n);
    while(n--) {
        scanf("%d", &op);
        
        if(op == 0) {
            scanf("%d", &x);
            S = Push(x, S);
        }
        else if(op == 1) {
            S = Pop(S);
        }
        else if(op == 2) {
            if(isEmpty(S)) {
                printf("!\n");
            }
            else {
                printf("%d\n", Top(S));
            }
        }
        else {
            printf("%d\n", Size(S));
        }
    }
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */    
    return 0;
}






// #include <stdio.h>
// #include <string.h>
// #include <math.h>
// #include <stdlib.h>

// struct node{
//     int val;
//     struct node *prev;
// };

// void push(struct node** head,int n)
// {
//     struct node* temp = (struct node*)malloc(sizeof(struct node));
//     temp->val=n;
//     temp->prev=*head;
//     *head=temp;
// }

// void pop(struct node** head)
// {
//     if(*head==NULL)
//     {
//         printf("!\n");
//     }
//     else
//     {
//         struct node* temp = *head;
//         int v= (*head)->val;
//         *head=(*head)->prev;
//         free(temp);
//         printf("%d\n",v);
//     }
// }

// int top(struct node* head)
// {
//     return head->val;
// }

// void size(struct node* head)
// {
//    if(head==NULL)
//    {
//       printf("%d\n",0);
//    }
//    else
//    {
//        int c=0;
//        struct node* temp = head;
//        while(temp!=NULL)
//        {
//            c++;
//            temp=temp->prev;
//        }
//        printf("%d\n",c);
//    }
// }

// int main() {

//     /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
//     struct node* head = NULL;
//     int n;
//     scanf("%d",&n);
//     while(n--)
//     {
//         int ch;
//         scanf("%d",&ch);
//         if(ch==0)
//         {
//             int k;
//             scanf("%d",&k);
//             push(&head,k);
//         }
//         else if(ch==1)
//         {
//             pop(&head);
//         }
//         else if(ch==2)
//         {
//             if(head==NULL)
//             {
//                 printf("!\n");
//             }
//             else
//             {
//                 printf("%d\n",top(head));    
//             }
//         }
//         else
//         {
//             size(head);
//         }
//     }
//     return 0;
// }