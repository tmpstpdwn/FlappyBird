#include <raylib.h>
#include <stdbool.h>
#include "settings.h"
#include "pipe.h"
 
Pipe pipes[MAX_PIPES] = {0};
int curr_pipe = 0;
int next_pipe = 0;

void spawn_pipe(void) {
  int h = GetRandomValue(PIPE_MIN_Y, PIPE_MAX_Y);
  Pipe *curr_ppipe = &pipes[curr_pipe];
  curr_ppipe->top = (Rectangle){WIDTH, 0, PIPE_WIDTH, h};
  curr_ppipe->bottom = (Rectangle){WIDTH, curr_ppipe->top.height + PIPE_V_GAP,
     PIPE_WIDTH, HEIGHT - curr_ppipe->top.height - PIPE_V_GAP - GROUND_HEIGHT};
  curr_ppipe->alive = true;
}

void maybe_spawn_pipe(void) {
  if (pipes[curr_pipe].top.x + PIPE_WIDTH < WIDTH - PIPE_H_GAP) {
    curr_pipe = (curr_pipe + 1) % MAX_PIPES;
    spawn_pipe();
  }
}

void move_pipes(float dt) {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i].alive) {
      pipes[i].top.x -= SCROLL_SPEED * dt;
      pipes[i].bottom.x = pipes[i].top.x;
      if (pipes[i].top.x + pipes[i].top.width < 0)
        pipes[i].alive = false;
    }
  }
}

void draw_pipes(void) {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i].alive) {
      DrawRectangleRec(pipes[i].top, BLACK);
      DrawRectangleRec(pipes[i].bottom, BLACK);
    }
  }
} 
