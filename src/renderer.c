#include "../include/renderer.h"
#include "../include/nob.h"
#include <curses.h>
#include <pthread.h>
extern GameState state;

/**
 * @brief Inicializa a janela do jogo
 * @details Inicializa, por meio de funções do curses, janelas padrões,
 * com alguns valores padrões, e cria uma janela com tamanho `height` x `width`
 * @param[in] height Altura, em linhas de terminal
 * @param[in] width Largura, em linhas de termina
 * @return Retorna a janela do jogo
 */
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

// Se q é pressionado, significa que vamos sair do jogo
int render_should_quit(char input) { return input == 'q' ? 1 : 0; }

/**
 * @brief Renderiza os `objects` em `game`
 * @details A partir de uma array da classe abstrata `Entity`, renderiza
 * todos as entidades dentro da janela do jogo
 * @param[in,out] game Janela onde deveria ser renderizado
 * @param[in] objects Array de objetos a serem renderizados
 */
void render_frame_loop(WINDOW *game, const EntityArray *objects) {
  box(game, 0, 0);
  for (size_t i = 0; i < objects->count; i++) {
    Entity *e = objects->items[i];
    e->draw(e, game);
  }
}

/**
 * @brief Renderiza uma nova janela de game over
 * @details Renderiza, de forma centralizada, uma janela de game over,
 * mostrando o `score` e uma instrução para sair do jogo
 * @param[in] score
 */
void render_show_game_over(int score) {
  int height = 10;
  int width = 40;
  int starty = (LINES - height) / 2;
  int startx = (COLS - width) / 2;

  WINDOW *win = newwin(height, width, starty, startx);
  box(win, 0, 0);

  const char *title = "GAME OVER";
  mvwprintw(win, 2, (width - strlen(title)) / 2, "%s", title);
  mvwprintw(win, 4, (width - 20) / 2, "Score Final: %d", score);
  const char *msg = "Pressione qualquer tecla para sair";
  mvwprintw(win, 7, (width - strlen(msg)) / 2, "%s", msg);
  wrefresh(win);
  nodelay(stdscr, FALSE);
  getch();
  delwin(win);
}