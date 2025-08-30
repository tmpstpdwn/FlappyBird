#ifndef SETTINGS_H
#define SETTINGS_H

typedef enum {
  DAY,
  NIGHT
} Time;

#define W_HEIGHT_PERC 0.8f

#define FPS 60

#define HEIGHT 512
#define WIDTH 288

#define HEADER_Y 25

#define PIPE_WIDTH 52
#define PIPE_HEIGHT 320
#define PIPE_H_GAP 180
#define PIPE_V_GAP 100
#define PIPE_MAX_H PIPE_HEIGHT
#define PIPE_MIN_H 30
#define MAX_PIPES 2
 
#define SCROLL_SPEED   150
#define GROUND_HEIGHT 62 // (HEIGHT - PIPE_HEIGHT - PIPE_V_GAP - PIPE_MIN_H)
#define GROUND_Y 450 // (HEIGHT - GROUND_HEIGHT)

#define SCORE_PADDING 2

#define GRAVITY 2400

#define BIRD_WIDTH 34
#define BIRD_HEIGHT 24
#define BIRD_STARTX 72 // (WIDTH / 4)
#define BIRD_STARTY 256 // (HEIGHT / 2)
#define FLAP -500
#define BIRD_ANIMATION_SPEED 5

#endif
