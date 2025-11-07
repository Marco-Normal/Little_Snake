#ifndef SEARCH_H_
#define SEARCH_H_

#include "cobrinha.h"
#include "food.h"
#include "utils.h"
typedef struct {
  Movement *items;
  size_t count;
  size_t capacity;
} MovementsToMake;

typedef MovementsToMake *(*SearchAlgorithm)(const Point, const Board *,
                                            const FoodArray *);

typedef struct {
  Board *b;
  FoodArray *f;
  Snake *s;
  SearchAlgorithm search;

} SearchParameterList;
MovementsToMake *search_with(const Snake *snake, const Board *board,
                             const FoodArray *f, SearchAlgorithm s);
MovementsToMake *dfs(const Point start, const Board *board, const FoodArray *f);
void *search_routine(void *params);

#endif