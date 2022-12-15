#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define NUM_ITER 500
#define MAX_RANGE 1000000
int num_of_comparisons = 0;
double array[1000000];
double C[1000000];


// <------------------------------------------ Code for Merge Sort begins here ------------------------------->

void Copy(double *A, double *C, int start, int end) { //Copy the sorted subarray to the original array
    for(int i=start; i<=end; i++) {
        A[i] = C[i - start];
    }
}

// this function merges two sorted subarrays of A into a sorted array C.
void Merge(double *A, double *C, int start, int end, int mid) {
    int i = start, j = mid + 1, k = 0;
    while(i < mid + 1 && j < end + 1) {
        num_of_comparisons++;
        if(A[i] < A[j]) { 
            C[k] = A[i];
            k++;
            i++;
        }
        else {
            C[k] = A[j];
            k++;
            j++;
        }
    }
    while(i < mid + 1) {
        C[k] = A[i];
        k++;
        i++;
    }
    while(j < end + 1) {
        C[k] = A[j];
        k++;
        j++;
    }
}

// actual working code of Merge Sort
void MSort(double *A, int i, int j) {
    if(i<j) {
        int mid = (i+j)/2;
        MSort(A, i, mid);
        MSort(A, mid + 1, j);
        for(int k=0; k<= j-i; k++) {
            C[k] = 0;
        }
        Merge(A, C, i, j, mid);
        Copy(A, C, i, j);
    }
}

int main() {
    int i,j,k;
    clock_t start_t, end_t;
    int time_taken = 0;
    srand((unsigned)time(NULL));

    FILE *fptr;
    fptr = fopen("./MergeSortData.csv", "w");

    if(fptr == NULL) {
        printf("Error, file couldn't be opened");
        exit(1);
    }

    fprintf(fptr, "Size of array, Average Time Taken, Average Number of Comparisons\n");

    for(i=100; i<=1000000; i*=10) {
        num_of_comparisons = 0;

        for(j=0; j<NUM_ITER; j++) {
            for(k=0; k<i; k++) {
                array[k] = ((double) rand() / (double) RAND_MAX)*MAX_RANGE;
            }
            start_t = clock();
            MSort(array, 0, i - 1);
            end_t = clock();
            time_taken += ((double)(end_t - start_t)*1000000/(double)CLOCKS_PER_SEC);
        }
        fprintf(fptr, "%d, %d, %d\n", i, time_taken/NUM_ITER, num_of_comparisons/NUM_ITER);
        printf("Average Number of Comparisons = %d\n", num_of_comparisons/NUM_ITER);
        printf("Average Time Taken = %d\n\n", time_taken/NUM_ITER);
    }

    fclose(fptr);
    fptr = 0;

    return 0;
}