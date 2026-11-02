#include "quicksort.h"

void quickSort(int arr[], int low, int high) {
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
        quickSort(arr, low, j);
    }

    if (i < high) {
        quickSort(arr, i, high);
    }
}