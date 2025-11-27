##
# Project 1
# @version 0.1

CC ?= gcc
# 1. Separate Include paths (CFLAGS)
INCLUDES := -I./include/ -I./include/ncurses
CFLAGS   ?= -lm -Wall -Wextra -lc -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 $(INCLUDES)

TARGET   ?= Main
ZIP_NAME ?= project.zip

# 2. Separate Linker paths/libs (LDFLAGS/LDLIBS)
# Note: -l:filename forces it to look for that exact filename (static)
LDFLAGS  := -L./lib
LDLIBS   := -l:libncurses.a -lm

SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build

SRC_MAIN  := main.c
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_MAIN  := $(BUILD_DIR)/main.o
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
OBJS      := $(OBJ_MAIN) $(OBJ_FILES)

all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	@echo "Linking $@"
	# Put LDLIBS at the very end
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Compile main.c
$(OBJ_MAIN): $(SRC_MAIN)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<

run:
	@TERM=xterm-256color ./$(TARGET)

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR) $(TARGET)

zip:
	@echo "Creating $(ZIP_NAME)..."
	@zip -q -r $(ZIP_NAME) main.c Makefile \
        `find src -name "*.c"` \
        `find include -name "*.h"` \
        -x "build/*"

.SILENT:
.PHONY: all clean zip
