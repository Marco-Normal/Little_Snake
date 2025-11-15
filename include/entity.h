#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "utils.h"
#include <ncurses.h>
typedef enum { SNAKE = 1, FOOD } Type;
#define ENTITY_FIELDS                                                          \
  char repr;                                                                   \
  void (*draw)(void *self, WINDOW *at);                                        \
  int (*collision)(void *self, Point *with);                                   \
  Type type;

// Classe abstrata para todos os itens que podem ser desenhados
typedef struct {
  ENTITY_FIELDS
} Entity;

typedef struct {
  Entity **items;
  size_t count;
  size_t capacity;
} EntityArray;

#endif