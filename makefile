##
# Little Snake
# @version 0.1

CC        ?= gcc
CFLAGS    ?= -lm -Wall -Wextra -pedantic -lc -lcurses -O2
TARGET    ?= Main
ZIP_NAME  ?= project.zip
SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build
SRC_MAIN  := main.c
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_MAIN  := $(BUILD_DIR)/main.o
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
OBJS      := $(OBJ_MAIN) $(OBJ_FILES)

all: $(TARGET)

# Estágio final
$(TARGET): $(OBJS)
	@echo "Linking $@"
	$(CC)  $(CFLAGS) -o $@ $^

# Compila a Main
$(OBJ_MAIN): $(SRC_MAIN)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<

# Compila os outros arquivos para .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<

run:
	@./$(TARGET)

clean:
	@echo "Limpando..."
	@rm -rf $(BUILD_DIR) $(TARGET)

zip:
	@echo "Criando $(ZIP_NAME)..."
	@zip -q -r $(ZIP_NAME) main.c Makefile \
        `find src -name "*.c"` \
        `find include -name "*.h"` \
        -x "build/*"

.SILENT:
.PHONY: all clean zip
