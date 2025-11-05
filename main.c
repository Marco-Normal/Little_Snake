#define NOB_IMPLEMENTATION
#include "include/cobrinha.h"
#include "include/nob.h"
#include "include/renderer.h"
#include <curses.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
  WINDOW *game = render_init_game_window(HEIGHT, WIDTH);
  Point p = {.x = WIDTH / 2 - 1, .y = HEIGHT / 2 - 1};
  Snake *snake = snake_init(WIDTH / 2, HEIGHT / 2);
  nob_da_append(&snake->body, p);
  int teste = 0;
  int tecla;
  while (1) {
    tecla = getch();
    if (render_should_quit(tecla))
      break;
    snake_change_direction(snake, tecla);
    snake_update(snake, teste < 20);
    render_frame_loop(game, snake);
    if (snake_check_collision(snake)) {
      break;
    }
    usleep(100000 * 1); // 10 FPS
    teste++;
  }
  endwin();
  return 0;
}
