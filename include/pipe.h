#ifndef PIPE_H
#define PIPE_H

#include <raylib.h>
#include "settings.h"

#define PIPE_UP 0
#define PIPE_DOWN 1

typedef struct {
  Rectangle top;
  Rectangle bottom;
  bool alive;
} Pipe;

void pipe_load_assets(void);
void pipe_set_tx(Time t);
void pipe_spawn(void);
void pipe_maybe_spawn(void);
void pipe_move_all(float dt);
bool pipe_collision_bird(Rectangle bird_rect);
bool pipe_is_passed(Rectangle bird_rect);
bool pipe_visible(void);
void pipe_draw_all(void);
void pipe_reset(void);
void pipe_unload_assets(void);

#endif
