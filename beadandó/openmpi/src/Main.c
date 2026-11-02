#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#include "mergesort_mpi.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3) {
        if (rank == 0) {
            printf("Hasznalat: mpiexec -n <folyamatok_szama> %s <tomb_meret> <algoritmus>\n", argv[0]);
            printf("Algoritmusok: quicksort, mergesort, oddeven\n");
        }

        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);
    char *algorithm = argv[2];

    if (n <= 0) {
        if (rank == 0) {
            printf("Hibas tombmeret!\n");
        }

        MPI_Finalize();
        return 1;
    }

    int *arr = NULL;

    if (rank == 0) {
        arr = (int*)malloc(n * sizeof(int));

        if (arr == NULL) {
            printf("Memoriafoglalasi hiba!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        srand(time(NULL));
        fillArray(arr, n);

        if (n <= 50) {
            printf("Eredeti tomb:\n");
            printArray(arr, n);
        }
    }

    int depth = -1;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();


    if (strcmp(algorithm, "mergesort") == 0) {
        mpiMergeSort(arr, n, rank, size);
    }
    else {
        if (rank == 0) {
            printf("Ismeretlen algoritmus: %s\n", algorithm);
            printf("Hasznalhato: quicksort, mergesort, oddeven\n");
            free(arr);
        }

        MPI_Finalize();
        return 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    double elapsedTime = end - start;

    if (rank == 0) {
        if (n <= 50) {
            printf("Rendezett tomb:\n");
            printArray(arr, n);
        }

        printf("OpenMPI %s futasi ido: %f masodperc\n", algorithm, elapsedTime);

        saveResult("times.txt","OpenMPI",algorithm,n,size,depth,elapsedTime);

        free(arr);
    }

    MPI_Finalize();
    return 0;
}