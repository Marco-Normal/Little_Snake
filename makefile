##
# Projeto 1
#
# @file
# @version 0.1

# Default parameters (can be overridden by passing arguments to make)
# Compiler and flags
CC ?= gcc
CFLAGS ?= -lm -Wall -Wextra -ggdb -lc
TARGET ?= Main
ZIP_NAME ?= project.zip
LINKER_FLAGS ?= -lncurses

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

# Source files
SRC_MAIN := main.c
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_MAIN := $(BUILD_DIR)/main.o
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Combine all objects
OBJS := $(OBJ_MAIN) $(OBJ_FILES)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	echo "Linking $@"
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $@ $^

# Compile main.c
$(OBJ_MAIN): $(SRC_MAIN)
	@mkdir -p $(dir $@)
	echo "Compiling $<"
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -c -o $@ $<

# Compile source files from src/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	echo "Compiling $<"
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -c -o $@ $<

run:
	@./$(TARGET)

# Clean build artifacts
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR) $(TARGET)

# Create project zip
zip:
	@echo "Creating $(ZIP_NAME)..."
	@zip -q -r $(ZIP_NAME) main.c Makefile \
        `find src -name "*.c"` \
        `find include -name "*.h"` \
        -x "build/*"

# Silent execution
.SILENT:

.PHONY: all clean zip
#end