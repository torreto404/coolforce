CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./src
LDFLAGS = 

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

TARGET = $(BIN_DIR)/coolforce
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/hash_algo.c $(SRC_DIR)/md5.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS = $(SRC_DIR)/hash_algo.h $(SRC_DIR)/md5.h

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

rebuild: clean all

.PHONY: all clean rebuild
