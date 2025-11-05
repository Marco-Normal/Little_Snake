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

void render_frame_loop(WINDOW *game, DrawableArray *objects) {
  werase(game);
  box(game, 0, 0);
  for (Drawable *d = *(objects)->items;
       d < *(objects)->items + (objects)->count; ++d) {
    d->draw(d, game);
  }
  wrefresh(game);
}

void render_draw(WINDOW *at, Drawable *object) { object->draw(object, at); }
