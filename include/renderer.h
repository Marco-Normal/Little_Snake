#ifndef RENDERER_H
#define RENDERER_H
#include "cobrinha.h"
#define WIDTH 80
#define HEIGHT 40
#include <curses.h>

typedef struct {
  Drawable **items;
  size_t count;
  size_t capacity;
} DrawableArray;

WINDOW *render_init_game_window(int height, int width);
int render_should_quit(char input);
void render_frame_loop(WINDOW *game, DrawableArray *objects);
void render_draw(WINDOW *at, Drawable *object);
void render_draw_food_array(WINDOW *at, FoodArray *f);
#endif
