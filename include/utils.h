#ifndef UTILS_H
#define UTILS_H
#include <semaphore.h>
#include <stdlib.h>
#define INDEX(x, y, width) ((x) + (y * width))
typedef enum { UP = 1, DOWN, RIGHT, LEFT } Movement;

typedef struct {
  size_t x;
  size_t y;
  char repr;
} Point;
typedef struct {
  pthread_mutex_t mutex;
  pthread_mutex_t rand_mutex;
  pthread_cond_t food_available;
  pthread_cond_t start_signal;
  pthread_mutex_t score_mutex;
  sem_t n_food;
  int game_running;
  int game_started;
  int score;
} GameState;

#endif
