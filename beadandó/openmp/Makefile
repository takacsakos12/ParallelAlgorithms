CC = gcc
CFLAGS = -Wall -Wextra -O2 -fopenmp -Iinclude

TARGET = openmp_sorting

SRCS = src/Main.c src/quicksort_openmp.c src/mergesort_openmp.c src/oddeven_openmp.c src/utils.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe