#include <raylib.h>
#include "settings.h"
#include "bg_gnd.h"

/*
BG
--

BG is static.

GROUND
------

- Two ground textures are drawn side by side and
  moved along the horizontal direction to the left to
  make them scroll.
- It starts with the first one beign on the screen and
  the second one out to the right of the screen.
- with time the first one moves out of the screen to the left.
- once the first one is completely out of the screen to the
  left of the screen, the positions of the 2 ground
  textures are reset to they're initial ones.

This creates the illution of an endless scrolling ground. 
*/

static Texture2D bg_tx[NIGHT + 1];
static Time curr_bg_tx = DAY;

static Texture2D gnd_tx;
static float x_offset = 0;

void bg_gnd_load_assets(void) {
  bg_tx[DAY] = LoadTexture("sprites/background-day.png");
  bg_tx[NIGHT] = LoadTexture("sprites/background-night.png");
  gnd_tx = LoadTexture("sprites/base.png");
}

void bg_set_tx(Time t) {
  curr_bg_tx = t;
}

void gnd_update(float dt) {
  x_offset -= SCROLL_SPEED * dt;
  if (x_offset <= -WIDTH)
    x_offset = 0;
}

void bg_draw(void) {
  DrawTexture(bg_tx[curr_bg_tx], 0, 0, WHITE);
}

void gnd_draw(void) {
  DrawTexture(gnd_tx, x_offset, GROUND_Y, WHITE);
  DrawTexture(gnd_tx, x_offset + WIDTH, GROUND_Y, WHITE);
}

void bg_gnd_unload_assets(void) {
  UnloadTexture(bg_tx[DAY]);
  UnloadTexture(bg_tx[NIGHT]);
  UnloadTexture(gnd_tx);
}
