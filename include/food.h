#ifndef FOOD_H
#define FOOD_H
#define FOOD 'o'
#define MAX_FOOD 20
#include "drawable.h"
#include "utils.h"
typedef struct {
  DRAWABLE_FIELDS;
  Point position;
} Food;

typedef struct {
  Food **items;
  size_t count;
  size_t capacity;
} FoodArray;

typedef struct {
  int height;
  int width;
  char repr;
  Board *b;
  FoodArray *food_array;
} FoodParams;

Food *food_gen(int height, int width, char repr, Board *b);
void *food_routine(void *params);
#endif
