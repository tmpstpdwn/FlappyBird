#ifndef BIRD_H
#define BIRD_H

#include <raylib.h>

extern float bird_speed;

void bird_apply_gravity(Rectangle *bird, Rectangle *floor, float dt);

#endif
