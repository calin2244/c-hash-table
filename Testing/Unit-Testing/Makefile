CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Source files
SRC_DIR = ../../src
OBJ_DIR = obj

# Chaining test
CHAINING_TARGET = chaining_test
CHAINING_SRCS = chaining_test.c $(wildcard $(SRC_DIR)/*.c)
CHAINING_OBJS = $(CHAINING_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# LP test
LP_TARGET = lp_test
LP_SRCS = lp_test.c $(wildcard $(SRC_DIR)/*.c)
LP_OBJS = $(LP_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# QP Test
QP_TARGET = qp_test
QP_SRCS = qp_test.c $(wildcard $(SRC_DIR)/*.c)
QP_OBJS = $(QP_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean chaining_test lp_test

all: chaining_test lp_test

chaining_test: $(CHAINING_TARGET)

$(CHAINING_TARGET): $(CHAINING_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

lp_test: $(LP_TARGET)

$(LP_TARGET): $(LP_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

qp_test: $(QP_TARGET)

$(QP_TARGET): $(QP_OBJS)
	$(CC) $(CFLAGS) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

chaining_test.o: chaining_test.c
	$(CC) $(CFLAGS) -c $< -o $@

lp_test.o: lp_test.c
	$(CC) $(CFLAGS) -c $< -o $@

qp_test.o: qp_test.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(CHAINING_TARGET) $(LP_TARGET) $(QP_TARGET)