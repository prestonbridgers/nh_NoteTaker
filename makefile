BUILD_DIR=./build
BIN=nhnt

default: build

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
