#ifndef FOOD_H
#define FOOD_H
#include "drawable.h"
#include "utils.h"
#define FOOD 'o'
typedef struct {
  DRAWABLE_FIELDS;
  Point position;
} Food;

Food *food_gen(int height, int width, char repr, Board *b);
#endif