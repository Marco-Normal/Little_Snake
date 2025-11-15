#include "../include/cobrinha.h"
#include "../include/nob.h"
#include <ncurses.h>
#include <pthread.h>
#include <semaphore.h>
extern GameState state;
Head init_head(size_t x, size_t y, Movement mov);
Body init_body(char repr);
int snake_eat_food(Snake *snake, EntityArray *c);
void snake_update(Snake *self, EntityArray *board) {
  int grow = snake_eat_food(self, board);
  Point old_head = self->head.position;
  snake_movement(self);
  Point prev_pos = old_head;
  for (size_t i = 0; i < self->body.count; ++i) {
    Point current_pos = self->body.items[i];
    self->body.items[i] = prev_pos;
    prev_pos = current_pos;
  }
  if (grow) {
    nob_da_append(&self->body, prev_pos);
  }
}

int snake_check_self_collision(Snake *self) {
  nob_da_foreach(Point, p, &self->body) {
    if (p->x == self->head.position.x && p->y == self->head.position.y) {
      return 1;
    }
  }
  return 0;
}

int snake_check_collision(void *_self, Point *with) {
  Snake *self = (Snake *)_self;
  nob_da_foreach(Point, p, &self->body) {
    if (p->x == with->x && p->y == with->y) {
      return 1;
    }
  }
  return (self->head.position.x == with->x) &&
         (self->head.position.y == with->y);
}

void snake_draw(void *_self, WINDOW *at) {
  Snake *self = (Snake *)_self;
  int h, w;
  getmaxyx(at, h, w);
  snake_check_bounds(self, h - 2, w - 2);
  nob_da_foreach(Point, p, &self->body) {
    mvwaddch(at, p->y, p->x, self->body.repr);
  }
  mvwaddch(at, self->head.position.y, self->head.position.x, self->repr);
  return;
}

Snake *snake_init(size_t x, size_t y) {
  Snake *snake = (Snake *)malloc(sizeof(Snake) * 1);
  snake->head = init_head(x, y, DOWN);
  snake->body = init_body(PLAYER_BODY);
  snake->repr = PLAYER_HEAD;
  snake->draw = (snake_draw);
  snake->collision = (snake_check_collision);
  snake->type = SNAKE;
  return snake;
}

Head init_head(size_t x, size_t y, Movement mov) {
  Head c;
  c.position.y = y;
  c.position.x = x;
  c.movement = mov;
  return c;
}

Body init_body(char repr) {
  Body b;
  b.repr = repr;
  b.capacity = 0;
  b.count = 0;
  b.items = NULL;
  return b;
}

void snake_movement(Snake *self) {
  switch (self->head.movement) {
  case UP:
    self->head.position.y--;
    break;
  case DOWN:
    self->head.position.y++;
    break;
  case RIGHT:
    self->head.position.x++;
    break;
  case LEFT:
    self->head.position.x--;
    break;
  }
}

void snake_change_direction(Snake *self, char command) {
  switch (command) {
  case 'w':
    self->head.movement = UP;
    break;
  case 's':
    self->head.movement = DOWN;
    break;
  case 'a':
    self->head.movement = LEFT;
    break;
  case 'd':
    self->head.movement = RIGHT;
    break;
  default:
    break;
  }
}

void snake_check_bounds(Snake *self, size_t height, size_t width) {
  self->head.position.y = (self->head.position.y + height) % (height);
  self->head.position.x = (self->head.position.x + width) % (width);
}

int snake_eat_food(Snake *snake, EntityArray *board) {
  pthread_mutex_lock(&state.mutex);
  for (size_t i = 0; i < board->count; i++) {
    Entity *d = board->items[i];
    if (d->type == SNAKE) {
      continue;
    }
    if (d->collision(d, &snake->head.position)) {
      nob_da_remove_unordered(board, i);
      sem_post(&state.n_food);
      state.score++;
      pthread_mutex_unlock(&state.mutex);
      return 1;
    }
  }
  pthread_mutex_unlock(&state.mutex);
  return 0;
}
