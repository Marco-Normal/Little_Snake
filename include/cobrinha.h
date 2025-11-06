#ifndef COBRINHA_H
#define COBRINHA_H
#define PLAYER_HEAD '@'
#define PLAYER_BODY '#'
#include "drawable.h"
#include "food.h"
#include "utils.h"
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

void snake_movement(Snake *self);
void snake_check_bounds(Snake *self, int height, int width);
void snake_update(Snake *self, Board *c, FoodArray *food_board);
void snake_change_direction(Snake *self, char command);
int snake_check_collision(Snake *self);
Snake *snake_init(int x, int y);
#endif
