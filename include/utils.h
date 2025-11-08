#ifndef UTILS_H
#define UTILS_H
#include <semaphore.h>
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
typedef struct {
  pthread_mutex_t mutex;
  pthread_mutex_t rand_mutex;
  pthread_cond_t food_available;
  pthread_cond_t start_signal;
  sem_t n_food;
  int game_running;
  int game_started;
} GameState;

typedef struct {
  Movement *items;
  size_t count;
  size_t capacity;
} MovementsToMake;

Board board_init(const size_t width, const size_t height);
void board_change_repr(Board *b, const Point point, const char repr);
Point *board_get_point(const Board *b, const Point point);
#endif
