CC=gcc
LFLAGS=-lncurses
CFLAGS=-Wall -g -c

BIN=nhnt

$(BIN): main.o data.o
	$(CC) $^ $(LFLAGS) -o $@

main.o: main.c nhnt.h
	$(CC) $(CFLAGS) $<

data.o: data.c nhnt.h
	$(CC) $(CFLAGS) $<

clean:
	@rm *.o
	@rm $(BIN)
