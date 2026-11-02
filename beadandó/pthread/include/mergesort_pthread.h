#ifndef MERGESORT_PTHREAD_H
#define MERGESORT_PTHREAD_H

typedef struct {
    int *arr;
    int left;
    int right;
} ThreadData;

void pthreadMergeSort(int arr[], int n, int threadCount);

#endif