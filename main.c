#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>

#define HEIGHT 800
#define WIDTH 600

#define SCROLL_SPEED   300
#define GROUND_HEIGHT  100

#define PIPE_WIDTH 50
#define PIPE_H_GAP 300
#define PIPE_V_GAP 200
#define PIPE_MIN_Y 50
#define PIPE_MAX_Y (HEIGHT - PIPE_V_GAP - PIPE_MIN_Y - GROUND_HEIGHT)
#define MAX_PIPES (WIDTH / (PIPE_H_GAP - PIPE_WIDTH))

#define BIRD_SIZE 50
#define GRAVITY 2500
#define FLAP -700

typedef struct {
  Rectangle top;
  Rectangle bottom;
  bool alive;
} Pipe;
 
typedef enum {
  START,
  RUN,
  OVER
} GameState;

static float speed = 0;

static Pipe pipes[MAX_PIPES] = {0};
static int curr_pipe = 0;
static int next_pipe = 0;

static Rectangle Bird = {(float)WIDTH/4, (float)HEIGHT/3, BIRD_SIZE, BIRD_SIZE};
static Rectangle Ground = {0, HEIGHT - GROUND_HEIGHT, WIDTH, GROUND_HEIGHT};

static int score = 0;

static GameState gs = START;

static void apply_gravity(Rectangle *obj, Rectangle *floor, float dt) {
  obj->y += speed * dt;
  if (obj->y + obj->height > floor->y) {
    obj->y = floor->y - obj->height;
    speed = 0;
  } else {
    speed += GRAVITY * dt;
  }
}

static void draw_pipes(void) {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i].alive) {
      DrawRectangleRec(pipes[i].top, PINK);
      DrawRectangleRec(pipes[i].bottom, PINK);
    }
  }
} 

static void spawn_pipe(void) {
  int h = GetRandomValue(PIPE_MIN_Y, PIPE_MAX_Y);
  Pipe *curr_ppipe = &pipes[curr_pipe];
  curr_ppipe->top.x = WIDTH;
  curr_ppipe->top.width = PIPE_WIDTH;
  curr_ppipe->top.height = h;

  curr_ppipe->bottom.x = curr_ppipe->top.x;
  curr_ppipe->bottom.y = curr_ppipe->top.height + PIPE_V_GAP;
  curr_ppipe->bottom.width = PIPE_WIDTH;
  curr_ppipe->bottom.height = HEIGHT - curr_ppipe->top.height - PIPE_V_GAP - GROUND_HEIGHT;
  
  curr_ppipe->alive = true;
}

static void move_pipes(float dt) {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i].alive) {
      pipes[i].top.x -= SCROLL_SPEED * dt;
      pipes[i].bottom.x = pipes[i].top.x;
      if (pipes[i].top.x + pipes[i].top.width < 0)
        pipes[i].alive = false;
    }
  }
}

int main() {
  InitWindow(WIDTH, HEIGHT, "flappybird");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    if (gs == START) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        gs = RUN;
        spawn_pipe();
      }
      
    } else if (gs == RUN) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        speed = FLAP;
      }

      apply_gravity(&Bird, &Ground, dt);

      if (pipes[curr_pipe].top.x + PIPE_WIDTH < WIDTH - PIPE_H_GAP) {
        curr_pipe = (curr_pipe + 1) % MAX_PIPES;
        spawn_pipe();
      }

      move_pipes(dt);

      bool collided = false;
      if (pipes[next_pipe].alive)  {
        if (CheckCollisionRecs(Bird, pipes[next_pipe].top) ||
            CheckCollisionRecs(Bird, pipes[next_pipe].bottom)) {
          collided = true;
        } else if (Bird.x > pipes[next_pipe].top.x + PIPE_WIDTH) {
          score++;
          next_pipe = (next_pipe + 1) % MAX_PIPES;
        }
      }

      if (Bird.y + Bird.height >= Ground.y ||
          Bird.y < 0 || collided) {
          gs = OVER;
      }

    } else {
      if (pipes[curr_pipe].top.x + PIPE_WIDTH > 0)
        move_pipes(dt);
      else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Bird.x = (float)WIDTH/4;
        Bird.y = (float)HEIGHT/3;
        score = 0;
        curr_pipe = 0;
        next_pipe = 0;
        gs = RUN;
        spawn_pipe();
      }

      if (Bird.y + Bird.height == Ground.y) {
        if (Bird.x + Bird.width > 0)
          Bird.x -= SCROLL_SPEED * dt;
      } else {
        apply_gravity(&Bird, &Ground, dt);
      }
      
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_pipes();
    DrawRectangleRec(Bird, BLUE);
    DrawRectangleRec(Ground, BLACK);
    DrawText(TextFormat("Score: %d", score), 10, 10, 40, BLACK);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
