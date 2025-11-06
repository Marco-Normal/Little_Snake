#include "include/food.h"
#include "include/utils.h"
#define NOB_IMPLEMENTATION
#include "include/cobrinha.h"
#include "include/debug.h"
#include "include/nob.h"
#include "include/renderer.h"
#include <curses.h>
#include <unistd.h>

WINDOW *debug_win = NULL;
bool debug_enabled = true; // Toggle with keypress!
int main(void) {
  WINDOW *game = render_init_game_window(HEIGHT, WIDTH);
  Snake *snake = snake_init(WIDTH / 2, HEIGHT / 2);
  init_debug_console(5);
  DrawableArray objects = {0};
  nob_da_append(&objects, (void *)snake);
  Board board = board_init(WIDTH, HEIGHT);
  nob_da_append(&board, snake->head.position.x * snake->head.position.y);
  Food *f = food_gen(HEIGHT, WIDTH, FOOD, &board);
  Board food_board = board_init(WIDTH, HEIGHT);
  nob_da_append(&food_board, f->position.x * f->position.y);
  nob_da_append(&objects, (void *)f);
  int tecla;
  while (1) {
    tecla = getch();
    if (render_should_quit(tecla))
      break;

    werase(debug_win); // Clear previous debug text
    snake_change_direction(snake, tecla);
    snake_update(snake, &board, &food_board);
    render_frame_loop(game, &objects);
    wrefresh(debug_win);

    // Check for toggle key (e.g., F1)
    if (tecla == KEY_F(1)) {
      toggle_debug_console();
    }
    if (snake_check_collision(snake)) {
      break;
    }
    usleep(100000 * 1); // 10 FPS
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