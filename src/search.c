#include "../include/search.h"
#include "../include/cobrinha.h"
#include "../include/nob.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdlib.h>
typedef struct {
  Point *neighbours;
  int *delta_x;
  int *delta_y;
} Neighbours;
extern pthread_mutex_t mutex;
extern int game_running;
Neighbours *find_neighbours(const Point p, const Board *board);
Movement delta_to_movement(size_t x, size_t y);
void *search_routine(void *params) {
  SearchParameterList *p = (SearchParameterList *)params;
  pthread_mutex_lock(&mutex);
  MovementsToMake *m = search_with(p->s, p->b, p->f, *p->search);
  pthread_mutex_unlock(&mutex);
  return (void *)m;
}
MovementsToMake *search_with(const Snake *snake, const Board *board,
                             const FoodArray *f, SearchAlgorithm s) {
  return s(snake->head.position, board, f);
}

typedef struct {
  Point *items;
  size_t count;
  size_t capacity;
} Stack;

MovementsToMake *dfs(const Point start, const Board *board,
                     const FoodArray *f) {
  size_t width = board->width;
  int *visited = (int *)calloc(board->height * board->width, sizeof(int));
  MovementsToMake *movs = (MovementsToMake *)calloc(1, sizeof(MovementsToMake));
  nob_da_reserve(movs, 256);
  visited[INDEX(start.x, start.y, width)] = 1;
  Stack s = {0};
  nob_da_reserve(&s, board->capacity);
  nob_da_append(&s, start);
  while (s.count != 0) {
    Point last = nob_da_last(&s);
    Neighbours *neighbours = find_neighbours(last, board);
    for (size_t i = 0; i < 3; i++) {
      Point p = neighbours->neighbours[i];
      if (p.repr == FOOD) {
        goto post_loop;
      } else if (p.repr == PLAYER_BODY) {
        visited[INDEX(p.x, p.y, width)] = 1;
        continue;
      }
      if (!visited[INDEX(p.x, p.y, width)]) {
        nob_da_append(&s, p);
        nob_da_append(movs, delta_to_movement(neighbours->delta_x[i],
                                              neighbours->delta_y[i]));
        visited[INDEX(p.x, p.y, width)] = 1;
      }
    }
  }
post_loop:
  nob_da_free(s);
  NOB_FREE(visited);
  return movs;
}

Neighbours *find_neighbours(const Point p, const Board *board) {
  const int dx[] = {-1, 1, 0, 0};
  const int dy[] = {0, 0, -1, 1};
  Neighbours *ns = (Neighbours *)malloc(sizeof(Neighbours) * 1);
  ns->neighbours = (Point *)malloc(sizeof(Point) * 4);
  ns->delta_x = (int *)malloc(sizeof(int) * 4);
  ns->delta_y = (int *)malloc(sizeof(int) * 4);
  for (int i = 0; i < 3; i++) {
    size_t x = p.x + dx[i];
    if (x < 1) {
      x = board->width - 2;
    } else if (x >= board->width - 2) {
      x = 2;
    }
    size_t y = p.y + dy[i];
    if (y < 1) {
      y = board->height - 2;
    } else if (x >= board->height - 2) {
      y = 2;
    }
    Point p = {.x = x, .y = y, .repr = '*'};
    ns->neighbours[i] = *board_get_point(board, p);
    ns->delta_x[i] = dx[i];
    ns->delta_y[i] = dy[i];
  }
  return ns;
}

Movement delta_to_movement(size_t x, size_t y) {
  switch (x) {
  case 1:
    return RIGHT;
  case -1:
    return LEFT;
  default:
    break;
  }
  switch (y) {
  case -1:
    return UP;
  case 1:
    return DOWN;
  default:
    break;
  }
  NOB_UNREACHABLE("All cases were covered");
}