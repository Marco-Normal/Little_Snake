#include "../include/utils.h"
#include "../include/nob.h"
#include <stdlib.h>
Board board_init(size_t width, size_t height) {
  Board b = {0};
  nob_da_reserve(&b, width * height);
  b.width = width;
  b.height = height;
  return b;
}