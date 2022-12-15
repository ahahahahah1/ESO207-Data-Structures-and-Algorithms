#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define NUM_ITER 500
#define MAX_RANGE 1000000
int num_of_comparisons = 0;
double array[1000000];

void Swap (double *A, int id1, int id2) {
    int temp = 0;
    temp = A[id1];
    A[id1] = A[id2];
    A[id2] = temp;
}

int Partition(double *A, int start, int end) {
    int left_idx = start + 1;
    int right_idx = end;
    // int temp = 0;

    while(left_idx < right_idx) {
        num_of_comparisons++;
        if(A[left_idx] < A[start]) {
            left_idx++;
        } else { 
            // temp = A[left_idx];
            // A[left_idx] = A[right_idx];
            // A[right_idx] = temp;
            // right_idx--;
            Swap(A, left_idx, right_idx);
            right_idx--;
        }
    }

    if(A[left_idx] > A[start]) {
        // temp = A[left_idx - 1];
        // A[left_idx - 1] = A[start];
        // A[start] = temp;
        Swap(A, start, left_idx - 1);
        num_of_comparisons++;
        return left_idx - 1;
    } else {
        // temp = A[left_idx];
        // A[left_idx] = A[start];
        // A[start] = temp;
        Swap(A, start, left_idx);
        num_of_comparisons++;
        return left_idx;
    }
}

void QuickSort(double *A, int l, int r) {
    if(l < r) {
        int i = Partition(A, l, r);
        QuickSort(A, l, i - 1);
        QuickSort(A, i + 1, r);
    }
}

int main() {
    int i,j,k;
    clock_t start_t, end_t;
    int time_taken[NUM_ITER] = {0};
    int average_time = 0;
    srand((unsigned)time(NULL));

    FILE *fptr;
    fptr = fopen("./QuickSortData.csv", "w");

    if(fptr == NULL) {
        printf("Error, file couldn't be opened");
        exit(1);
    }

    fprintf(fptr, "Size of array, Average Time Taken, Average Number of Comparisons, Number of Cases when time exceeds average by\n");
    fprintf(fptr, ",,, 5%%, 10%%, 20%%, 30%%, 50%%, 100%%\n");

    for(i=100; i<=1000000; i*=10) {
        num_of_comparisons = 0;
        int num_cases[6] = {0};
        printf("Iteration Number %d\n", i);

        for(j=0; j<NUM_ITER; j++) {
            for(k=0; k<i; k++) {
                array[k] = ((double) rand() / (double) RAND_MAX)*MAX_RANGE;
            }
            start_t = clock();
            QuickSort(array, 0, i - 1);
            end_t = clock();
            time_taken[j] = ((double)(end_t - start_t)*1000000/(double)CLOCKS_PER_SEC);
        }
        for(j=0; j<NUM_ITER; j++) {
            average_time += time_taken[j];
        }
        average_time /= NUM_ITER;
        for(j=0; j<NUM_ITER; j++) {
            if(time_taken[j] >= 1.05 * average_time) {
                num_cases[0]++;
            }
            if(time_taken[j] >= 1.10 * average_time) {
                num_cases[1]++;
            }
            if(time_taken[j] >= 1.20 * average_time) {
                num_cases[2]++;
            }
            if(time_taken[j] >= 1.30 * average_time) {
                num_cases[3]++;
            }
            if(time_taken[j] >= 1.50 * average_time) {
                num_cases[4]++;
            }
            if(time_taken[j] >= 2.0 * average_time) {
                num_cases[5]++;
            }
        }
        fprintf(fptr, "%d, %d, %d,", i, average_time, num_of_comparisons/NUM_ITER);

        for(k=0; k<6; k++) {
            fprintf(fptr, "%d,", num_cases[k]);
        }
        fprintf(fptr, "\n");
        printf("Average Number of Comparisons = %d\n", num_of_comparisons/NUM_ITER);
        printf("Average Time Taken = %d\n\n", average_time);
    }

    fclose(fptr);
    fptr = 0;

    return 0;
}