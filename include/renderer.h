#ifndef RENDERER_H
#define RENDERER_H
#include "cobrinha.h"
#define WIDTH 80
#define HEIGHT 40
#include <curses.h>

void render_draw(WINDOW *at, Snake *snake);
WINDOW *render_init_game_window(int height, int width);
int render_should_quit(char input);
void render_frame_loop(WINDOW *game, Snake *snake);
void render_draw(WINDOW *at, Snake *snake);
#endif