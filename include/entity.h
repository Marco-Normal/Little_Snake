#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "utils.h"
#include <ncurses.h>
typedef enum { SNAKE = 1, FOOD, ENEMY } Type;
#define ENTITY_FIELDS                                                          \
  char repr;                                                                   \
  void (*draw)(void *self, WINDOW *at);                                        \
  int (*collision)(void *self, Point *with);                                   \
  Type type

/**
 * @struct Entity
 * Classe abstrata que representa todas as entidades no nosso jogo.
 * Serve para unifircarmos algumas  interfaces de funções, em principal para
 * o renderização. Basicamente ele possui uma representação, qual o tipo, se é
 * a cobrinha ou comida, e ponteiros para funções de utilidade.
 */
typedef struct {
  ENTITY_FIELDS; /**< campos necessários */
} Entity;

/**
 * @struct EntityArray
 * Array dinâmico de entidades. Puramente conveniência
 */
typedef struct {
  Entity **items;  /**< Ponteiros de entidades */
  size_t count;    /**< Número de entidades atuais */
  size_t capacity; /**< Capacidade do array */
} EntityArray;

#endif