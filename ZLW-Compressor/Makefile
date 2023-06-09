CC = gcc
CFLAGS = -Wall -Wextra

SRC_DIR = ../src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, %.o, $(SRC_FILES))

all: zlw

zlw: zlw.o $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

zlw.o: zlw.c
	$(CC) $(CFLAGS) -c $<

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f zlw.o zlw $(OBJ_FILES)
