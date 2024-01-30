CC=gcc
LFLAGS=-lncurses
CFLAGS=-Wall -g -c

BIN=nhnt

nhnt: main.o
	$(CC) $^ $(LFLAGS) -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^

