CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS =

SRC_DIR = ../src
SRC_FILES = $(SRC_DIR)/hash_map.c
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))

TARGET = struct_testing

.PHONY: all clean

all: $(TARGET)

$(TARGET): struct_testing.o $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

struct_testing.o: struct_testing.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_FILES): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) struct_testing.o $(TARGET)
