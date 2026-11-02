#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int countPrimes(int arr[], int size) { 
    int count = 0; 
    for(int i = 0; i < size; i++) { 
        int num = arr[i]; 
        if(num < 2) continue;
        int isPrime = 1;
        for(int j = 2; j <= num / 2; j++) { 
            if(num % j == 0) 
            { isPrime = 0;
                break; } } 
                if(isPrime) count++; } return count; } 

void fillArray(int size, int arr[]) {
    for (int i = 0; i < size; i++) {   
        arr[i] = i + 1;
    }
}

int main() {
    int size = 20000; 
    int array[size];

    fillArray(size, array);

    clock_t start = clock();
    int counter = countPrimes(array, size);
    clock_t end = clock();

    double cpu_time_used = (double)(end - start) / CLOCKS_PER_SEC;

    
    FILE *file = fopen("results.txt", "a");

    if (file == NULL) {
        printf("Hiba a results.txt megnyitasakor!\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fprintf(file, "n,prime_count,time_sec\n");
    }

    fprintf(file, "%d,%d,%.10f\n", size, counter, cpu_time_used);

    fclose(file);

    printf("Intervallum: 1-tol %d-ig\n", size);
    printf("Primszamok szama: %d\n", counter);
    printf("Execution time: %.10f seconds\n", cpu_time_used);
    printf("Eredmeny hozzaadva a results.txt fajlhoz.\n");

    return 0;
}
