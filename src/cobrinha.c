#include "../include/cobrinha.h"
#include "../include/nob.h"
#include <ncurses.h>

Board b = {0};
Board c = {0};
Head init_head(int x, int y, Movement mov);
Body init_body(char repr);
int snake_eat_food(Snake *snake, Board *c);
void snake_update(Snake *self) {
  int grow = snake_eat_food(self, &c);
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

int snake_check_collision(Snake *self) {
  nob_da_foreach(Point, p, &self->body) {
    if (p->x == self->head.position.x && p->y == self->head.position.y) {
      return 1;
    }
  }
  return 0;
}

void snake_draw(void *_self, WINDOW *at) {
  Snake *self = (Snake *)_self;
  int h, w;
  getmaxyx(at, h, w);
  snake_check_bounds(self, h, w);
  nob_da_foreach(Point, p, &self->body) {
    mvwaddch(at, p->y, p->x, self->body.repr);
  }
  mvwaddch(at, self->head.position.y, self->head.position.x, self->repr);
  return;
}

Snake *snake_init(int x, int y) {
  Snake *snake = (Snake *)malloc(sizeof(Snake) * 1);
  snake->head = init_head(x, y, DOWN);
  snake->body = init_body(PLAYER_BODY);
  snake->repr = PLAYER_HEAD;
  snake->draw = (snake_draw);
  return snake;
}

Head init_head(int x, int y, Movement mov) {
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

void snake_check_bounds(Snake *self, int height, int width) {
  if (self->head.position.y < 1)
    self->head.position.y = height - 2;
  if (self->head.position.y >= height - 1)
    self->head.position.y = 1;
  if (self->head.position.x < 1)
    self->head.position.x = width - 2;
  if (self->head.position.x >= width - 1)
    self->head.position.x = 1;
}

void food_draw(void *_self, WINDOW *at) {
  Food *self = (Food *)_self;
  mvwaddch(at, self->position.y, self->position.x, self->repr);
}

Food *food_gen(int height, int width, char repr) {
  Food *f = (Food *)malloc(sizeof(Food));
  srand(time(NULL));
  int x, y;
  while (true) {
    y = rand() % height;
    x = rand() % width;
    nob_da_foreach(Point, p, &b) {
      if (p->x != x && p->y != y) {
        break;
      }
    }
  }
  f->position.x = x;
  f->position.y = y;
  nob_da_append(&c, f->position);
  f->repr = repr;
  f->draw = (food_draw);
  return f;
}

int snake_eat_food(Snake *snake, Board *c) {
  int i = 0;
  nob_da_foreach(Point, p, c) {
    if (snake->head.position.x == p->x && snake->head.position.y == p->y) {
      nob_da_remove_unordered(c, i);
      return 1;
    }
  }
  return 0;
}

Board *board_init(void) {
  Board *b = (Board *)malloc(sizeof(Board));
  b->capacity = 0;
  b->count = 0;
  b->items = NULL;
  return b;
}
