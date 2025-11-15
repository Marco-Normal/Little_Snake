#include "../include/food.h"
#include "../include/nob.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
extern GameState state;
void food_draw(void *_self, WINDOW *at) {
  Food *self = (Food *)_self;
  mvwaddch(at, self->position.y, self->position.x, self->repr);
}

int food_collision(void *_self, Point *with) {
  Food *self = (Food *)_self;
  return (self->position.x == with->x) && (self->position.y == with->y);
}

Food *food_gen(int height, int width, char repr, EntityArray *objects) {
  Food *f = (Food *)malloc(sizeof(Food));
  int x, y;
  while (true) {
    pthread_mutex_lock(&state.rand_mutex);
    y = rand() % (height);
    x = rand() % (width);
    Point p = {.x = x, .y = y};
    pthread_mutex_unlock(&state.rand_mutex);
    int found = 0;
    for (size_t i = 0; i < objects->count; i++) {
      Entity *d = objects->items[i];
      if (!d->collision(d, &p)) {
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
  f->collision = (food_collision);
  f->type = FOOD;
  return f;
}

void *food_routine(void *params) {
  FoodParams *p = (FoodParams *)params;
  pthread_mutex_lock(&(state.mutex));
  while (!state.game_started) {
    pthread_cond_wait(&(state.start_signal), &(state.mutex));
  }
  pthread_mutex_unlock(&(state.mutex));
  while (true) {
    sem_wait(&state.n_food);
    pthread_mutex_lock(&state.mutex);
    if (!state.game_running) {
      pthread_mutex_unlock(&state.mutex);
      sem_post(&state.n_food);
      break;
    }
    Food *f = food_gen(p->height, p->width, p->repr, p->objects);
    nob_da_append(p->objects, (void *)f);
    pthread_mutex_unlock(&state.mutex);
    sleep(1);
  }
  return NULL;
}
