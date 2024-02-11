CC=gcc
LFLAGS=-lpanel -lncurses -lform
CFLAGS=-Wall -g -c

BIN=nhnt
BUILD_DIR=./build/

default: build

$(BIN): main.o data.o ui.o
	$(CC) $^ $(LFLAGS) -o $@

main.o: main.c nhnt.h
	$(CC) $(CFLAGS) $<

data.o: data.c nhnt.h
	$(CC) $(CFLAGS) $<

ui.o: ui.c nhnt.h
	$(CC) $(CFLAGS) $<


.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)*

.PHONY: build
build:
	@cmake -B $(BUILD_DIR) -S .
	@cd $(BUILD_DIR) && cmake --build .

.PHONY: run
run:
	@cd $(BUILD_DIR) && ./$(BIN) -n test 2> err.log
