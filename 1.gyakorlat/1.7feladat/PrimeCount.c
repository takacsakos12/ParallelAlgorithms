#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int countPrimes(int arr[], int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if ((arr[i])% 2 == 0) {
            count++;
        }
    }
    return count;
}
void fillArray(int size, int arr[]) {
    for(int i = 1; i < size; i++)   {
        arr[i]= i+1;
    }
}

int main()  {
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    int size = 200000; 
    int array[size];
    fillArray(size, array);
    int counter =  countPrimes(array,size);
    printf("A primszamok szama: %d",counter);

    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %.10f seconds\n", cpu_time_used);

    
}

