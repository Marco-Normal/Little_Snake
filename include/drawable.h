#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <ncurses.h>
#define DRAWABLE_FIELDS                                                        \
  char repr;                                                                   \
  void (*draw)(void *self, WINDOW *at);
// Classe abstrata para todos os itens que podem ser desenhados
typedef struct {
  DRAWABLE_FIELDS
} Drawable;

#endif