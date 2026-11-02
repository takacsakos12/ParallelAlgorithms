#include <stdlib.h>
#include <mpi.h>

#include "mergesort_mpi.h"

static void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *leftArr = (int*)malloc(n1 * sizeof(int));
    int *rightArr = (int*)malloc(n2 * sizeof(int));

    if (leftArr == NULL || rightArr == NULL) {
        free(leftArr);
        free(rightArr);
        return;
    }

    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }

    for (int j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < n1) {
        arr[k++] = leftArr[i++];
    }

    while (j < n2) {
        arr[k++] = rightArr[j++];
    }

    free(leftArr);
    free(rightArr);
}

static void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

static void createCountsAndDispls(int n, int size, int counts[], int displs[]) {
    int base = n / size;
    int remainder = n % size;
    int offset = 0;

    for (int i = 0; i < size; i++) {
        counts[i] = base;

        if (i < remainder) {
            counts[i]++;
        }

        displs[i] = offset;
        offset += counts[i];
    }
}

static void mergeTwoBlocks(int arr[], int firstSize, int secondStart, int secondSize, int temp[]) {
    int i = 0;
    int j = secondStart;
    int k = 0;

    int firstEnd = firstSize;
    int secondEnd = secondStart + secondSize;

    while (i < firstEnd && j < secondEnd) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i < firstEnd) {
        temp[k++] = arr[i++];
    }

    while (j < secondEnd) {
        temp[k++] = arr[j++];
    }
}

static void mergeGatheredParts(int arr[], int n, int counts[], int displs[], int size) {
    int *temp = (int*)malloc(n * sizeof(int));

    if (temp == NULL) {
        return;
    }

    int currentSize = counts[0];

    for (int i = 1; i < size; i++) {
        mergeTwoBlocks(arr, currentSize, displs[i], counts[i], temp);

        currentSize += counts[i];

        for (int j = 0; j < currentSize; j++) {
            arr[j] = temp[j];
        }
    }

    free(temp);
}

void mpiMergeSort(int *arr, int n, int rank, int size) {
    int *sendcounts = (int*)malloc(size * sizeof(int));
    int *displs = (int*)malloc(size * sizeof(int));

    if (sendcounts == NULL || displs == NULL) {
        free(sendcounts);
        free(displs);
        return;
    }

    createCountsAndDispls(n, size, sendcounts, displs);

    int localSize = sendcounts[rank];
    int *localArr = (int*)malloc((localSize > 0 ? localSize : 1) * sizeof(int));

    if (localArr == NULL) {
        free(sendcounts);
        free(displs);
        return;
    }

    MPI_Scatterv(
        arr,
        sendcounts,
        displs,
        MPI_INT,
        localArr,
        localSize,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    if (localSize > 1) {
        mergeSort(localArr, 0, localSize - 1);
    }

    MPI_Gatherv(
        localArr,
        localSize,
        MPI_INT,
        arr,
        sendcounts,
        displs,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    if (rank == 0) {
        mergeGatheredParts(arr, n, sendcounts, displs, size);
    }

    free(localArr);
    free(sendcounts);
    free(displs);
}