CC=gcc
LFLAGS=-lpanel -lncurses -lform
CFLAGS=-Wall -g -c

BIN=nhnt
BUILD_DIR=./build/

$(BIN): main.o data.o ui.o
	$(CC) $^ $(LFLAGS) -o $@

main.o: main.c nhnt.h
	$(CC) $(CFLAGS) $<

data.o: data.c nhnt.h
	$(CC) $(CFLAGS) $<

ui.o: ui.c nhnt.h
	$(CC) $(CFLAGS) $<

clean:
	@rm *.o
	@rm $(BIN)

build:
	@cmake -B ./build/ -S .

run:
	@cd $(BUILD_DIR)
	@./$(BIN) -n test 2> err.log
