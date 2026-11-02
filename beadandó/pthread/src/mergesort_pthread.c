#include <stdlib.h>
#include <pthread.h>

#include "mergesort_pthread.h"
#include "utils.h"

typedef struct {
    int *arr;
    int *temp;
    int left;
    int right;
    int depth;
} MergeSortData;

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

static void *parallelMergeSort(void *arg) {
    MergeSortData *data = (MergeSortData*)arg;

    int *arr = data->arr;
    int *temp = data->temp;
    int left = data->left;
    int right = data->right;
    int depth = data->depth;

    if (left >= right) {
        return NULL;
    }

    if (depth <= 0) {
        sequentialMergeSort(arr, temp, left, right);
        return NULL;
    }

    int mid = left + (right - left) / 2;

    pthread_t leftThread;
    pthread_t rightThread;

    MergeSortData leftData;
    MergeSortData rightData;

    int hasLeftThread = 0;
    int hasRightThread = 0;

    leftData.arr = arr;
    leftData.temp = temp;
    leftData.left = left;
    leftData.right = mid;
    leftData.depth = depth - 1;

    rightData.arr = arr;
    rightData.temp = temp;
    rightData.left = mid + 1;
    rightData.right = right;
    rightData.depth = depth - 1;

    if (pthread_create(&leftThread, NULL, parallelMergeSort, &leftData) == 0) {
        hasLeftThread = 1;
    } else {
        sequentialMergeSort(arr, temp, left, mid);
    }

    if (pthread_create(&rightThread, NULL, parallelMergeSort, &rightData) == 0) {
        hasRightThread = 1;
    } else {
        sequentialMergeSort(arr, temp, mid + 1, right);
    }

    if (hasLeftThread) {
        pthread_join(leftThread, NULL);
    }

    if (hasRightThread) {
        pthread_join(rightThread, NULL);
    }

    merge(arr, temp, left, mid, right);

    return NULL;
}

void pthreadMergeSort(int arr[], int n, int threadCount) {
    int *temp = (int*)malloc(n * sizeof(int));

    if (temp == NULL) {
        return;
    }

    if (threadCount <= 1) {
        sequentialMergeSort(arr, temp, 0, n - 1);
        free(temp);
        return;
    }

    MergeSortData data;
    data.arr = arr;
    data.temp = temp;
    data.left = 0;
    data.right = n - 1;
    data.depth = calculateDepth(threadCount);

    parallelMergeSort(&data);

    free(temp);
}