#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <windows.h>



int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Használat: %s <fájlnév> <szám>\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];
    int n = atoi(argv[2]);
    if(n <= 0) {
        printf("Kérem adjon meg egy pozitív egész számot!\n");
        return 1;
    }
    int *array = (int *)malloc(n * sizeof(int));
    if (array == NULL) {
        printf("Memóriafoglalási hiba!\n");
        return 1;
    }
    srand((unsigned)time(NULL));
    clock_t start_gen, end_gen;
    clock_t start_save, end_save;
    start_gen = clock();
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 1000000; // 0 és 999999 közötti véletlenszám
    }
    end_gen = clock();
    double gen_time = (double)(end_gen - start_gen) / CLOCKS_PER_SEC;

    start_save = clock();
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Hiba a fájl megnyitásakor!\n");
        free(array);
        return 1;
    }
    for (int i = 0; i < n; i++) {
        fprintf(file, "%d\n", array[i]);
    }
    end_save = clock();
    double save_time = (double)(end_save - start_save) / CLOCKS_PER_SEC;
    fprintf(file, "Fajlba iras ideje: %.6f masodperc\n", save_time);
    fclose(file);
    

    
    

    printf("Veletlenszamok generalasanak ideje: %.6f masodperc\n", gen_time);
    printf("Fajlba iras ideje: %.6f masodperc\n", save_time);   
    free(array);
    return 0;


}