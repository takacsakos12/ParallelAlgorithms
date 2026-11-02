#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "quicksort.h"
#include "mergesort.h"
#include "oddeven.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Hasznalat: %s <tomb_meret> <algoritmus>\n", argv[0]);
        printf("Algoritmusok: quicksort, mergesort, oddeven\n");
        return 1;
    }

    int n = atoi(argv[1]);
    char *algorithm = argv[2];

    if (n <= 0) {
        printf("Hibas tombmeret!\n");
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

    clock_t start = clock();

    if (strcmp(algorithm, "quicksort") == 0) {
        quickSort(arr, 0, n - 1);
    }
    else if (strcmp(algorithm, "mergesort") == 0) {
        mergeSort(arr, 0, n - 1);
    }
    else if (strcmp(algorithm, "oddeven") == 0) {
        oddEvenSort(arr, n);
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

    printf("sequential %s futasi ido: %f masodperc\n", algorithm, elapsedTime);

    saveResult("times.txt", "sequential", algorithm, n, 1, -1, elapsedTime);

    free(arr);

    return 0;
}