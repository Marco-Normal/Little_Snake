#ifndef UTILS_H
#define UTILS_H
#include <semaphore.h>
#include <stdlib.h>
#define INDEX(x, y, width) ((x) + (y * width))
// Define o tipo de movimento
typedef enum { UP = 1, DOWN, RIGHT, LEFT } Movement;

/**
 * @struct Point
 * Representação de um ponto com representação
 */
typedef struct {
  size_t x;  /**< Posição x */
  size_t y;  /**< Posição y */
  char repr; /**< Representação do ponto */
} Point;

/**
 * @struct GameState
 * Representa nosso estado de jogo. Ele contém
 * algumas variáveis globais e mutexes e sinais
 * para sincronização do nosso jogo
 */
typedef struct {
  pthread_mutex_t mutex;       /**< Mutex geral para entrada e saída
                                 da região crítica */
  pthread_mutex_t rand_mutex;  /**< Mutex para geração de números aleátorios,
                                 visto a função `rand` padrão não tem segurança
                                de thread. */
  pthread_cond_t start_signal; /**< Sinal de inicio de jogo */
  sem_t n_food;                /**< Semáforo de quantidade de comida
                                 possível no jogo */
  int game_running;            /**< O jogo está rolando? */
  int game_started;            /**< O jogo começou? */
  int score;                   /**< Score atual */
} GameState;

#endif
