CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

TARGET = sequential_sorting

SRCS = src/Main.c src/quicksort.c src/mergesort.c src/oddeven.c src/utils.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe