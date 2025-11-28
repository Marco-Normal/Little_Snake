#ifndef RENDERER_H
#define RENDERER_H
#include "entity.h"
#define WIDTH 80
#define HEIGHT 40
#include <curses.h>

/**
 * @brief Inicializa a janela do jogo, com altura
 * `height` e largura `width`
 * @param[in] height Altura, em linhas de terminal
 * @param[in] width Largura, em linhas de terminal
 * @return Janela do jogo
 */
WINDOW *render_init_game_window(int height, int width);

/**
 * @brief Se o `input` for `q`, devemos sair, se não ficamos
 * @param[in] input Valor para checarmos
 * @return 1 caso devemos sair
 *         0 caso contrário
 */
int render_should_quit(char input);

/**
 * @brief Renderiza os `objects` em `game`
 * @param[in,out] game Janela onde deveria ser renderizado
 * @param[in] objects Array de objetos a serem renderizados
 */
void render_frame_loop(WINDOW *game, const EntityArray *objects);

/**
 * @brief Renderiza uma nova janela de game over
 * @param[in] score
 * @param[in] enemy_score
 */
void render_show_game_over(int score, int enemy_score);
#endif
