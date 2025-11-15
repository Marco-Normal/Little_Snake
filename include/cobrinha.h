#ifndef COBRINHA_H
#define COBRINHA_H
#include "entity.h"
#include "food.h"
#include "renderer.h"
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
  ENTITY_FIELDS;
  Head head;
  Body body;
} Snake;

void snake_movement(Snake *self);
void snake_check_bounds(Snake *self, size_t height, size_t width);
void snake_update(Snake *self, EntityArray *board);
void snake_change_direction(Snake *self, char command);
int snake_check_self_collision(Snake *self);
Snake *snake_init(size_t x, size_t y);
#endif
