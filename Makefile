SRC_DIR=src
INC_DIR=include

OBJ_DIR=obj
BIN_DIR=bin

CFLAGS += -std=c11 -Wall -Wextra -Wpedantic -Werror
CFLAGS += -O0
CFLAGS += -I$(INC_DIR)

LDFLAGS = 

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/6502: $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR):
	mkdir -p $@
$(BIN_DIR):
	mkdir -p $@

.PHONY: build clean test

build: $(BIN_DIR)/6502

test: build
	$(BIN_DIR)/6502

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
