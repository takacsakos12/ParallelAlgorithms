#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "quicksort_openmp.h"
#include "mergesort_openmp.h"
#include "oddeven_openmp.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Hasznalat: %s <tomb_meret> <algoritmus> <szalak_szama>\n", argv[0]);
        printf("Algoritmusok: quicksort, mergesort, oddeven\n");
        return 1;
    }

    int n = atoi(argv[1]);
    char *algorithm = argv[2];
    int threadCount = atoi(argv[3]);

    if (n <= 0 || threadCount <= 0) {
        printf("Hibas parameter!\n");
        return 1;
    }

    int *arr = (int*)malloc(n * sizeof(int));

    if (arr == NULL) {
        printf("Memoriafoglalasi hiba!\n");
        return 1;
    }

    srand(time(NULL));
    fillArray(arr, n);

    if (n <= 50) {
        printf("Eredeti tomb:\n");
        printArray(arr, n);
    }

    int depth = -1;

    if (strcmp(algorithm, "quicksort") == 0) {
        depth = calculateDepth(threadCount);
    }

    clock_t start = clock();

    if (strcmp(algorithm, "quicksort") == 0) {
        openmpQuickSort(arr, n, threadCount);
    }
    else if (strcmp(algorithm, "mergesort") == 0) {
        openmpMergeSort(arr, n, threadCount);
    }
    else if (strcmp(algorithm, "oddeven") == 0) {
        openmpOddEvenSort(arr, n, threadCount);
    }
    else {
        printf("Ismeretlen algoritmus: %s\n", algorithm);
        printf("Hasznalhato: quicksort, mergesort, oddeven\n");

        free(arr);
        return 1;
    }

    clock_t end = clock();

    double elapsedTime = (double)(end - start) / CLOCKS_PER_SEC;

    if (n <= 50) {
        printf("Rendezett tomb:\n");
        printArray(arr, n);
    }

    printf("OpenMP %s futasi ido: %f masodperc\n", algorithm, elapsedTime);

    saveResult("times.txt", "OpenMP", algorithm, n, threadCount, depth, elapsedTime);

    free(arr);

    return 0;
}