#include "../include/renderer.h"
#include "../include/nob.h"
#include <curses.h>

WINDOW *render_init_game_window(int height, int width) {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  nodelay(stdscr, TRUE);
  int start_y = (LINES - height) / 2;
  int start_x = (COLS - width) / 2;
  WINDOW *game = newwin(height, width, start_y, start_x);
  box(game, 0, 0);
  wrefresh(game);
  return game;
}

int render_should_quit(char input) { return input == 'q' ? 1 : 0; }

void render_frame_loop(WINDOW *game, Snake *snake) {
  werase(game);
  box(game, 0, 0);
  render_draw(game, snake);
  wrefresh(game);
}

void render_draw(WINDOW *at, Snake *snake) {
  int h, w;
  getmaxyx(at, h, w);
  snake_check_bounds(snake, h, w);
  getmaxyx(at, h, w);
  nob_da_foreach(Point, p, &snake->body) {
    mvwaddch(at, p->y, p->x, snake->body.repr);
  }
  mvwaddch(at, snake->head.position.y, snake->head.position.x,
           snake->head.repr);
}