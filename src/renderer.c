#include "../include/renderer.h"
#include "../include/debug.h"
#include "../include/nob.h"
#include <curses.h>
#include <pthread.h>
extern pthread_mutex_t mutex;
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
  box(game, 0, 0);
  for (size_t i = 0; i < objects->count; i++) {
    Drawable *d = objects->items[i];
    d->draw(d, game);
  }
}

void render_draw(WINDOW *at, Drawable *object) { object->draw(object, at); }

void render_draw_food_array(WINDOW *at, FoodArray *f) {
  pthread_mutex_lock(&mutex);
  for (size_t i = 0; i < f->count; i++) {
    Food *d = f->items[i];
    debug_log("Drawing %i at (%d, %d)", i, d->position.x, d->position.y);
    d->draw(d, at);
  }
  pthread_mutex_unlock(&mutex);
}
