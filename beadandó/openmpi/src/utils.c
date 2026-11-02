#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void fillArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void saveResult(const char *fileName,const char *method,const char *algorithm,int arraySize,int workers,int depth,double elapsedTime) {
    FILE *file = fopen(fileName, "a");

    if (file == NULL) {
        printf("Nem sikerult megnyitni a %s fajlt!\n", fileName);
        return;
    }

    fprintf(file, "%s;%s;%d;%d;%d;%f\n",method,algorithm,arraySize,workers,depth,elapsedTime);

    fclose(file);
}