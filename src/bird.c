#include <raylib.h>
#include "settings.h"
#include "bird.h"

// Vertical speed of the bird.
float bird_speed = 0;

// Apply gravity to the bird.
void bird_apply_gravity(Rectangle *bird, Rectangle *floor, float dt) {
  bird->y += bird_speed * dt;
  if (bird->y + bird->height > floor->y) {
    bird->y = floor->y - bird->height;
    bird_speed = 0;
  } else {
    bird_speed += GRAVITY * dt;
  }
}
