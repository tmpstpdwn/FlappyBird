#include <raylib.h>
#include "settings.h"
#include "bird.h"

float bird_speed = 0;

void bird_apply_gravity(Rectangle *bird, Rectangle *floor, float dt) {
  bird->y += bird_speed * dt;
  if (bird->y + bird->height > floor->y) {
    bird->y = floor->y - bird->height;
    bird_speed = 0;
  } else {
    bird_speed += GRAVITY * dt;
  }
}
