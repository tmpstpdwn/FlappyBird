#ifndef SETTINGS_H
#define SETTINGS_H

#define FPS 60

#define HEIGHT 800
#define WIDTH 500

#define SCROLL_SPEED   300
#define GROUND_HEIGHT  100

#define PIPE_WIDTH 50
#define PIPE_H_GAP 300
#define PIPE_V_GAP 200
#define PIPE_MIN_Y 50
#define PIPE_MAX_Y (HEIGHT - PIPE_V_GAP - PIPE_MIN_Y - GROUND_HEIGHT)
#define MAX_PIPES ((WIDTH / (PIPE_H_GAP + PIPE_WIDTH)) + 1)

#define GRAVITY 2500

#define BIRD_SIZE 50
#define BIRD_STARTX (WIDTH / 4)
#define BIRD_STARTY (HEIGHT / 3)
#define FLAP -700

#endif
