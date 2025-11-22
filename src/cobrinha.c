#include "../include/cobrinha.h"
#include "../include/nob.h"
#include <ncurses.h>
#include <pthread.h>
#include <semaphore.h>
extern GameState state;

/**
 * @brief Inicializa uma struct `Head` para a cobrinha
 * @param[in] x Na coordenada `x`
 * @param[in] y Na coordenada `y`
 * @param[in] mov Com movimento `mov`
 * @return A cabeça da cobrinha
 */
Head init_head(size_t x, size_t y, Movement mov) {
  Head c;
  c.position.y = y;
  c.position.x = x;
  c.movement = mov;
  return c;
}

/**
 * @brief Inicializa o corpo da cobrinha
 * @param[in] repr Como o corpo vai ser desenhado
 * @return Corpo da cobrinha
 */
Body init_body(char repr) {
  Body b;
  b.repr = repr;
  b.capacity = 0;
  b.count = 0;
  b.items = NULL;
  return b;
}
int snake_check_collision(void *_self, Point *with);
void snake_draw(void *_self, WINDOW *at);

/**
 * @brief Inicializa a cobrinha na posição (`x`, `y`), como
 * movimento padrão para baixo.
 * @details Inicializa a classe cobrinha, que herda da classe entidade.
 * Possui movimento inicial para baixo. A representação da cabeça é dada por
 * `PLAYER_HEAD`
 * @param[in] x Posição inicial
 * @param[in] y Posição inicial
 * @return Um membro da classe `Snake`
 */
Snake *snake_init(size_t x, size_t y) {
  Snake *snake = (Snake *)malloc(sizeof(Snake) * 1);
  snake->head = init_head(x, y, DOWN);
  snake->body = init_body(PLAYER_BODY);
  snake->repr = PLAYER_HEAD;
  snake->draw = (snake_draw);
  snake->collision = (snake_check_collision);
  snake->type = SNAKE;
  return snake;
}

/**
 * @brief Realiza a checagem se a cobrinha bateu nela mesma
 * @details Para cada ponto do corpo, checamos se a posição da cabeça
 * é a mesma que aquela do corpo, se sim, houve auto-colisão
 * @param[in,out] self A cobrinha
 * @return 1 caso houve auto-colisão
 *         0 caso contrário
 */
int snake_check_self_collision(Snake *self) {
  nob_da_foreach(Point, p, &self->body) {
    if (p->x == self->head.position.x && p->y == self->head.position.y) {
      return 1;
    }
  }
  return 0;
}

/**
 * @brief Realizamos a checagem se a cobrinha está dentro da área do jogo.
 * @details Admitimos que nossa área é toroidal, ou seja, se ela vai para a
 * extremidade direita da tela, ela aparecerá na extremidade esquerda da tela.
 * @param[in,out] self Cobrinha
 * @param[in] height Altura da tela
 * @param[in] width Largura da tela
 */
void snake_check_bounds(Snake *self, size_t height, size_t width) {
  self->head.position.y = (self->head.position.y + height) % (height);
  self->head.position.x = (self->head.position.x + width) % (width);
}

/**
 * @brief Checa se a cobrinha teve colisão com o outro ponto
 * @details Função da classe pai, `Entity` que indica como lidar
 * caso tenha necessidade da checagem da colisão da cobrinha com outro
 * objeto. No caso da cobrinha, esse método realiza a checagem em cada
 * parte do corpo e posteriormente com a cabeça, caso em algum delas tenha
 * colisão, retornamos 1.
 * @param[in,out] _self Membro da classe `Entity`
 * @param[out] with Ponto que queremos checar a colisão
 * @return 1 caso `self` colidiu com `with`
 *         0 caso contrário.
 */
int snake_check_collision(void *_self, Point *with) {
  Snake *self = (Snake *)_self;
  nob_da_foreach(Point, p, &self->body) {
    if (p->x == with->x && p->y == with->y) {
      return 1;
    }
  }
  return (self->head.position.x == with->x) &&
         (self->head.position.y == with->y);
}

