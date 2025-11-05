#define NOB_IMPLEMENTATION
#include "include/cobrinha.h"
#include "include/nob.h"
#include "include/renderer.h"
#include <curses.h>
#include <unistd.h>

int main(void) {
  WINDOW *game = render_init_game_window(HEIGHT, WIDTH);
  Point p = {.x = WIDTH / 2 - 1, .y = HEIGHT / 2 - 1};
  Snake *snake = snake_init(WIDTH / 2, HEIGHT / 2);
  nob_da_append(&snake->body, p);
  DrawableArray objects = {0};
  nob_da_append(&objects, (void *)snake);
  int tecla;
  while (1) {
    tecla = getch();
    if (render_should_quit(tecla))
      break;
    snake_change_direction(snake, tecla);
    snake_update(snake);
    render_frame_loop(game, &objects);
    if (snake_check_collision(snake)) {
      break;
    }
    usleep(100000 * 1); // 10 FPS
  }
  endwin();
  return 0;
}
