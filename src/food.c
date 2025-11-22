#include "../include/food.h"
#include "../include/nob.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
extern GameState state;

/**
 * @brief Função de desenho da comida
 * @details Função da classe pai, `Entity`. Nesse caso, o desenho é simples,
 * apenas pegamos a posição (x,y) da comida e sua representação e desenhamos a
 * mesma.
 * @param[in,out] _self Comida
 * @param[out] at Janela para desenho.
 */
void food_draw(void *_self, WINDOW *at) {
  Food *self = (Food *)_self;
  mvwaddch(at, self->position.y, self->position.x, self->repr);
}

/**
 * @brief Checagem se houve colisão da comida com um outro ponto.
 * @details Função da classe pai, `Entity`. Nesse caso não é tão útil
 * visto que ela não se movimenta.
 * @param[in,out] _self Comida
 * @param[out] with Ponto de checagem
 * @return 1 caso haja colisão,
 *         0 caso contrário
 */
int food_collision(void *_self, Point *with) {
  Food *self = (Food *)_self;
  return (self->position.x == with->x) && (self->position.y == with->y);
}

/**
 * @brief Gera uma comida
 * @details Mia suma função de zona crítica, para leitura dentro
 * do array de objetos. Vamos gerar uma comida dentro dos limites
 * definidos por `height` e `width`. A comidinha é gerada aleatoriamente
 * dentro dos limites. Ela é representada por `repr`. Para sabermos se
 * podemos colocar a comidinha lá, precisamos percorrer todas as entidades
 * do jogo para não houver sobreposição. Uma vez achada uma posição que não
 * sobreponha nenhuma, saímos do loop.
 * @param[in] height Altura
 * @param[in] width Largura
 * @param[in] repr Representação da comidinha
 * @param[out] objects Entidades do jogo
 * @return Uma comidinha.
 */
Food *food_gen(int height, int width, char repr, EntityArray *objects) {
  Food *f = (Food *)malloc(sizeof(Food));
  int x, y;
  while (true) {
    pthread_mutex_lock(&state.rand_mutex);
    y = (rand() % (height) + height) % (height);
    x = (rand() % (width) + width) % (width);
    Point p = {.x = x, .y = y};
    pthread_mutex_unlock(&state.rand_mutex);
    int found = 0;
    for (size_t i = 0; i < objects->count; i++) {
      Entity *d = objects->items[i];
      if (!d->collision(d, &p)) {
        found = 1;
        break;
      }
    }
    if (found) {
      break;
    }
  }
  f->position.x = x;
  f->position.y = y;
  f->repr = repr;
  f->draw = (food_draw);
  f->collision = (food_collision);
  f->type = FOOD;
  return f;
}

/**
 * @brief Rotina da thread de comidinha
 * @details Essa é a função que nossa thread de comida vai fazer até o jogo
 * acabar. A ideia geral é, esperarmos o sinal de começo de jogo. Uma vez com
 * o jogo iniciado, vamos fazer um down no semaforo de comidinha, caso ainda
 * tenha espaço para mais, vamos entrar na região crítica do array de entidades.
 * Dentro da região crítica, vamos gerar uma comidinha, adicionar no array de
 * entidades e sair da região crítica e vamos dormir por 1 segundo, apenas para
 * não aparecer todas as comidas de uma vez. Vamos sair do jogo caso nossa flag
 * do jogo funcionando esteja em 0. Com isso, saímos do loop e retornamos nada.
 * @param[in,out] params Description
 * @return Description
 */
void *food_routine(void *params) {
  FoodParams *p = (FoodParams *)params;
  // Esperamos sinal para começarmos
  pthread_mutex_lock(&(state.mutex));
  while (!state.game_started) {
    pthread_cond_wait(&(state.start_signal), &(state.mutex));
  }
  // Começamos
  pthread_mutex_unlock(&(state.mutex));
  while (true) {
    // Down no semáforo de comida
    sem_wait(&state.n_food);
    // Entramos na região crítica
    pthread_mutex_lock(&state.mutex);
    // Se o jogo acabou, saímos
    if (!state.game_running) {
      pthread_mutex_unlock(&state.mutex);
      break;
    }
    // Senão, geramos uma comida
    Food *f = food_gen(p->height, p->width, p->repr, p->objects);
    // Adicionamos na região crítica
    nob_da_append(p->objects, (void *)f);
    pthread_mutex_unlock(&state.mutex);
    // zzzzzzz
    sleep(1);
  }
  return NULL;
}
