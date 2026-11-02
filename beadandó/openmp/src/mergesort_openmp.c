#include <stdlib.h>
#include <omp.h>

#include "mergesort_openmp.h"

#define MERGE_THRESHOLD 10000

static void merge(int arr[], int temp[], int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (int x = left; x <= right; x++) {
        arr[x] = temp[x];
    }
}

static void sequentialMergeSort(int arr[], int temp[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        sequentialMergeSort(arr, temp, left, mid);
        sequentialMergeSort(arr, temp, mid + 1, right);

        merge(arr, temp, left, mid, right);
    }
}

static void mergeSortTask(int arr[], int temp[], int left, int right) {
    if (left >= right) {
        return;
    }

    if (right - left < MERGE_THRESHOLD) {
        sequentialMergeSort(arr, temp, left, right);
        return;
    }

    int mid = left + (right - left) / 2;

    #pragma omp task shared(arr, temp)
    {
        mergeSortTask(arr, temp, left, mid);
    }

    #pragma omp task shared(arr, temp)
    {
        mergeSortTask(arr, temp, mid + 1, right);
    }

    #pragma omp taskwait

    merge(arr, temp, left, mid, right);
}

void openmpMergeSort(int arr[], int n, int threadCount) {
    int *temp = (int*)malloc(n * sizeof(int));

    if (temp == NULL) {
        return;
    }

    omp_set_num_threads(threadCount);

    #pragma omp parallel
    {
        #pragma omp single
        {
            mergeSortTask(arr, temp, 0, n - 1);
        }
    }

    free(temp);
}