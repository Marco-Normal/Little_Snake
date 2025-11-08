#ifndef COBRINHA_H
#define COBRINHA_H
#include "drawable.h"
#include "food.h"
#include "utils.h"
#define PLAYER_HEAD '@'
#define PLAYER_BODY '#'
#include <stdlib.h>
typedef struct {
  Point position;
  Movement movement;
} Head;
typedef struct {
  Point *items;
  size_t count;
  size_t capacity;
  char repr;
} Body;

typedef struct {
  DRAWABLE_FIELDS;
  Head head;
  Body body;
} Snake;

typedef struct {
  Snake *s;
  MovementsToMake *movs;
} SnakeAi;

void snake_movement(Snake *self);
void snake_check_bounds(Snake *self, size_t height, size_t width);
void snake_update(Snake *self, Board *c, FoodArray *food_board);
void snake_change_direction(Snake *self, char command);
int snake_check_collision(Snake *self);
Snake *snake_init(size_t x, size_t y);
#endif
