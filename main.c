#include "include/cobrinha.h"
#include "include/entity.h"
#include "include/food.h"
#include "include/nob.h"
#include "include/renderer.h"
#include <curses.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NOB_IMPLEMENTATION
sem_t n_food;

GameState state = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .rand_mutex = PTHREAD_MUTEX_INITIALIZER,
    .food_available = PTHREAD_COND_INITIALIZER,
    .start_signal = PTHREAD_COND_INITIALIZER,
    .score_mutex = PTHREAD_MUTEX_INITIALIZER,
    .game_running = false,
    .game_started = false,
    .score = 0,
};

typedef struct {
  WINDOW *game;
  Snake *s;
  EntityArray *objects;
  FoodParams *p;
} Initializers;

Initializers init(void) {
  sem_init(&state.n_food, 0, 20);
  Initializers init = {0};
  init.game = render_init_game_window(HEIGHT, WIDTH);
  init.s = snake_init(WIDTH / 2, HEIGHT / 2);
  init.objects = (EntityArray *)calloc(1, sizeof(EntityArray));
  nob_da_append(init.objects, (void *)init.s);
  init.p = (FoodParams *)calloc(1, sizeof(FoodParams));
  init.p->height = HEIGHT - 2;
  init.p->width = WIDTH - 2;
  init.p->repr = FOOD_REPR;
  init.p->objects = init.objects;
  return init;
}

int main(void) {
  srand(time(NULL));
  Initializers ini = init();
  WINDOW *game = ini.game;
  Snake *snake = ini.s;
  EntityArray *objects = ini.objects;
  FoodParams *p = ini.p;
  state.game_running = 1;
  pthread_t food_thread;
  pthread_create(&food_thread, NULL, food_routine, (void *)(p));
  int tecla;
  pthread_mutex_lock(&state.mutex);
  state.game_started = 1;
  pthread_cond_broadcast(&state.start_signal);
  pthread_mutex_unlock(&state.mutex);
  int local_game_running = 1;
  while (local_game_running) {
    tecla = getch();
    snake_change_direction(snake, tecla);
    snake_update(snake, objects);
    if (render_should_quit(tecla) || snake_check_self_collision(snake)) {
      pthread_mutex_lock(&state.mutex);
      state.game_running = 0;
      pthread_mutex_unlock(&state.mutex);
      // NOTE: Temos que acordar, a thread de comida para fazermos a
      // limpeza da mesma.
      sem_post(&state.n_food);
    }
    pthread_mutex_lock(&state.mutex);
    werase(game);
    render_frame_loop(game, objects);
    local_game_running = state.game_running;
    pthread_mutex_unlock(&state.mutex);
    wrefresh(game);
    usleep(100000 * 1);
  }
  endwin();
  pthread_join(food_thread, NULL);
  return 0;
}
