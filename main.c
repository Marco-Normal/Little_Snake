#include "include/food.h"
#include "include/search.h"
#include "include/utils.h"
#include <bits/pthreadtypes.h>
#include <sched.h>
#include <search.h>
#define NOB_IMPLEMENTATION
#include "include/cobrinha.h"
#include "include/debug.h"
#include "include/nob.h"
#include "include/renderer.h"
#include <curses.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

WINDOW *debug_win = NULL;
bool debug_enabled = true;
FoodArray f_array = {0};
sem_t n_food;
int game_running = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rand_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
  srand(time(NULL));
  sem_init(&n_food, 0, 20);
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
  pthread_t food_thread;
  pthread_create(&food_thread, NULL, food_routine, (void *)(&p));
  game_running = 1;
  /* pthread_t search_thread; */
  SearchParameterList s = {
      .b = &board, .f = &f_array, .s = snake, .search = dfs};
  /* pthread_create(&search_thread, NULL, search_routine, (void *)(&s)); */
  MovementsToMake *m = (MovementsToMake *)search_routine((void *)&s);
  /* pthread_join(search_thread, (void **)m); */
  int tecla;
  while (game_running) {
    tecla = getch();
    if (render_should_quit(tecla))
      break;
    if (debug_enabled) {
      werase(debug_win);
    }
    werase(game);
    if (m->count > 0) {
      Movement mov = nob_da_last(m);
      nob_da_remove_unordered(m, m->count);
      snake->head.movement = mov;
    } else {
      snake_change_direction(snake, tecla);
    }
    snake_update(snake, &board, &f_array);
    render_frame_loop(game, &objects);
    render_draw_food_array(game, &f_array);
    if (debug_enabled) {
      wrefresh(debug_win);
    }
    wrefresh(game);
    if (tecla == KEY_F(1)) {
      toggle_debug_console();
    }
    if (snake_check_collision(snake)) {
      break;
    }
    usleep(100000 * 1);
  }
  endwin();
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
