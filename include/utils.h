#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#define INDEX(x, y, width) ((x) + (y * width))
#define EMPTY '.'
typedef enum { UP = 1, DOWN, RIGHT, LEFT } Movement;
typedef struct {
  size_t x;
  size_t y;
  char repr;
} Point;
typedef struct {
  Point *items;
  size_t count;
  size_t capacity;
  size_t width;
  size_t height;
} Board;

Board board_init(size_t width, size_t height);
void board_change_repr(Board *b, const Point point, const char repr);
Point *board_get_point(const Board *b, const Point point);
#endif
