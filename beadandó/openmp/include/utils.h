#ifndef UTILS_H
#define UTILS_H

void fillArray(int arr[], int n);
void printArray(int arr[], int n);
void saveResult(const char *fileName, const char *method, const char *algorithm, int arraySize, int workers, int depth, double elapsedTime);
int calculateDepth(int workerCount); 

#endif