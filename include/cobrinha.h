#ifndef COBRINHA_H
#define COBRINHA_H
#define PLAYER_HEAD '@'
#define PLAYER_BODY '#'
#define FOOD 'o'
#include "stdlib.h"
#include <ncurses.h>
#define DRAWABLE_FIELDS                                                        \
  char repr;                                                                   \
  void (*draw)(void *self, WINDOW *at);
// Classe abstrata para todos os itens que podem ser desenhados
typedef struct {
  DRAWABLE_FIELDS
} Drawable;
typedef enum { UP = 1, DOWN, RIGHT, LEFT } Movement;
typedef struct {
  int x;
  int y;
} Point;

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
  DRAWABLE_FIELDS;
  Point position;
} Food;

typedef struct {
  Point *items;
  size_t count;
  size_t capacity;
} Board;

void snake_movement(Snake *self);
void snake_check_bounds(Snake *self, int height, int width);
void snake_update(Snake *self);
void snake_change_direction(Snake *self, char command);
int snake_check_collision(Snake *self);
Snake *snake_init(int x, int y);
Board *board_init(void);
Food *food_gen(int height, int width, char repr);
#endif
