#ifndef FOOD_H
#define FOOD_H
#define FOOD_REPR 'o'
#define MAX_FOOD 20
#include "entity.h"

/**
 * @struct Food
 * Classe comida. Herda os campos da classe `Entity`
 */
typedef struct {
  ENTITY_FIELDS;  /**< Campos da classe pai */
  Point position; /**< Posição dentro do jogo */
} Food;

/**
 * @struct FoodParams
 * Parâmetros necessários para que a thread de comidinha funcione
 */
typedef struct {
  int height;           /**< Altura do jogo */
  int width;            /**< Largura do jogo */
  char repr;            /**< Representação da comidinha */
  EntityArray *objects; /**< Objetos no jogo */
} FoodParams;

/**
 * @brief Rotina da thread de comidinha. Os parâmetros são transformados em
 * `FoodParams`.
 * @param[in,out] params Parâmetros para geração de comidinha
 * @return NULL
 */
void *food_routine(void *params);
#endif
