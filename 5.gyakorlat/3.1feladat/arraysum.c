#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int *array;
    long start;
    long end;
    long long result;
} ThreadData;

typedef struct {
    int *array;
    long start;
    long end;
    int threads;
    long long result;
} RecThreadData;


void* sum_array(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->result = 0;

    for (long i = data->start; i < data->end; i++) {
        data->result += data->array[i];
    }

    return NULL;
}

void* recursive_worker(void* arg);

long long recursive_sum_internal(int *array, long start, long end, int threads) {
    if (threads <= 1 || end - start <= 1) {
        long long sum = 0;
        for (long i = start; i < end; i++) {
            sum += array[i];
        }
        return sum;
    }

    long mid = start + (end - start) / 2;

    RecThreadData left_data = {array, start, mid, threads / 2, 0};
    RecThreadData right_data = {array, mid, end, threads - threads / 2, 0};

    pthread_t left_thread;

    pthread_create(&left_thread, NULL, recursive_worker, &left_data);

    right_data.result = recursive_sum_internal(
        right_data.array,
        right_data.start,
        right_data.end,
        right_data.threads
    );

    pthread_join(left_thread, NULL);

    return left_data.result + right_data.result;
}

void* recursive_worker(void* arg) {
    RecThreadData* data = (RecThreadData*)arg;
    data->result = recursive_sum_internal(
        data->array,
        data->start,
        data->end,
        data->threads
    );
    return NULL;
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Hasznalat: %s <tomb_meret> <szalak_szama>\n", argv[0]);
        return 1;
    }

    long n = atol(argv[1]);
    int p = atoi(argv[2]);

    int *array = malloc(n * sizeof(int));
    if (array == NULL) {
        printf("Memoria hiba!\n");
        return 1;
    }

    for (long i = 0; i < n; i++) {
        array[i] = 1;
    }

 
    pthread_t *threads = malloc(p * sizeof(pthread_t));
    ThreadData *data = malloc(p * sizeof(ThreadData));

    long chunk = n / p;
    long start_index = 0;

    clock_t start1 = clock();

    for (int i = 0; i < p; i++) {
        data[i].array = array;
        data[i].start = start_index;
        data[i].end = (i == p - 1) ? n : start_index + chunk;
        data[i].result = 0;

        pthread_create(&threads[i], NULL, sum_array, &data[i]);
        start_index = data[i].end;
    }

    long long sum_chunk = 0;
    for (int i = 0; i < p; i++) {
        pthread_join(threads[i], NULL);
        sum_chunk += data[i].result;
    }

    clock_t end1 = clock();
    double time_chunk = (double)(end1 - start1) / CLOCKS_PER_SEC;

   
    clock_t start2 = clock();

    long long sum_recursive = recursive_sum_internal(array, 0, n, p);

    clock_t end2 = clock();
    double time_recursive = (double)(end2 - start2) / CLOCKS_PER_SEC;


    printf("Meret: %ld | Szalak: %d\n\n", n, p);

    printf("Egyenlo reszek:\n");
    printf("  Osszeg: %lld\n", sum_chunk);
    printf("  Ido: %f s\n\n", time_chunk);

    printf("Rekurziv:\n");
    printf("  Osszeg: %lld\n", sum_recursive);
    printf("  Ido: %f s\n", time_recursive);

    if (sum_chunk != sum_recursive) {
        printf("\nHIBA: nem egyeznek az eredmenyek!\n");
    }

    free(array);
    free(threads);
    free(data);

    return 0;
}