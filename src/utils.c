#include "../include/utils.h"
#include "../include/nob.h"
#include <stdlib.h>
Board board_init(size_t width, size_t height) {
  Board b = {0};
  nob_da_reserve(&b, width * height);
  b.width = width;
  b.height = height;
  for (size_t w = 0; w < width; w++) {
    for (size_t h = 0; h < height; h++) {
      Point p = {.x = w, .y = h, .repr = EMPTY};
      nob_da_append(&b, p);
    }
  }
  return b;
}

void board_change_repr(Board *b, const Point point, const char repr) {
  nob_da_foreach(Point, p, b) {
    if (p->x == point.x && p->y == point.y) {
      p->repr = repr;
    }
  }
}
Point *board_get_point(const Board *b, const Point point) {
  nob_da_foreach(Point, p, b) {
    if (p->x == point.x && p->y == point.y) {
      return p;
    }
  }
  return NULL;
}