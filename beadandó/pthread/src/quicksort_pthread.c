#include <stdlib.h>
#include <pthread.h>
#include "utils.h"
#include "quicksort_pthread.h"

typedef struct {
    int *arr;
    int low;
    int high;
    int depth;
} QuickSortData;

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

static void *parallelQuickSort(void *arg) {
    QuickSortData *data = (QuickSortData*)arg;

    int *arr = data->arr;
    int low = data->low;
    int high = data->high;
    int depth = data->depth;

    if (low >= high) {
        return NULL;
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

    if (depth <= 0) {
        if (low < j) {
            sequentialQuickSort(arr, low, j);
        }

        if (i < high) {
            sequentialQuickSort(arr, i, high);
        }

        return NULL;
    }

    pthread_t leftThread;
    pthread_t rightThread;

    QuickSortData leftData;
    QuickSortData rightData;

    int hasLeftThread = 0;
    int hasRightThread = 0;

    if (low < j) {
        leftData.arr = arr;
        leftData.low = low;
        leftData.high = j;
        leftData.depth = depth - 1;

        if (pthread_create(&leftThread, NULL, parallelQuickSort, &leftData) == 0) {
            hasLeftThread = 1;
        } else {
            sequentialQuickSort(arr, low, j);
        }
    }

    if (i < high) {
        rightData.arr = arr;
        rightData.low = i;
        rightData.high = high;
        rightData.depth = depth - 1;

        if (pthread_create(&rightThread, NULL, parallelQuickSort, &rightData) == 0) {
            hasRightThread = 1;
        } else {
            sequentialQuickSort(arr, i, high);
        }
    }

    if (hasLeftThread) {
        pthread_join(leftThread, NULL);
    }

    if (hasRightThread) {
        pthread_join(rightThread, NULL);
    }

    return NULL;
}

void pthreadQuickSort(int arr[], int n, int threadCount) {
    if (threadCount <= 1) {
        sequentialQuickSort(arr, 0, n - 1);
        return;
    }

    QuickSortData data;
    data.arr = arr;
    data.low = 0;
    data.high = n - 1;
    data.depth = calculateDepth(threadCount);

    parallelQuickSort(&data);
}