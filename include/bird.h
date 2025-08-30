#ifndef BIRD_H
#define BIRD_H

#include <raylib.h>

#define BIRDS 3
#define BIRD_SPRITES 3

void bird_load_assets(void);
void bird_set_tx(void);
Rectangle bird_get_rect(void);
void bird_apply_gravity(float dt);
void bird_flap(void); 
void bird_scroll(float dt);
bool bird_collision_roof(void);
bool bird_collision_gnd(void);
void bird_animate(float dt);
bool bird_visible(void);
void bird_draw(void);
void bird_reset(void);
void bird_unload_assets(void);

#endif
