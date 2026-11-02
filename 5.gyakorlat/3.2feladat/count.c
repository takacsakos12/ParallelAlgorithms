#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int *array;
    long start;
    long end;
    long result;
} ThreadData;

void fill_random_int_array(int *arr, long n, int min, int max) {
    for (long i = 0; i < n; i++) {
        arr[i] = min + rand() % (max - min + 1);
    }
}

long count_even(int *array, long n) {
    long count = 0;
    for (long i = 0; i < n; i++) {
        if (array[i] % 2 == 0) {
            count++;
        }
    }
    return count;
}

long count_odd(int *array, long n) {
    long count = 0;
    for (long i = 0; i < n; i++) {
        if (array[i] % 2 != 0) {
            count++;
        }
    }
    return count;
}

long count_zero(int *array, long n) {
    long count = 0;
    for (long i = 0; i < n; i++) {
        if (array[i] == 0) {
            count++;
        }
    }
    return count;
}

long count_negative(int *array, long n) {
    long count = 0;
    for (long i = 0; i < n; i++) {
        if (array[i] < 0) {
            count++;
        }
    }
    return count;
}

long count_in_range(int *array, long n, int min, int max) {
    long count = 0;
    for (long i = 0; i < n; i++) {
        if (array[i] >= min && array[i] <= max) {
            count++;
        }
    }
    return count;
}

void* thread_count_even(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    data->result = count_even(data->array + data->start, data->end - data->start);
    return NULL;
}

void* thread_count_odd(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    data->result = count_odd(data->array + data->start, data->end - data->start);
    return NULL;
}

void* thread_count_zero(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    data->result = count_zero(data->array + data->start, data->end - data->start);
    return NULL;
}

void* thread_count_negative(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    data->result = count_negative(data->array + data->start, data->end - data->start);
    return NULL;
}

void* thread_count_in_range(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    int min = -500; // Példa érték
    int max = 500;  // Példa érték
    data->result = count_in_range(data->array + data->start, data->end - data->start, min, max);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Hasznalat: %s <tomb_meret> <szalak_szama>\n", argv[0]);
        return 1;
    }

    long n = atol(argv[1]);
    int num_threads = atoi(argv[2]);

    int *array = malloc(n * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Hiba a memoria foglalasakor!\n");
        return 1;
    }

    srand(time(NULL));
    fill_random_int_array(array, n, -1000, 1000);

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadData *data = malloc(num_threads * sizeof(ThreadData));

    long chunk_size = n / num_threads;
    long start_index = 0;

    clock_t t1 = clock();

    for (int i = 0; i < num_threads; i++) {
        data[i].array = array;
        data[i].start = start_index;
        data[i].end = (i == num_threads - 1) ? n : start_index + chunk_size;
        data[i].result = 0;

        pthread_create(&threads[i], NULL, thread_count_even, &data[i]);
        start_index = data[i].end;
    }

    long total = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total += data[i].result;
    }

    clock_t t2 = clock();
    double time = (double)(t2 - t1) / CLOCKS_PER_SEC;

    printf("Paros elemek szama: %ld\n", total);
    printf("Futasi ido: %f s\n", time);

    free(array);
    free(threads);
    free(data);

    return 0;
}