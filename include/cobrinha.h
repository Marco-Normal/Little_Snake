#ifndef COBRINHA_H
#define COBRINHA_H
#include "entity.h"
#include "food.h"
#include "renderer.h"
#include "utils.h"
#define PLAYER_HEAD '@'
#define PLAYER_BODY '#'
#define ENEMY_HEAD 'O'
#define ENEMY_BODY '*'
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

/**
 * @brief Função geral de atualização da cobrinha. Considera entidades
 * comidinhas para poder saber se ela deveria crescer ou não
 * @param[in,out] self Cobrinha
 * @param[out] board Entidades no jogo. Inclui a cobrinha.
 */
void snake_update(Snake *self, EntityArray *board);

/**
 * @brief A partir do `command` realizamos a mudança de direção da cobrinha
 * @param[in,out] self Cobrinha
 * @param[in] command Comando de movimento.
 */
void snake_change_direction(Snake *self, char command);

/**
 * @brief Realiza a checagem se a cobrinha bateu nela mesma
 * @param[in,out] self Cobrinha
 * @return 1 caso houve auto-colisão
 *         0 caso contrário
 */
int snake_check_self_collision(Snake *self);

/**
 * @brief Inicializa a cobrinha na posição (`x`, `y`), como
 * movimento padrão para baixo.
 * @param[in] x Posição inicial
 * @param[in] y Posição inicial
 * @return Um membro da classe `Snake`
 */
Snake *snake_init(size_t x, size_t y);

/**
 * @brief Inicializa a cobrinha inimiga na posição (`x`, `y`), como
 * movimento padrão para baixo.
 * @param[in] x Posição inicial
 * @param[in] y Posição inicial
 * @return Um membro da classe `Snake`
 */
Snake *enemy_snake_init(size_t x, size_t y);

/**
 * @struct EnemyParams
 * Parâmetros necessários para a rotina do inimigo
 */
typedef struct {
  Snake *enemy;         /**< Self */
  EntityArray *objects; /**< Array de entidades */
  int width;            /**< Largura do jogo */
  int height;           /**< Altura do jogo */
} EnemyParams;

/**
 * @brief Rotina de movimentação da cobrinha inimiga
 * @param[in,out] arg Aceita como parâmetros a struct `EnemyParams`
 */
void *enemy_routine(void *arg);

#endif
