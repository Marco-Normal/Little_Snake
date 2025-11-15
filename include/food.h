#ifndef FOOD_H
#define FOOD_H
#define FOOD_REPR 'o'
#define MAX_FOOD 20
#include "entity.h"
typedef struct {
  ENTITY_FIELDS;
  Point position;
} Food;

typedef struct {
  int height;
  int width;
  char repr;
  EntityArray *objects;
} FoodParams;

Food *food_gen(int height, int width, char repr, EntityArray *objects);
void *food_routine(void *params);
#endif
