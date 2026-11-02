#include <omp.h>

#include "quicksort_openmp.h"

#define QUICK_THRESHOLD 10000

static void sequentialQuickSort(int arr[], int low, int high) {
    int i = low;
    int j = high;
    int pivot = arr[low + (high - low) / 2];

    while (i <= j) {
        while (arr[i] < pivot) {
            i++;
        }

        while (arr[j] > pivot) {
            j--;
        }

        if (i <= j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;

            i++;
            j--;
        }
    }

    if (low < j) {
        sequentialQuickSort(arr, low, j);
    }

    if (i < high) {
        sequentialQuickSort(arr, i, high);
    }
}

static void quickSortTask(int arr[], int low, int high) {
    if (low >= high) {
        return;
    }

    if (high - low < QUICK_THRESHOLD) {
        sequentialQuickSort(arr, low, high);
        return;
    }

    int i = low;
    int j = high;
    int pivot = arr[low + (high - low) / 2];

    while (i <= j) {
        while (arr[i] < pivot) {
            i++;
        }

        while (arr[j] > pivot) {
            j--;
        }

        if (i <= j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;

            i++;
            j--;
        }
    }

    #pragma omp task shared(arr)
    {
        if (low < j) {
            quickSortTask(arr, low, j);
        }
    }

    #pragma omp task shared(arr)
    {
        if (i < high) {
            quickSortTask(arr, i, high);
        }
    }

    #pragma omp taskwait
}

void openmpQuickSort(int arr[], int n, int threadCount) {
    omp_set_num_threads(threadCount);

    #pragma omp parallel
    {
        #pragma omp single
        {
            quickSortTask(arr, 0, n - 1);
        }
    }
}