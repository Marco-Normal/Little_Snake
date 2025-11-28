/**
 * Alunos: 
 *         Augusto Fernandes Ildefonso (15441810)
 *         Allan Vitor de Souza Silva (14712657)
 *         Lucas Lombardi Castro (13672978)
 *         Marco Túlio Mello Silva (12548657)
 *
 * Descrição: Esse é um jogo clássico da cobrinha, porém, invés de gerar apenas
 * uma comidinha, ela gera até 20, controlada por um semáforo. A ideia não é
 * fazer um bom jogo, mas sim utilizar conceitos de threads e de semáforos.
 *
 */
#include "include/cobrinha.h"
#include "include/entity.h"
#include "include/food.h"
#include "include/nob.h"
#include "include/renderer.h"
#include <ncurses.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NOB_IMPLEMENTATION

GameState state = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .rand_mutex = PTHREAD_MUTEX_INITIALIZER,
    .start_signal = PTHREAD_COND_INITIALIZER,
    .game_running = false,
    .game_started = false,
    .score = 0,
};

/**
 * @struct Initializers
 * Struct de conveniência para inicialização de parâmetros gerais
 * para o jogo da cobrinha. Simplesmente é uma forma de inicializar
 * tudo sem estar, necessariamente na main.
 */
typedef struct {
  WINDOW *game;         /**< Janela do jogo */
  Snake *s;             /**< Cobrinha */
  Snake *enemy;         /**< Inimigo */
  EntityArray *objects; /**< Array de Objetos no jogo */
  FoodParams *p;        /**< Parâmetros para a thread de comida */
  EnemyParams *ep;      /**< Parâmetros para a thread do inimigo */
} Initializers;

/**
 * @brief Inicialização de parâmetros básico
 * @return Initializers com valores "padrões" setados.
 */
Initializers init(void) {
  sem_init(&state.n_food, 0, 20);
  Initializers init = {0};
  init.game = render_init_game_window(HEIGHT, WIDTH);
  init.s = snake_init(WIDTH / 2, HEIGHT / 2);
  init.enemy = enemy_snake_init(5, 5);
  init.objects = (EntityArray *)calloc(1, sizeof(EntityArray));
  nob_da_append(init.objects, (void *)init.s);
  nob_da_append(init.objects, (void *)init.enemy);
  init.p = (FoodParams *)calloc(1, sizeof(FoodParams));
  init.p->height = HEIGHT - 2;
  init.p->width = WIDTH - 2;
  init.p->repr = FOOD_REPR;
  init.p->objects = init.objects;

  init.ep = (EnemyParams *)calloc(1, sizeof(EnemyParams));
  init.ep->enemy = init.enemy;
  init.ep->objects = init.objects;
  init.ep->width = WIDTH - 2;
  init.ep->height = HEIGHT - 2;
  return init;
}

void clean_up(Initializers ini) {
  delwin(ini.game);
  free(ini.s);
  free(ini.enemy);
  nob_da_free(*ini.objects);
  free(ini.p);
  free(ini.ep);
}

int main(void) {
  // Seeding da geração randômica. Feita 1 vez na main
  srand(time(NULL));
  Initializers ini = init();
  WINDOW *game = ini.game;
  Snake *snake = ini.s;
  /*
   * NOTE: Esses objetos são compartilhados entre a thread principal e
   * a thread de comidinha. É necessário que esse array tenha o mesmo
   * tempo de vida que a thread principal. Como ele é emprestado para
   * a thread filha, é necessário que ele esteja válido enquanto o a thread
   * esteja viva também. Como essse é o caso, não temos tanto problemas.
   */
  EntityArray *objects = ini.objects;
  FoodParams *p = ini.p;
  EnemyParams *ep = ini.ep;
  state.game_running = 1;
  // Criação da thread que faz a geração de comida.
  pthread_t food_thread;
  pthread_create(&food_thread, NULL, food_routine, (void *)(p));

  pthread_t enemy_thread;
  pthread_create(&enemy_thread, NULL, enemy_routine, (void *)(ep));

  int tecla;
  // Sincrozinação de quando devemos começar a gerar comida.
  // Falamos que começamos o jogo e sinalizamos para a thread
  // de comida isso.
  pthread_mutex_lock(&state.mutex);
  state.game_started = 1;
  pthread_cond_broadcast(&state.start_signal);
  pthread_mutex_unlock(&state.mutex);
  int local_game_running = 1;
  int inimigo_se_comeu = 0;
  // Game Loop
  while (local_game_running) {
    // Pegamos a tecla
    tecla = getch();
    // Mudamos a direção da cobrinha
    snake_change_direction(snake, tecla);
    // Damos update na sua posição.
    // NOTE: ainda não atualizamos a mesma na tela
    snake_update(snake, objects);
    // Checamos se deveríamos sair do jogo ou a cobrinha se comeu.
    if (render_should_quit(tecla) || snake_check_self_collision(snake) ||
        inimigo_se_comeu) {
      pthread_mutex_lock(&state.mutex);
      state.game_running = 0;
      pthread_mutex_unlock(&state.mutex);
      // NOTE: Temos que acordar, a thread de comida para fazermos a
      // limpeza da mesma.
      sem_post(&state.n_food);
    }
    // Região crítica, tela do jogo. As funções do ncurses não são
    // thread safe, então como estamos desenhando na tela por meio do
    // vetor de objetos, temos que fazer um lock no mutex.
    pthread_mutex_lock(&state.mutex);
    inimigo_se_comeu = snake_check_self_collision(ini.enemy);
    werase(game);
    render_frame_loop(game, objects);
    // NOTE: Atualização da variável de controle do jogo.
    // fazemos isso agora para não precisarmos checarmos o state.mutex,
    // quando não estamos sobre a proteção do mutex.
    local_game_running = state.game_running;
    pthread_mutex_unlock(&state.mutex);
    wrefresh(game);
    usleep(100000 * 1);
  }
  render_show_game_over(state.score, state.enemy_score);
  endwin();
  pthread_join(food_thread, NULL);
  pthread_join(enemy_thread, NULL);
  clean_up(ini);
  return 0;
}
