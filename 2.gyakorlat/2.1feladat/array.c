#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getSum(const int array[], int size);
int getMax(const int array[], int size);
int getMin(const int array[], int size);

int recGetSum(const int array[], int size);
int recGetMax(const int array[], int size);
int recGetMin(const int array[], int size);

int getSum(const int array[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += array[i];
    return sum;
}

int getMax(const int array[], int size) {
    int tempMax = array[0];
    for (int i = 1; i < size; i++)
        if (array[i] > tempMax)
            tempMax = array[i];
    return tempMax;
}

int getMin(const int array[], int size) {
    int tempMin = array[0];
    for (int i = 1; i < size; i++)
        if (array[i] < tempMin)
            tempMin = array[i];
    return tempMin;
}

int recGetSum(const int array[], int size) {
    if (size <= 0) return 0;
    return array[size - 1] + recGetSum(array, size - 1);
}

int recGetMax(const int array[], int size) {
    if (size == 1) return array[0];
    int tempMax = recGetMax(array, size - 1);
    return (array[size - 1] > tempMax) ? array[size - 1] : tempMax;
}

int recGetMin(const int array[], int size) {
    if (size == 1) return array[0];
    int tempMin = recGetMin(array, size - 1);
    return (array[size - 1] < tempMin) ? array[size - 1] : tempMin;
}

int main(void) {

    int n;

    printf("Add meg az elemszamot (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Hibas elemszam.\n");
        return 1;
    }

    int *arr = malloc(n * sizeof(int));
    if (!arr) {
        printf("Memoriafoglalasi hiba.\n");
        return 1;
    }

    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100001;   
    }


    clock_t t1 = clock();
    int sumI = getSum(arr, n);
    int maxI = getMax(arr, n);
    int minI = getMin(arr, n);
    clock_t t2 = clock();

    clock_t t3 = clock();
    int sumR = recGetSum(arr, n);
    int maxR = recGetMax(arr, n);
    int minR = recGetMin(arr, n);
    clock_t t4 = clock();

    printf("\n--- Iterativ ---\n");
    printf("Osszeg: %d\n", sumI);
    printf("Max: %d\n", maxI);
    printf("Min: %d\n", minI);

    printf("\n--- Rekurziv ---\n");
    printf("Osszeg: %d\n", sumR);
    printf("Max: %d\n", maxR);
    printf("Min: %d\n", minR);

    double timeIter = (double)(t2 - t1) / CLOCKS_PER_SEC;
    double timeRec  = (double)(t4 - t3) / CLOCKS_PER_SEC;

    printf("\n--- Futasi idok (mp) ---\n");
    printf("Iterativ: %.9f\n", timeIter);
    printf("Rekurziv: %.9f\n", timeRec);

    free(arr);
    return 0;
}