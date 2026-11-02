#include <omp.h>

#include "oddeven_openmp.h"

void openmpOddEvenSort(int arr[], int n, int threadCount) {
    omp_set_num_threads(threadCount);

    int sorted = 0;

    while (!sorted) {
        int changed = 0;

        #pragma omp parallel for reduction(|:changed)
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                changed = 1;
            }
        }

        #pragma omp parallel for reduction(|:changed)
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                changed = 1;
            }
        }

        if (!changed) {
            sorted = 1;
        }
    }
}