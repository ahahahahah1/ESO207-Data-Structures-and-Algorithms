#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define NUM_ITER 500 //number of iterations to be performed per algorithm to compute average working
#define MAX_RANGE 1000000 //we will use rand() function to generate a random number between 0 and MAX_RANGE
long long int num_comparisons_QSort = 0, num_comparisons_MSort = 0; //to count the number of comparisons in each algorithm
double MSortArray[1000000]; //will store the elements that we will sort using Merge Sort
double QSortArray[1000000]; //will store the elements that we will sort using Quick Sort
double ImprovedMSortArray[1000000]; //will store the elements that we will sort using Improved Merge Sort
double C[1000000]; //C will be the array that we will use to copy and store sorted subarrays (while using merge sort)


// <----------------------------------- Code for Merge Sort Starts here ------------------------------>

void Copy(double *destination, double *source, int start, int end) { //Copy the sorted subarray to the original array
    for(int i=start; i<=end; i++) {
        destination[i] = source[i - start];
    }
}

// this function merges two sorted subarrays of A into another array C
void Merge(double *A, double *C, int start, int end, int mid) {
    int i = start, j = mid + 1, k = 0;
    while(i < mid + 1 && j < end + 1) {
        num_comparisons_MSort++;
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

void MSort(double *A, int i, int j) { //actual working function for Merge Sort
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

// <--------------------------------- Code for Merge Sort Ends Here ------------------------------------->


// <--------------------------------- Code for Quick Sort Starts Here ----------------------------------->
void Swap (double *A, int id1, int id2) { //swaps the elements of the array present at indices id1 and id2
    int temp = 0;
    temp = A[id1];
    A[id1] = A[id2];
    A[id2] = temp;
}

int Partition(double *A, int start, int end) {
    int left_idx = start + 1;
    int right_idx = end;

    while(left_idx < right_idx) {
        num_comparisons_QSort++;
        if(A[left_idx] < A[start]) {
            left_idx++;
        } else { 
            Swap(A, left_idx, right_idx);
            right_idx--;
        }
    }

    num_comparisons_QSort++;
    if(A[left_idx] > A[start]) {
        Swap(A, start, left_idx - 1);
        return left_idx - 1;
    } else {
        Swap(A, start, left_idx);
        return left_idx;
    }
}

void QSort(double *A, int l, int r) {
    if(l < r) {
        int i = Partition(A, l, r);
        QSort(A, l, i - 1);
        QSort(A, i + 1, r);
    }
}

//<------------------------------------ Code for Quick Sort Ends Here --------------------------->

//<----------------------------------- Functions involved in Improved Merge Sort --------------------------->

/*when implementing merge sort, while calling the function recursively, we copy sorted subarrays into the
original array too many times.
Instead, I will maintain 2 arrays and store pointers to those arrays in a Node. Then we alternately store sorted
subarrays in A1 and A2 and we can save the time that we spend to copy the elements of A1 into A2 every time.
 */

typedef struct Node {
    double *A1, *A2;
    int flag_for_destination_array; //this will help us find out in which array the final sorted array is
} Node;


void ImprovedMerge(double *source, double *destination, int start, int end, int mid) {
    int i = start, j = mid + 1, k = start;

    while(i < mid + 1 && j < end + 1) {
        if(source[i] < source[j]) {
            destination[k] = source[i];
            k++;
            i++;
        }
        else {
            destination[k] = source[j];
            k++;
            j++;
        }
    }
    while(i < mid + 1) {
        destination[k] = source[i];
        i++;
        k++;
    }
    while(j < end + 1) {
        destination[k] = source[j];
        j++;
        k++;
    }
}

void ImprovedMSort(Node *ptr, int i, int j, int flag) {
    if(i < j) {
        int mid = (i+j)/2;
        ImprovedMSort(ptr, i, mid, (flag + 1) % 2);
        ImprovedMSort(ptr, mid + 1, j, (flag + 1) % 2);

        if(flag) {
            ImprovedMerge(ptr->A1, ptr->A2, i, j, mid);
            ptr->flag_for_destination_array = 1;
        }
        else {
            ImprovedMerge(ptr->A2, ptr->A1, i, j, mid);
            ptr->flag_for_destination_array = 0;
        }   
    }
}

// <-------------------------------- Code for Improved Merge Sort Ends Here -------------------------------->

int main() {
    int i,j,k;
    clock_t start_t, end_t;
    int time_taken_quick[NUM_ITER] = {0}, time_taken_merge[NUM_ITER] = {0}, time_taken_improvedMerge[NUM_ITER] = {0};
    int avg_time_Quick = 0, avg_time_Merge = 0, avg_time_ImprovedMerge = 0;
    int num_cases_merge_outperformed = 0, num_cases_improvedMerge_outperformed = 0;
    Node *ptr = (Node *) malloc(sizeof(Node));
    srand((unsigned)time(NULL));

    FILE *fptr1, *fptr2, *fptr3;
    //fptr1 will be the file pointer to the file containing data about Quick Sort
    //fptr2 will be the file pointer to the file containing data that compares Quick Sort and Merge Sort
    //fptr3 will be the file pointer to the file containing data that compares Quick Sort and Improved Merge Sort
    fptr1 = fopen("./QuickSortData.csv", "w");
    fptr2 = fopen("./QSort_vs_MSort.csv", "w");
    fptr3 = fopen("./QSort_vs_ImprovedMSort.csv", "w");

    if(fptr1 == NULL || fptr2 == NULL || fptr3 == NULL) {
        printf("Error, file couldn't be opened");
        exit(1);
    }

    fprintf(fptr1, "Size of array,Average Running Time,Number of cases where time exceeds average by 5%%,Number of cases where time exceeds average by 10%%,Number of cases where time exceeds average by 20%%,Number of cases where time exceeds average by 30%%,Number of cases where time exceeds average by 50%%,Number of cases where time exceeds average by 100%%\n");
    fprintf(fptr2, "Size of array,Average Number of Comparisons in Quick Sort,Average Running time of Quick Sort,Average Number of Comparisons in Merge Sort,Average Running time in Merge Sort,Number of times Merge Sort outperformed Quick Sort\n");
    fprintf(fptr3, "Size of array,Average running time of Quick Sort,Average running time of Improved Merge Sort,Number of times Improved Merge Sort outperformed Quick Sort\n");


    for(i=100; i <= 1000000; i*=10) {
        num_comparisons_QSort = num_comparisons_MSort = 0;
        avg_time_ImprovedMerge = avg_time_Merge = avg_time_Quick = 0;
        num_cases_merge_outperformed = num_cases_improvedMerge_outperformed = 0;

        for(j=0; j<NUM_ITER; j++) {
            for(k=0; k<i; k++) {
                QSortArray[k] = ((double) rand() / (double) RAND_MAX)*MAX_RANGE;
                MSortArray[k] = QSortArray[k];
                ImprovedMSortArray[k] = QSortArray[k];
            }

            start_t = clock();
            QSort(QSortArray, 0, i - 1);
            end_t = clock();
            time_taken_quick[j] = ((double)(end_t - start_t)*1000000/(double)CLOCKS_PER_SEC);
                
            start_t = clock();
            MSort(MSortArray, 0, i - 1);
            end_t = clock();
            time_taken_merge[j] = ((double)(end_t - start_t)*1000000/(double)CLOCKS_PER_SEC);
            
            start_t = clock();
            ptr->A1 = ImprovedMSortArray;
            Copy(C, ImprovedMSortArray, 0, i - 1);
            ptr->A2 = C;
            ImprovedMSort(ptr, 0, i - 1, 0);
            if(ptr->flag_for_destination_array) {
                for(k=0; k<i; k++) {
                    ImprovedMSortArray[k] = C[k];
                }
            }
            end_t = clock();
            time_taken_improvedMerge[j] = ((double)(end_t - start_t)*1000000/(double)CLOCKS_PER_SEC);
            avg_time_Quick += time_taken_quick[j];
            avg_time_Merge += time_taken_merge[j];
            avg_time_ImprovedMerge += time_taken_improvedMerge[j];

            if(time_taken_quick[j] > time_taken_merge[j]) {
                num_cases_merge_outperformed++;
            }
            if(time_taken_quick[j] > time_taken_improvedMerge[j]) {
                num_cases_improvedMerge_outperformed++;
            }
        }
        avg_time_Quick /= NUM_ITER;
        avg_time_Merge /= NUM_ITER;
        avg_time_ImprovedMerge /= NUM_ITER;

        printf("Comparative analysis for %d array size done\n", i);

        fprintf(fptr2, "%d, %d, %d, %d, %d, %d\n", i, num_comparisons_QSort/NUM_ITER, avg_time_Quick, num_comparisons_MSort/NUM_ITER, avg_time_Merge, num_cases_merge_outperformed);
        fprintf(fptr3, "%d, %d, %d, %d\n", i, avg_time_Quick, avg_time_ImprovedMerge, num_cases_improvedMerge_outperformed);
        fprintf(fptr1, "%d, %d\n", i, avg_time_Quick);
    }

    for(i = 100000; i<1000000; i+= 200000) {
        int time_above_avg[6] = {0};
        avg_time_Quick = 0;
        for(j = 0; j < NUM_ITER; j++) {
            for(k=0; k<i; k++) {
                QSortArray[k] = ((double) rand() / (double) RAND_MAX)*MAX_RANGE;
            }

            start_t = clock();
            QSort(QSortArray, 0, i - 1);
            end_t = clock();
            time_taken_quick[j] = ((double)(end_t - start_t)*1000000/(double)CLOCKS_PER_SEC);
            avg_time_Quick += time_taken_quick[j];
        }
        avg_time_Quick /= NUM_ITER;

        for(j=0; j<NUM_ITER; j++) {
            if(time_taken_quick[j] >= 1.05 * avg_time_Quick) {
                time_above_avg[0]++;
            }
            if(time_taken_quick[j] >= 1.10 * avg_time_Quick) {
                time_above_avg[1]++;
            }
            if(time_taken_quick[j] >= 1.20 * avg_time_Quick) {
                time_above_avg[2]++;
            }
            if(time_taken_quick[j] >= 1.30 * avg_time_Quick) {
                time_above_avg[3]++;
            }
            if(time_taken_quick[j] >= 1.50 * avg_time_Quick) {
                time_above_avg[4]++;
            }
            if(time_taken_quick[j] >= 2.0 * avg_time_Quick) {
                time_above_avg[5]++;
            }
        }
        printf("Analysis of Quick Sort done for %d array size\n", i);
        fprintf(fptr1, "%d, %d,", i, avg_time_Quick);

        for(k=0; k<6; k++) {
            fprintf(fptr1, "%d", time_above_avg[k]);
            if(k < 5) {
                fprintf(fptr1, ",");
            }
        }
        fprintf(fptr1, "\n");
    }

    fclose(fptr1);
    fclose(fptr2);
    fclose(fptr3);

    fptr1 = fptr2 = fptr3 = 0;

    return 0;
}