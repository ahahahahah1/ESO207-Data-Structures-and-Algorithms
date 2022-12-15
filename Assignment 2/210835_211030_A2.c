#include <stdio.h>
#include <stdlib.h>
#define uint64 unsigned long long int
typedef struct Matrixnode
{
    int val;
    uint64 rowIndex, columnIndex;
    struct Matrixnode* nextDown, *nextRight;
}nd;
typedef struct matrix
{
    uint64 n;
    nd* FirstColumnHead, *FirstRowHead;
}mtx;

mtx* createMatrix(uint64 n)
{
    mtx* M=(mtx*)malloc(sizeof(mtx));
    M->n=n;
    M->FirstColumnHead=M->FirstRowHead=NULL;
    return M;
}
nd* createColumnHead(uint64 columnIndex)
{
    nd* newColumnHead=(nd*)malloc(sizeof(nd));
    newColumnHead->columnIndex=columnIndex;
    newColumnHead->nextRight=newColumnHead->nextDown=NULL;
    return newColumnHead;
}
nd* createRowHead(uint64 rowIndex)
{
    nd* newColumnHead=(nd*)malloc(sizeof(nd));
    newColumnHead->rowIndex=rowIndex;
    newColumnHead->nextRight=newColumnHead->nextDown=NULL;
    return newColumnHead;
}
nd* createNode(int val,uint64 rowIndex, uint64 columnIndex)
{
    nd* newNode=(nd*)malloc(sizeof(nd));
    newNode->rowIndex=rowIndex;
    newNode->columnIndex=columnIndex;
    newNode->val=val;
    newNode->nextRight=newNode->nextDown=NULL;
    return newNode;
}
int makeMatrix(mtx** M, int matrixIndex)
{
    int val;
    uint64 rowIndex,columnIndex;
    int matrixIndexTemp;
    int currRowIndex=0;
    nd* newRowHead, *currRowHead;
    nd* newColumnHead,*currColumnHead,*prevColumnHead;
    while(1)
    {
        scanf("%llu %llu %d\n",rowIndex,columnIndex,val);
        if(currRowIndex!=rowIndex)
        {
            currRowIndex=rowIndex;
            if((*M)->FirstColumnHead==NULL || (*M)->FirstColumnHead->columnIndex>columnIndex)
            {
                newColumnHead=createColumnHead(columnIndex);
                newColumnHead->nextRight=(*M)->FirstColumnHead;
                (*M)->FirstColumnHead=newColumnHead;
                newColumnHead->rowIndex=(uint64)newColumnHead;
                currColumnHead=newColumnHead;
            }
            newRowHead=createRowHead(rowIndex);
            if((*M)->FirstRowHead==NULL)
            {
                (*M)->FirstRowHead=newRowHead;
            }
            else
            {
                currRowHead->nextDown=newRowHead;
            }
            newRowHead->columnIndex=(uint64)newRowHead;
            currRowHead=newRowHead;
        }
        else
        {
            if((*M)->FirstColumnHead==NULL || (*M)->FirstColumnHead->columnIndex>columnIndex)
            {
                newColumnHead=createColumnHead(columnIndex);
                newColumnHead->nextRight=(*M)->FirstColumnHead;
                (*M)->FirstColumnHead=newColumnHead;
                newColumnHead->rowIndex=(uint64)newColumnHead;
                currColumnHead=newColumnHead;
            }
        }
        nd* newNode=createNode(val,rowIndex,columnIndex);
        ((nd*)currRowHead->columnIndex)->nextRight=newNode;
        currRowHead->columnIndex=(uint64)newNode;
        prevColumnHead = (*M)->FirstColumnHead;
        currColumnHead = prevColumnHead->nextRight;
        while(currColumnHead != NULL || currColumnHead->columnIndex < columnIndex)
        {
            prevColumnHead = currColumnHead;
            currColumnHead = currColumnHead->nextRight;
        }
        if(currColumnHead == NULL || currColumnHead->columnIndex > columnIndex) {
            newColumnHead = createColumnHead(columnIndex);
            newColumnHead->nextRight = currColumnHead;
            prevColumnHead->nextRight = newColumnHead;
            newColumnHead->rowIndex = (uint64)newColumnHead;
            currColumnHead = newColumnHead;
        }
        ((nd*)currColumnHead->rowIndex)->nextDown=newNode;
        currColumnHead->rowIndex=(uint64)newNode;
        scanf("%d",matrixIndexTemp);
        if(matrixIndex!=matrixIndexTemp)
        {
            return matrixIndexTemp;
        }
        
    }
}
void printColumn(nd* columnHead)
{
    printf("%llu\n",columnHead->columnIndex);
    nd* rowPointer=columnHead->nextDown;
    while(rowPointer!=NULL)
    {
        printf("%llu %d\n",rowPointer->rowIndex,rowPointer->val);
        rowPointer=rowPointer->nextDown;
    }
}
void printRow(nd* rowHead)
{
    printf("%llu\n",rowHead->rowIndex);
    nd* columnPointer=rowHead->nextRight;
    while(columnPointer!=NULL)
    {
        printf("%llu %d\n",columnPointer->columnIndex,columnPointer->val);
        columnPointer=columnPointer->nextRight;
    }
}
void printMatrix(mtx* M)
{
    if(M->FirstColumnHead==NULL || M->FirstRowHead==NULL)
        printf("NULL MATRIX\n");
    else
    {
        nd* rowHead=M->FirstRowHead;
        while(rowHead!=NULL)
        {
            nd* columnPointer=rowHead->nextRight;
            while(columnPointer!=NULL)
            {
                printf("%llu %llu %d\n",columnPointer->rowIndex, columnPointer->columnIndex, columnPointer->val);
                columnPointer=columnPointer->nextRight;
            }
            rowHead=rowHead->nextDown;
        }
    }
}
int getValue(mtx* M, uint64 rowIndex, uint64 columnIndex)
{
    if(rowIndex > M->n || columnIndex > M->n)
        return -1;
    nd* rowHead=M->FirstRowHead;
    while(rowHead!=NULL && rowHead->rowIndex < rowIndex)
    {
        rowHead= rowHead->nextDown;
    }
    if(rowHead != NULL && rowHead->rowIndex == rowIndex) {
        nd *columnPointer=rowHead->nextRight;
        while(columnPointer!=NULL && columnPointer->columnIndex<columnIndex)
        {
            columnPointer=columnPointer->nextRight;
        }
        if(columnPointer!=NULL && columnPointer->columnIndex==columnIndex)
        {
            return columnPointer->val;
        }
        return 0;
    }
    return 0;
}
int dotProduct(nd* rowHead, nd* columnHead)
{
    int val=0;
    nd* rowPointer=rowHead->nextRight;
    nd* columnPointer=columnHead->nextDown;
    while(rowPointer!=NULL || columnPointer!=NULL)
    {
        if(rowPointer->columnIndex == columnPointer->rowIndex)
        {
            val+=(rowPointer->val)*(columnPointer->val);
        }
        else if(rowPointer->columnIndex > columnPointer->rowIndex)
        {
            columnPointer=columnPointer->nextDown;
        }
        else{
            rowPointer=rowPointer->nextRight;
        }
    } 
    return val;   
}
mtx* multiplyMatrix(mtx *A, mtx *B, uint64 n)
{
    mtx* C= createMatrix(n);
    uint64 rowIndex, columnIndex;
    int val;
    int currRowIndex=0;
    nd *newRowHead, *currRowHead;
    nd* newColumnHead,*currColumnHead,*prevColumnHead;

    nd*rowHead, *columnHead;
    rowHead = A->FirstRowHead;

    while(rowHead != NULL) {
        columnHead = B->FirstColumnHead;
        rowIndex = rowHead->rowIndex;
        while(columnHead != NULL) {
            columnIndex = columnHead->columnIndex;
            val = dotProduct(rowHead, columnHead);
            if(val != 0) {
                if(currRowIndex!=rowIndex)
                {
                    currRowIndex=rowIndex;
                    if((*C)->FirstColumnHead==NULL || (*C)->FirstColumnHead->columnIndex>columnIndex)
                    {
                        newColumnHead=createColumnHead(columnIndex);
                        newColumnHead->nextRight=(*C)->FirstColumnHead;
                        (*C)->FirstColumnHead=newColumnHead;
                        newColumnHead->rowIndex=(uint64)newColumnHead;
                        currColumnHead=newColumnHead;
                    }
                    newRowHead=createRowHead(rowIndex);
                    if((*C)->FirstRowHead==NULL)
                    {
                        (*C)->FirstRowHead=newRowHead;
                    }
                    else
                    {
                        currRowHead->nextDown=newRowHead;
                    }
                    newRowHead->columnIndex=(uint64)newRowHead;
                    currRowHead=newRowHead;
                }
                else
                {
                    if((*C)->FirstColumnHead==NULL || (*C)->FirstColumnHead->columnIndex>columnIndex)
                    {
                        newColumnHead=createColumnHead(columnIndex);
                        newColumnHead->nextRight=(*C)->FirstColumnHead;
                        (*C)->FirstColumnHead=newColumnHead;
                        newColumnHead->rowIndex=(uint64)newColumnHead;
                        currColumnHead=newColumnHead;
                    }
                }
                nd* newNode=createNode(val,rowIndex,columnIndex);
                ((nd*)currRowHead->columnIndex)->nextRight=newNode;
                currRowHead->columnIndex=(uint64)newNode;
                prevColumnHead = (*M)->FirstColumnHead;
                currColumnHead = prevColumnHead->nextRight;
                while(currColumnHead != NULL || currColumnHead->columnIndex < columnIndex)
                {
                    prevColumnHead = currColumnHead;
                    currColumnHead = currColumnHead->nextRight;
                }
                if(currColumnHead == NULL || currColumnHead->columnIndex > columnIndex) {
                    newColumnHead = createColumnHead(columnIndex);
                    newColumnHead->nextRight = currColumnHead;
                    prevColumnHead->nextRight = newColumnHead;
                    newColumnHead->rowIndex = (uint64)newColumnHead;
                    currColumnHead = newColumnHead;
                }
                ((nd*)currColumnHead->rowIndex)->nextDown=newNode;
                currColumnHead->rowIndex=(uint64)newNode;
            }
        }
    }

}
void main()
{
   uint64 n;
   scanf("%llu",n);
   mtx *A=createMatrix(n);
   mtx *B=createMatrix(n);
   int matrixIndex;
   scanf("%d",matrixIndex);
   if(matrixIndex==1)
   {
     matrixIndex=makeMatrix(&A,matrixIndex);
   }
   else
   {
     matrixIndex=makeMatrix(&B,matrixIndex);
   }
   mtx *C= createMatrix(n);
   C=multiplyMatrix(A,B,n);
   printMatrix(C);
   return 0;
}