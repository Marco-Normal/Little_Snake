#include "../include/food.h"
#include "../include/nob.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
extern sem_t n_food;
extern int game_running;
extern pthread_mutex_t mutex;
extern pthread_mutex_t rand_mutex;
void food_draw(void *_self, WINDOW *at) {
  Food *self = (Food *)_self;
  mvwaddch(at, self->position.y, self->position.x, self->repr);
}

Food *food_gen(int height, int width, char repr, Board *b) {
  Food *f = (Food *)malloc(sizeof(Food));
  int x, y;
  while (true) {
    pthread_mutex_lock(&rand_mutex);
    y = rand() % height;
    x = rand() % width;
    pthread_mutex_unlock(&rand_mutex);
    int found = 0;
    nob_da_foreach(Point, p, b) {
      if (p->repr == EMPTY) {
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

void *food_routine(void *params) {
  FoodParams *p = (FoodParams *)params;
  while (game_running) {
    sem_wait(&n_food);
    pthread_mutex_lock(&mutex);
    Food *f = food_gen(p->height, p->width, p->repr, p->b);
    nob_da_append(p->food_array, f);
    board_change_repr(p->b, f->position, f->repr);
    pthread_mutex_unlock(&mutex);
    sleep(1);
  }
  return 0;
}
