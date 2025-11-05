#ifndef COBRINHA_H
#define COBRINHA_H
#define PLAYER_HEAD '@'
#define PLAYER_BODY '#'
#include "stdlib.h"
typedef enum { UP = 1, DOWN, RIGHT, LEFT } Movement;
typedef struct {
  int x;
  int y;
} Point;

typedef struct {
  Point position;
  Movement movement;
  char repr;
} Head;

typedef struct {
  Point *items;
  size_t count;
  size_t capacity;
  char repr;
} Body;

typedef struct {
  Head head;
  Body body;
} Snake;

typedef struct {
  Point position;
  char repr;
} Food;
void snake_movement(Snake *self);
void snake_check_bounds(Snake *self, int width, int height);
void snake_update(Snake *self, int grow);
void snake_change_direction(Snake *self, char command);
int snake_check_collision(Snake *self);
Snake *snake_init(int x, int y);
#endif