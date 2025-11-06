#include "../include/food.h"
#include "../include/nob.h"
#include <stdlib.h>
void food_draw(void *_self, WINDOW *at) {
  Food *self = (Food *)_self;
  mvwaddch(at, self->position.y, self->position.x, self->repr);
}

Food *food_gen(int height, int width, char repr, Board *b) {
  Food *f = (Food *)malloc(sizeof(Food));
  srand(time(NULL));
  int x, y;
  while (true) {
    y = rand() % height;
    x = rand() % width;
    int found = 0;
    nob_da_foreach(int, p, b) {
      if (X_POS(*p, width) != x && Y_POS(*p, height) != y) {
        found = 1;
        break;
      }
    }
    if (found) {
      break;
    }
  }
  f->position.x = x;
  f->position.y = y;
  f->repr = repr;
  f->draw = (food_draw);
  return f;
}