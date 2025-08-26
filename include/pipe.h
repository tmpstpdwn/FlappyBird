#ifndef PIPE_H
#define PIPE_H

#include <raylib.h>
#include <stdbool.h>
#include "settings.h"

typedef struct {
  Rectangle top;
  Rectangle bottom;
  bool alive;
} Pipe;

extern Pipe pipes[MAX_PIPES];
extern int curr_pipe;
extern int next_pipe;

void spawn_pipe(void);
void maybe_spawn_pipe(void);
void move_pipes(float dt);
void draw_pipes(void);

#endif
