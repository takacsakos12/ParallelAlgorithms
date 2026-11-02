#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <omp.h>

typedef struct {
    double *arr;
    long n;
    int depth;
} ThreadData;


double sequential_product(double *arr, long n) {
    double product = 1.0;
    for (long i = 0; i < n; i++) {
        product *= arr[i];
    }
    return product;
}


double recursive_product_with_depth(double *arr, long n, int depth) {
    if (n == 0) return 1.0;
    if (n == 1) return arr[0];

    if (depth == 0) {
        return sequential_product(arr, n);
    }

    long mid = n / 2;
    double left_product = recursive_product_with_depth(arr, mid, depth - 1);
    double right_product = recursive_product_with_depth(arr + mid, n - mid, depth - 1);

    return left_product * right_product;
}


void* pthread_recursive_worker(void *arg) {
    ThreadData *data = (ThreadData*)arg;

    double *result = (double*)malloc(sizeof(double));
    if (result == NULL) {
        pthread_exit(NULL);
    }

    if (data->n == 0) {
        *result = 1.0;
        pthread_exit(result);
    }

    if (data->n == 1) {
        *result = data->arr[0];
        pthread_exit(result);
    }

    if (data->depth == 0) {
        *result = sequential_product(data->arr, data->n);
        pthread_exit(result);
    }

    long mid = data->n / 2;

    ThreadData left_data;
    left_data.arr = data->arr;
    left_data.n = mid;
    left_data.depth = data->depth - 1;

    ThreadData right_data;
    right_data.arr = data->arr + mid;
    right_data.n = data->n - mid;
    right_data.depth = data->depth - 1;

    pthread_t left_thread, right_thread;

    double *left_result = NULL;
    double *right_result = NULL;

    pthread_create(&left_thread, NULL, pthread_recursive_worker, &left_data);
    pthread_create(&right_thread, NULL, pthread_recursive_worker, &right_data);

    pthread_join(left_thread, (void**)&left_result);
    pthread_join(right_thread, (void**)&right_result);

    double left_val = (left_result != NULL) ? *left_result : 1.0;
    double right_val = (right_result != NULL) ? *right_result : 1.0;

    free(left_result);
    free(right_result);

    *result = left_val * right_val;
    pthread_exit(result);
}

double pthread_product(double *arr, long n, int depth) {
    ThreadData data;
    data.arr = arr;
    data.n = n;
    data.depth = depth;

    double *result = NULL;
    pthread_t root_thread;

    pthread_create(&root_thread, NULL, pthread_recursive_worker, &data);
    pthread_join(root_thread, (void**)&result);

    double final_result = (result != NULL) ? *result : 1.0;
    free(result);

    return final_result;
}


double openmp_parallel_for_product(double *arr, long n) {
    double product = 1.0;

    #pragma omp parallel
    {
        double local_product = 1.0;

        #pragma omp for
        for (long i = 0; i < n; i++) {
            local_product *= arr[i];
        }

        #pragma omp critical
        {
            product *= local_product;
        }
    }

    return product;
}

double openmp_reduction_product(double *arr, long n) {
    double product = 1.0;

    #pragma omp parallel for reduction(*:product)
    for (long i = 0; i < n; i++) {
        product *= arr[i];
    }

    return product;
}


void fill_array(double *arr, long n) {
    for (long i = 0; i < n; i++) {
        arr[i] = 1.0 + ((double)rand() / RAND_MAX) * 0.00001;
    }
}

double measure_sequential(double *arr, long n, double *result) {
    double start = omp_get_wtime();
    *result = sequential_product(arr, n);
    double end = omp_get_wtime();
    return end - start;
}

double measure_recursive(double *arr, long n, int depth, double *result) {
    double start = omp_get_wtime();
    *result = recursive_product_with_depth(arr, n, depth);
    double end = omp_get_wtime();
    return end - start;
}

double measure_pthread(double *arr, long n, int depth, double *result) {
    double start = omp_get_wtime();
    *result = pthread_product(arr, n, depth);
    double end = omp_get_wtime();
    return end - start;
}

double measure_openmp_for(double *arr, long n, double *result) {
    double start = omp_get_wtime();
    *result = openmp_parallel_for_product(arr, n);
    double end = omp_get_wtime();
    return end - start;
}

double measure_openmp_reduction(double *arr, long n, double *result) {
    double start = omp_get_wtime();
    *result = openmp_reduction_product(arr, n);
    double end = omp_get_wtime();
    return end - start;
}

int main() {
    srand((unsigned)time(NULL));

    int depth = 4;

    long sizes[] = {
        10000,
        50000,
        100000,
        200000,
        500000,
        1000000
    };

    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("Tomb elemek szorzasanak futasi idejei\n");
    printf("Max rekurzios melyseg: %d\n\n", depth);

    printf("%-12s %-15s %-15s %-15s %-15s %-15s\n",
           "N",
           "Szekvencialis",
           "Rekurziv",
           "POSIX",
           "OpenMP for",
           "OMP reduction");

    for (int s = 0; s < num_sizes; s++) {
        long n = sizes[s];

        double *arr = (double*)malloc(n * sizeof(double));
        if (arr == NULL) {
            printf("Memoriafoglalasi hiba n = %ld esetben!\n", n);
            return 1;
        }

        fill_array(arr, n);

        double result_seq, result_rec, result_posix, result_omp_for, result_omp_red;
        double time_seq, time_rec, time_posix, time_omp_for, time_omp_red;

        time_seq = measure_sequential(arr, n, &result_seq);
        time_rec = measure_recursive(arr, n, depth, &result_rec);
        time_posix = measure_pthread(arr, n, depth, &result_posix);
        time_omp_for = measure_openmp_for(arr, n, &result_omp_for);
        time_omp_red = measure_openmp_reduction(arr, n, &result_omp_red);

        printf("%-12ld %-15f %-15f %-15f %-15f %-15f\n",
               n,
               time_seq,
               time_rec,
               time_posix,
               time_omp_for,
               time_omp_red);

        free(arr);
    }

    return 0;
}