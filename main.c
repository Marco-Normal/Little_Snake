#include "include/cobrinha.h"
#include "include/debug.h"
#include "include/food.h"
#include "include/nob.h"
#include "include/renderer.h"
#include <curses.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NOB_IMPLEMENTATION
WINDOW *debug_win = NULL;
bool debug_enabled = true;
FoodArray f_array = {0};
sem_t n_food;

GameState state = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .rand_mutex = PTHREAD_MUTEX_INITIALIZER,
    .food_available = PTHREAD_COND_INITIALIZER,
    .start_signal = PTHREAD_COND_INITIALIZER,
    .game_running = false,
    .game_started = false,

};

int main(void) {
  srand(time(NULL));
  sem_init(&state.n_food, 0, 20);
  WINDOW *game = render_init_game_window(HEIGHT, WIDTH);
  Snake *snake = snake_init(WIDTH / 2, HEIGHT / 2);
  DrawableArray objects = {0};
  Board board = board_init(WIDTH, HEIGHT);
  init_debug_console(5);
  nob_da_append(&objects, (void *)snake);
  board_change_repr(&board, snake->head.position, snake->repr);
  nob_da_append(&board, snake->head.position);
  FoodParams p = {.height = HEIGHT - 2,
                  .width = WIDTH - 2,
                  .repr = FOOD,
                  .b = &board,
                  .food_array = &f_array};
  state.game_running = 1;
  pthread_t food_thread;
  pthread_create(&food_thread, NULL, food_routine, (void *)(&p));
  int tecla;
  pthread_mutex_lock(&state.mutex);
  state.game_started = 1;
  pthread_cond_broadcast(&state.start_signal);
  pthread_mutex_unlock(&state.mutex);
  int local_game_running = 1;
  while (local_game_running) {
    tecla = getch();
    snake_change_direction(snake, tecla);
    snake_update(snake, &board, &f_array);
    if (render_should_quit(tecla) || snake_check_collision(snake)) {
      pthread_mutex_lock(&state.mutex);
      state.game_running = 0;
      pthread_mutex_unlock(&state.mutex);
      // NOTE: Temos que acordar, a thread de comida para fazermos a
      // limpeza da mesma.
      sem_post(&state.n_food);
    }
    if (debug_enabled) {
      werase(debug_win);
    }
    werase(game);
    render_frame_loop(game, &objects);
    render_draw_food_array(game, &f_array);
    if (debug_enabled) {
      wrefresh(debug_win);
    }
    wrefresh(game);
    if (tecla == KEY_F(1)) {
      toggle_debug_console();
    }
    pthread_mutex_lock(&state.mutex);
    local_game_running = state.game_running;
    pthread_mutex_unlock(&state.mutex);
    usleep(100000 * 1);
  }
  endwin();
  pthread_join(food_thread, NULL);
  return 0;
}

// Initialize debug console at bottom of screen
void init_debug_console(int height) {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);
  // Create window: height lines at bottom, full width
  debug_win = newwin(height, max_x, max_y - height, 0);
  wbkgd(debug_win, COLOR_PAIR(1) | A_BOLD); // Optional: colored background
  scrollok(debug_win, TRUE);                // Auto-scroll when full
  wrefresh(debug_win);
}

// Print debug messages (like printf but to debug window)
void debug_log(const char *format, ...) {
  if (!debug_enabled || !debug_win)
    return;

  va_list args;
  va_start(args, format);
  vw_printw(debug_win, format, args); // Formatted print
  va_end(args);

  wprintw(debug_win, "\n"); // Newline after each message
  wrefresh(debug_win);
}

// Toggle debug console visibility
void toggle_debug_console() {
  debug_enabled = !debug_enabled;
  if (debug_enabled) {
    werase(debug_win);
    debug_log("Debug console enabled");
  }
  wrefresh(debug_win);
}