/**
 * @brief Função de desenho da cobrinha
 * @details Função da classe pai, `Entity` que indica como a cobrinha
 * deverá ser desenhada no jogo. Nesse caso, fazemos o loop para cada
 * parte do corpo e colocamos no jogo sua representação. Por fim, fazemos
 * o mesmo com a cabeça
 * @param[in,out] _self Cobrinha
 * @param[out] at Jogo onde vamos desenha-la
 */
void snake_draw(void *_self, WINDOW *at) {
  Snake *self = (Snake *)_self;
  int h, w;
  getmaxyx(at, h, w);
  snake_check_bounds(self, h - 2, w - 2);
  nob_da_foreach(Point, p, &self->body) {
    mvwaddch(at, p->y, p->x, self->body.repr);
  }
  mvwaddch(at, self->head.position.y, self->head.position.x, self->repr);
  return;
}

/**
 * @brief Realiza o movimento da cobrinha baseado em onde
 * a cabeça estava se movendo
 * @param[in,out] self A cobrinha
 */
void snake_movement(Snake *self) {
  switch (self->head.movement) {
  case UP:
    self->head.position.y--;
    break;
  case DOWN:
    self->head.position.y++;
    break;
  case RIGHT:
    self->head.position.x++;
    break;
  case LEFT:
    self->head.position.x--;
    break;
  }
}

/**
 * @brief A partir do `command` realizamos a mudança de direção da cobrinha
 * @param[in,out] self Cobrinha
 * @param[in] command Comando de movimento.
 */
void snake_change_direction(Snake *self, char command) {
  switch (command) {
  case 'w':
    self->head.movement = UP;
    break;
  case 's':
    self->head.movement = DOWN;
    break;
  case 'a':
    self->head.movement = LEFT;
    break;
  case 'd':
    self->head.movement = RIGHT;
    break;
  default:
    break;
  }
}

/**
 * @brief Checa se a cobrinha comeu comida
 * @details Função que age sobre a região crítica, para leitura e escrita.
 * Vamos ter a cobrinha e todas as entidades presentes no jogo. Vamos ter que
 * dar lock no mutex e fazer um loop sob todas as entidades. Caso o tipo da
 * entidade seja cobrinha, pulamos. Caso contrário, checamos se houve colisão.
 * Em caso positivo, temos que aumentar o `score`, e retirar a comida que
 * comemos da lista de entidades. Retirando uma comida, precisamos aumentar
 * nosso semáforo de comida, para que outra possa ser gerada. Posteriormente,
 * liberamos o mutex
 * @param[in,out] snake Cobrinha
 * @param[out] board Todas as entidades presentes em um dado instante no jogo
 * @return 1, caso ela tenha comido uma comidinha
 *         0, caso contrário.
 */
int snake_eat_food(Snake *snake, EntityArray *board) {
  pthread_mutex_lock(&state.mutex);
  for (size_t i = 0; i < board->count; i++) {
    Entity *d = board->items[i];
    if (d->type == SNAKE) {
      continue;
    }
    if (d->collision(d, &snake->head.position)) {
      nob_da_remove_unordered(board, i);
      sem_post(&state.n_food);
      state.score++;
      pthread_mutex_unlock(&state.mutex);
      return 1;
    }
  }
  pthread_mutex_unlock(&state.mutex);
  return 0;
}

/**
 * @brief Função geral de atualização da cobrinha
 * @details Função que realiza a agregação de funções gerais da cobrinha,
 * checa caso ela tenha comida e realiza o update da posição da mesma,
 * de forma que aconteça um movimento no proximo frame.
 * @param[in,out] self Cobrinha
 * @param[out] board Array de entidades para checar se ela deveria
 * crescer ou não
 */
void snake_update(Snake *self, EntityArray *board) {
  int grow = snake_eat_food(self, board);
  Point old_head = self->head.position;
  snake_movement(self);
  Point prev_pos = old_head;
  for (size_t i = 0; i < self->body.count; ++i) {
    Point current_pos = self->body.items[i];
    self->body.items[i] = prev_pos;
    prev_pos = current_pos;
  }
  if (grow) {
    nob_da_append(&self->body, prev_pos);
  }
}