#include "../include/cobrinha.h"
#include "../include/nob.h"
Head init_head(int x, int y, char repr, Movement mov);
Body init_body(char repr);
void snake_update(Snake *self, int grow) {
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

Snake *snake_init(int x, int y) {
  Snake *snake = (Snake *)malloc(sizeof(Snake) * 1);
  snake->head = init_head(x, y, PLAYER_HEAD, DOWN);
  snake->body = init_body(PLAYER_BODY);
  return snake;
}

Head init_head(int x, int y, char repr, Movement mov) {
  Head c;
  c.position.y = y;
  c.position.x = x;
  c.repr = repr;
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

Food *food_gen(int height, int width, char repr) {
  Food *f = (Food *)malloc(sizeof(Food));
  srand(time(NULL));
  f->position.y = rand() % height;
  f->position.x = rand() % width;
  f->repr = repr;
  return f;
}