CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -pthread

TARGET = pthread_sorting

SRCS = src/Main.c src/quicksort_pthread.c src/mergesort_pthread.c src/oddeven_pthread.c src/utils.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(TARGET).exe