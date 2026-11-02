#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int isInArray(int *arr, int size, int num) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == num) {
            return 1;
        }
    }
    return 0; 
}

void generateUniqueRandomNumbers(int *arr, int size, int max_value) {
    int count = 0;
    while (count < size) {
        int num = rand() % max_value; // Generate a random number
        if (!isInArray(arr, count, num)) {
            arr[count] = num; 
            count++;
        }
    }
}

int main() {
srand(time(NULL)); 
int n; 
 printf("Add meg az elemszamot: "); 
    scanf("%d", &n); 
int *arr = malloc(n * sizeof(int)); 
if (!arr) { 
        printf("Memoriafoglalasi hiba.\n"); return 1; } 
    clock_t start = clock(); 
        generateUniqueRandomNumbers(arr, n, __INT_MAX__); 
    clock_t end = clock();

double gen_time = (double)(end - start) / CLOCKS_PER_SEC; 
printf("Generalasi ido: %.6f masodperc\n", gen_time);
free(arr);
return 0;
}