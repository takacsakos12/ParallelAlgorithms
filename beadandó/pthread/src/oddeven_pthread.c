#include <stdlib.h>
#include <pthread.h>

#include "oddeven_pthread.h"

typedef struct {
    int *arr;
    int n;
    int threadId;
    int threadCount;
} OddEvenData;

static pthread_barrier_t barrier;
static pthread_mutex_t mutex;

static int globalSorted;

static void *oddEvenWorker(void *arg) {
    OddEvenData *data = (OddEvenData*)arg;

    int *arr = data->arr;
    int n = data->n;
    int threadId = data->threadId;
    int threadCount = data->threadCount;

    while (1) {
        if (threadId == 0) {
            globalSorted = 1;
        }

        pthread_barrier_wait(&barrier);

        for (int phase = 0; phase < 2; phase++) {
            int localChanged = 0;

            for (int i = phase + 2 * threadId; i < n - 1; i += 2 * threadCount) {
                if (arr[i] > arr[i + 1]) {
                    int temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                    localChanged = 1;
                }
            }

            if (localChanged) {
                pthread_mutex_lock(&mutex);
                globalSorted = 0;
                pthread_mutex_unlock(&mutex);
            }

            pthread_barrier_wait(&barrier);
        }

        pthread_barrier_wait(&barrier);

        if (globalSorted) {
            break;
        }

        pthread_barrier_wait(&barrier);
    }

    return NULL;
}

void pthreadOddEvenSort(int arr[], int n, int threadCount) {
    if (threadCount <= 1) {
        int sorted = 0;

        while (!sorted) {
            sorted = 1;

            for (int i = 0; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    int temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                    sorted = 0;
                }
            }

            for (int i = 1; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    int temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                    sorted = 0;
                }
            }
        }

        return;
    }

    if (threadCount > n) {
        threadCount = n;
    }

    pthread_t *threads = malloc(threadCount * sizeof(pthread_t));
    OddEvenData *threadData = malloc(threadCount * sizeof(OddEvenData));

    if (threads == NULL || threadData == NULL) {
        free(threads);
        free(threadData);
        return;
    }

    pthread_barrier_init(&barrier, NULL, threadCount);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < threadCount; i++) {
        threadData[i].arr = arr;
        threadData[i].n = n;
        threadData[i].threadId = i;
        threadData[i].threadCount = threadCount;

        pthread_create(&threads[i], NULL, oddEvenWorker, &threadData[i]);
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);

    free(threads);
    free(threadData);
}