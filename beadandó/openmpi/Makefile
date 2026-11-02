CC = mpicc
CFLAGS = -Wall -Wextra -O2 -Iinclude

TARGET = mpi_sorting

SRCS = src/Main.c src/mergesort_mpi.c src/utils.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe