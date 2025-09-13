#include <raylib.h>
#include "settings.h"
#include "bird.h"

static Texture2D bird_tx[BIRDS][BIRD_SPRITES];
static int bird = 0; // Bird to use (blue, red, yellow).
static float bird_speed = 0; // Bird vertical speed.
static float bird_sprite = 0; // Current bird sprite (downflap, midflap, upflap).
static Rectangle bird_rect = (Rectangle){BIRD_STARTX, BIRD_STARTY, BIRD_WIDTH, BIRD_HEIGHT};

void bird_load_assets(void) {
  char *birds[BIRDS] = {"bluebird", "redbird", "yellowbird"};
  char *sprites[BIRD_SPRITES] = {"downflap", "midflap", "upflap"};
  for (int bird = 0; bird < BIRDS; bird++) {
    for (int sprite = 0; sprite < BIRD_SPRITES; sprite++) {
      bird_tx[bird][sprite] = LoadTexture(TextFormat("sprites/%s-%s.png", birds[bird], sprites[sprite]));
    }
  }
}

void bird_set_tx(void) {
  bird = GetRandomValue(0, BIRDS - 1);
}

Rectangle bird_get_rect(void) {
  return bird_rect;
}

void bird_apply_gravity(float dt) {
  bird_rect.y += bird_speed * dt;
  if (bird_rect.y + bird_rect.height > GROUND_Y) {
    bird_rect.y = GROUND_Y - bird_rect.height;
    bird_speed = 0;
  } else {
    bird_speed += GRAVITY * dt;
  }
}

void bird_flap(void) {
  bird_speed = FLAP;
}

void bird_scroll(float dt) {
  bird_rect.x -= SCROLL_SPEED * dt;
}

bool bird_collision_roof(void) {
  return  bird_rect.y < 0;
}

bool bird_collision_gnd(void) {
  return bird_rect.y + bird_rect.height >= GROUND_Y;
}

void bird_animate(float dt) {
  bird_sprite += BIRD_ANIMATION_SPEED * dt;
  bird_sprite = (bird_sprite >= 3)? 0: bird_sprite;
}

bool bird_visible(void) {
  return (bird_rect.x + bird_rect.width > 0);
}

void bird_draw(void) {
  DrawTexture(bird_tx[bird][(int)bird_sprite], bird_rect.x, bird_rect.y, WHITE);
}

void bird_reset(void) {
  bird_rect = (Rectangle){BIRD_STARTX, BIRD_STARTY, BIRD_WIDTH, BIRD_HEIGHT};
}

void bird_unload_assets(void) {
  for (int bird = 0; bird < BIRDS; bird++) {
    for (int sprite = 0; sprite < BIRD_SPRITES; sprite++) {
      UnloadTexture(bird_tx[bird][sprite]);
    }
  }
}
