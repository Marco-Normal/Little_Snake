#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#define X_POS(value, width) value / width
#define Y_POS(value, height) value % height
typedef enum { UP = 1, DOWN, RIGHT, LEFT } Movement;
typedef struct {
  int x;
  int y;
} Point;
typedef struct {
  int *items;
  size_t count;
  size_t capacity;
  size_t width;
  size_t height;
} Board;

Board board_init(size_t width, size_t height);
#endif