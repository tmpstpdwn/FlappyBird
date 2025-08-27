#ifndef SETTINGS_H
#define SETTINGS_H

#define W_HEIGHT_PERC 0.8f

#define FPS 60

#define HEIGHT 512
#define WIDTH 288

#define SCROLL_SPEED   150
#define GROUND_HEIGHT  50

#define PIPE_WIDTH 52
#define PIPE_H_GAP 180
#define PIPE_V_GAP 100
#define PIPE_MIN_Y 10
#define PIPE_MAX_Y 352 // (HEIGHT - PIPE_V_GAP - PIPE_MIN_Y - GROUND_HEIGHT)
#define MAX_PIPES 2

#define GRAVITY 2400

#define BIRD_WIDTH 34
#define BIRD_HEIGHT 24
#define BIRD_STARTX 72 // (WIDTH / 4)
#define BIRD_STARTY 170 // (HEIGHT / 3)
#define FLAP -500

#endif
