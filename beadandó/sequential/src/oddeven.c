#include "oddeven.h"

void oddEvenSort(int arr[], int n) {
    int sorted = 0;

    while (!sorted) {
        sorted = 1;

        // Paros fazis: (0,1), (2,3), (4,5) ...
        for (int i = 0; i <= n - 2; i += 2) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                sorted = 0;
            }
        }

        // Paratlan fazis: (1,2), (3,4), (5,6) ...
        for (int i = 1; i <= n - 2; i += 2) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                sorted = 0;
            }
        }
    }
}