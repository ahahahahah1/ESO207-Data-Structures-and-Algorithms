#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX 10000000
#define STEP 50000
#define LOWER_BOUND 100000
#define UPPER_BOUND 1000000
double time_for_n_insertions[MAX] = {0};
double time_for_ith_insertion[MAX] = {0};

typedef struct Node {
    int val;
    char color; //'r' for red and 'b' for black
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

Node *root = NULL;

Node* CreateNode(int value) {
    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->val = value;
    new_node->color = 'r';
    new_node->parent = NULL;
    new_node->left = new_node->right = NULL;

    return new_node;
}

void SwapColor(Node *ptr) {
    ptr->color = (ptr->color == 'b') ? 'r' : 'b';
}

void RightRotate(Node *ptr) {
    Node *parent_node = ptr->parent;
    Node *left_child = ptr->left;
    Node *subtree = left_child->right;
    
    if(ptr != root) { //since every element except root has a non NULL parent, whose child attributes need to be changed
        if(parent_node->right == ptr) {
            parent_node->right = left_child;
        } else {
            parent_node->left = left_child;
        }
    }

    left_child->parent = parent_node;
    ptr->left = subtree;
    ptr->parent = left_child;
    left_child->right = ptr;
    if(subtree != NULL)
        subtree->parent = ptr;

    if(root == ptr) 
        root = left_child;

    // printf("address of 15 child in rotate: %d\n", ptr->left);
}

void LeftRotate(Node *ptr) {
    Node *parent_node = ptr->parent;
    Node *right_child = ptr->right;
    Node *subtree = right_child->left;

    if(ptr != root) { //since every element except root has a non NULL parent, whose child attributes need to be changed
        if(parent_node->right == ptr) {
            parent_node->right = right_child;
        } else {
            parent_node->left = right_child;
        }
    }

    right_child->parent = parent_node;
    ptr->right = subtree;
    ptr->parent = right_child;
    right_child->left = ptr;
    if(subtree != NULL)
        subtree->parent = ptr;

    if(root == ptr) 
        root = right_child;
}

void fixTree(Node *ptr) {
    // printf("Called for key = %d\n", ptr->val);
    //why was if(ptr== root && ptr->color = 'b') not working?
    if(ptr == root) {
        ptr->color = 'b';
        return;
    }
    if(ptr->parent->color == 'r') {
        Node *parent_node = ptr->parent;
        Node *grandparent_node = parent_node->parent;
        Node *uncle_node;

        //locating the uncle node
        if(parent_node == grandparent_node->left) {
            uncle_node = grandparent_node->right;
        } else {
            uncle_node = grandparent_node->left;
        }

        if(uncle_node == NULL || uncle_node->color == 'b') {
            //handling the case when parent is 'r' and uncle is 'b' (NULL node is also considered to be a black leaf node)
            if(parent_node == grandparent_node->left) {
                if(ptr == parent_node->right) {
                    LeftRotate(parent_node);
                    Node *temp = ptr;
                    ptr = parent_node;
                    parent_node = temp;
                }
                RightRotate(grandparent_node);
            }
            else {
                if(ptr == parent_node->left) {
                    RightRotate(parent_node);
                    Node *temp = ptr;
                    ptr = parent_node;
                    parent_node = temp;
                }
                LeftRotate(grandparent_node);
            }
            parent_node->color = 'b';
            grandparent_node->color = 'r';
        }
        
        //handling the case when both parent and uncle are red
        else {
            parent_node->color = uncle_node->color = 'b';
            grandparent_node->color = 'r';
            fixTree(grandparent_node);
        }
    }
}

void InsertNode(int value) {
    // printf("Insert Node called for key : %d\n", value);
    Node *new_node = CreateNode(value);

    if(root == NULL) {
        new_node->color = 'b';
        root = new_node;
        return;
    }

    Node *temp;
    Node *parent_node = root;

    if(new_node->val > root->val) {
        temp = root->right;
    } else {
        temp = root->left;
    }

    while(temp != NULL) {
        parent_node = temp;
        if(new_node->val > temp->val) {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }

    new_node->parent = parent_node;
    if(new_node->val > parent_node->val) {
        parent_node->right = new_node;
    } else {
        parent_node->left = new_node;
    }

    fixTree(new_node);
}

void deleteBST(Node *root) {
    if(root == NULL) return;

    deleteBST(root->left);
    deleteBST(root->right);
    printf("Deleting Node %d\n", root->val);
    free(root);
}

Node *Search(int key) {
    Node *temp = root;

    while(temp->val != key && temp != NULL) {
        if(temp->val > key) {
            temp = temp->left;
        } else {
            temp = temp->right;
        }
    }
    return temp;
} 

void PrintInorder(Node *root) {
    if(root == NULL) return;

    PrintInorder(root->left);
    printf("%d ", root->val);
    PrintInorder(root->right);
}

void PrintPreorder(Node *root) {
    if(root == NULL) return;

    printf("%d ", root->val);
    PrintPreorder(root->left);
    PrintPreorder(root->right);
}

int main() {


    //<-------------- Attempt to calculate and store time -------------->
    // clock_t start_t, end_t;
    // double total_t;
    // int i = 0, j = 0;

    // // FILE *fptr1 = fopen("time-for-n-insertions.csv", "w");
    // // FILE *fptr2 = fopen("time_for_ith_insertion.csv", "w");

    // // if(fptr1 == NULL || fptr2 == NULL) {
    // //     printf("The file couldn't be opened\n");
    // //     system("pause");
    // //     return 1;
    // // }

    // for(i=1; i<=20000; i++) {
    //     start_t = clock();
    //     for(j=1; j<=i; j++) {
    //         InsertNode(j);
    //     }
    //     end_t = clock();
    //     total_t = (end_t - start_t)/CLOCKS_PER_SEC;

    //     deleteBST(root);
    //     root = NULL;

    //     time_for_n_insertions[i-1] = total_t;
    //     if(i == 1) {
    //         time_for_ith_insertion[i-1] = total_t;
    //     } else {
    //         time_for_ith_insertion[i-1] = total_t - time_for_ith_insertion[i-2];
    //     }
    //     if(i>=10000 && i<=10010) {
    //         printf("%f\n", total_t);
    //     }

        // if(i%STEP == 0) {
        //     fprintf(fptr1, "%d, %f\n", i, sum_array[i]);
        // }
        // if(i<= UPPER_BOUND && i>= LOWER_BOUND) {
        //     fprintf(fptr2, "%d, %f\n", i, time_array[i]);
        // }
    // }

    // fclose(fptr1);
    // fclose(fptr2);
    // fptr1 = 0;
    // fptr2 = 0;

    //<---------------- Attempt to calculate and store time ends here  -------------->


    
    for(int i=1; i<=1000; i++) {
        InsertNode(i);
    }
    PrintPreorder(root);
    // printf("\n%d\n", root->val);
    // deleteBST(root);
    // printf("\n%d", root->right->val);
    return 0;

    //<---------------- Can ignore everything below this ------------->
    
    //testing rotation about root
    // InsertNode(1);
    // InsertNode(2);
    // InsertNode(3);
    // printf("Value of root : %d\n", root->val);

    //testing the program
    // int arr[] = {10, 8, 15, 4, 9, 12, 20, 1, 3, 11, 14, 25, 13, 27};

    // for(int i= 0; i< sizeof(arr)/sizeof(int); i++) {
    //     InsertNode(arr[i]);
    // }
    

    //testing case 3 for insertion in color imbalance
    // Node *req = Search(3);
    // printf("Right Child of 3 : %d\n", req->right->val);


    // testing swapcolor function
    // PrintInorder(root);
    // Node *req = Search(15);
    // SwapColor(req);
    // req = Search(8);
    // SwapColor(req);
    // printf("\n");
    // PrintInorder(root);

    // testing rotate functions
    // printf("address of 10 child in main : %d\n", root->right);
    // printf("address of 12 in main : %d\n", req->left);
    // printf("address of 12 child in main: %d\n", req->left->right);
    // printf("address of 15 in main : %d\n", req);
    // printf("address of 15 child in main : %d\n\n", req->left);
    // printf("%d %c\n", req->val, req->color);

    // printf("parent of 15 before right rotate : %d\n", req->parent->val);
    // RightRotate(req);
    // printf("parent of 15 after right rotate : %d\n", req->parent->val);
    // // printf("address of 10 child after rotate : %d\n", root->right);
    // // printf("address of 15 child after rotate : %d\n", req->left);
    // LeftRotate(req->parent);
    // printf("parent of 15 after left rotate : %d\n", req->parent->val);
    // printf("%c\n", root->left->color);
    // PrintInorder(root);
    // printf("\n");
    // PrintPreorder(root);
}


//109 111 120 116 114 113 115 118 117 
//119 124 122 121 123 126 125 127 192 160 144
//109 111 120 116 114 113 115 118 117 119 124 122 121 123 126 125 127 192 160 144