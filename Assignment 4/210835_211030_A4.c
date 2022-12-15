#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*The idea that will be used to maintain a Nearly Balanced Binary Search Tree (NB-BST) is that we will keep on inserting nodes to a NB-BST till it doesn't satisfy
the condition for being nearly balanced about a particular node (say v) in which case we will store the addresses of elements of the subtree which has its root as v
in an array. The addresses will be stored in the array in increasing order of the value of the nodes (by Inorder traversal of the BST). Then we will rebuild this into a
Perfectly Balanced BST (PB-BST) using the stored addresses and again continue insertions till it ceases to be a NB-BST (in which case this cycle repeats) */

//Typical node used in a BST with an extra attribute called size
typedef struct Node {
    int val; //to store the value for a given node
    int size; //stores the size of subtree attached to a given node
    struct Node *left, *right; //pointer to the left and the right children
} Node;

int idx = 0;
//idx is used to rebuild the tree. It is used in the indexing of the array that will be used to rebuild the tree
//it is equal to the index at which we store the address of the ith inorder element of a given subtree of the BST

#define MAX 10000000 //the maximum number that we will insert in the tree
#define STEP 50000 //we need to write the time taken for n insertions in a .csv file for all values n s.t. n%STEP == 0
#define LOWER_BOUND 100000
#define UPPER_BOUND 1000000
//we will write the time taken for each insertion in another .csv file for all numbers n s.t. LOWER_BOUND <= n <= UPPER_BOUND

double time_for_n_insertions[MAX] = {0}; //array that stores the time taken for n insertions for all values of n
double time_for_ith_insertion[MAX] = {0}; //array that stores the time taken for the ith insertion
Node *arr[MAX] = {NULL};

//function to create a new Node that has to be added to the BST and return the pointer to that node
Node* CreateNode(int value) {
    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->val = value;
    new_node->size = 1; //the size of the subtree linked to a new node is 1 since sizes of left and right subtree are 0
    new_node->left = new_node->right = NULL; //the children should always be initialized to NULL

    return new_node;
}

//check that a subtree is nearly balanced given the condition that Size(right) and Size(left) are < 3/4 * Size(subtree)
int CheckIfNearlyBalanced (Node *node) {
    int l_size = node->left != NULL ? node->left->size : 0;
    int r_size = node->right != NULL ? node->right->size : 0;
    //if left or right are NULL then the size of left or right resp. are 0
    int size = node->size;

    if(r_size <= 3*size/4 && l_size <= 3*size/4) {
        return 0; //we return 0 only if the tree is a NB-BST
    }
    return 1; //if we need to rebuild the tree, we will return 1
}

//store the addresses of the subtree in an array in increasing order of values stored in the node
void storeInorder(Node *root) {
    if(root == NULL) {
        return;
    }

    storeInorder(root->left);
    arr[idx] = root;
    idx++; //move to the next index so that we store the next inorder element of the BST in the adjacent index of the array contiguously
    storeInorder(root->right);
}

//function which actually rebuilds the tree recursively by dividing the array into 2 parts and taking the middle element as root for a given subtree
Node *rebuildTreeHelper(int start, int end) {
    
    if(start > end) { //base case
        return NULL;
    }
    //we want a PB-BST so we want the root to be the mean value (or as close to the mean value as possible) so as to have roughly equal nodes to the left and right
    int mid = (start + end)/2;
    Node *root = arr[mid];
    // root->size = start + end;
    //we recursively rebuild the tree
    root->left = rebuildTreeHelper(start, mid - 1);
    root->right = rebuildTreeHelper(mid + 1, end);
    
    int l_size = root->left != NULL ? root->left->size : 0;
    int r_size = root->right != NULL ? root->right->size : 0;
    root->size = l_size + r_size + 1;

    return root;
}

//function to initiate the rebuilding of the tree
Node* rebuildTree(Node *root) {

    idx = 0; //initializing idx to 0 so that the addresses are stored in order from the 1st element of the array arr
    storeInorder(root);
    
    return rebuildTreeHelper(0, root->size - 1);
}

//inserting a node into the tree recursively whilst simultaneously checking the condition that the tree is nearly balanced - if not we call the rebuildTree() function
Node* InsertNode(Node *root, int value) {

    if(root == NULL) { //base case
        root = CreateNode(value);
    } 
    else {
        if(CheckIfNearlyBalanced(root)) { //if size is non-zero then that implies that the subtree is not a NB-BST
            root = rebuildTree(root);
        }

        root->size++; //since we are inserting a new node to the subtree linked to root its size increases by 1
        if(value > root->val) {
            root->right = InsertNode(root->right, value);
        }
        else if(value < root->val) {
            root->left = InsertNode(root->left, value);
        }
    }
    return root;
}

//printing the BST in Preorder format (root followed by its entire left subtree followed by its entire right subtree using a recursive algorithm)
void printPreorder(Node *node) {

    if(node == NULL) {
        return;
    }
    printf("node val : %d & node size : %d \n", node->val, node->size);
    printPreorder(node->left);
    printPreorder(node->right);
}

//deleting the BST at the end of the program to prevent memory leak
void deleteBST(Node *root) {
    if(root == NULL) return;

    deleteBST(root->left);
    deleteBST(root->right);
    free(root);
}

int main() {
    Node *root = NULL;
    //since we have to store the time for 2 different kinds of processes, we will require 2 sets of time variables
    time_t start1_t, start2_t, end1_t, end2_t;
    double total1_t = 0, total2_t = 0;
    //total1_t will store the time taken for n insertions and total2_t will store the time taken for the ith insertion

    FILE *fptr1 = fopen("time-for-n-insertions.csv", "w+");
    FILE *fptr2 = fopen("time_for_ith_insertion.csv", "w+");

    if(fptr1 == NULL || fptr2 == NULL) {
        printf("Failed to open file\n");
        system("pause");
        return 1;
    }

    fprintf(fptr1, "Value of n, Time \n");
    fprintf(fptr2, "Value of i, Time \n");

    start1_t = clock();
    for(int i=1; i<=MAX; i++) {
        if(i>=LOWER_BOUND && i<=UPPER_BOUND)
        {
            start2_t=clock();
            root = InsertNode(root, i);
            end2_t=clock();
            total2_t = (double)(end2_t - start2_t) / CLOCKS_PER_SEC;
            fprintf(fptr2,"%d, %f\n",i,total2_t);
        }
        else
        {
            root = InsertNode(root, i);
        }
        if(i%STEP==0)
        {
            end1_t = clock();
            total1_t = (double)(end1_t - start1_t) / CLOCKS_PER_SEC;
            fprintf(fptr1," %d,%f\n", i,total1_t);
        }
    }

    
    fclose(fptr1);
    fclose(fptr2);

    fptr2 = fptr1 = 0;

    deleteBST(root);
    root = NULL;

    return 0;
}