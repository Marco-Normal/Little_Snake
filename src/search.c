#include "../include/search.h"
#include "../include/cobrinha.h"
#include "../include/nob.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
  Board board_snapshot;
  FoodArray food_snapshot;
  Point snake_head;
} FrameSnapshot;

int food_at(FoodArray f_array, Point p) {
  for (size_t i = 0; i < f_array.count; i++) {
    Food *f = f_array.items[i];
    if (f->position.x == p.x && f->position.y == p.y) {
      return 1;
    }
  }
  return 0;
}
