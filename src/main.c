#include <raylib.h>
#include <stdbool.h>
#include <math.h>
#include "settings.h"
#include "pipe.h"
#include "bird.h"

typedef enum {
  RUN,
  IDLE
} GameState;

static int window_width, window_height;

static const char *TITLE = "Floopy";

static Rectangle Bird = {0};
static Rectangle Ground = {0, HEIGHT - GROUND_HEIGHT, WIDTH, GROUND_HEIGHT};

static int score = 0;
static bool resetted = false;
static GameState gs = IDLE;

static RenderTexture2D target;
static Rectangle src = {0, 0, WIDTH, -HEIGHT};
static Vector2 origin = {0, 0};
static Rectangle dest;

static bool check_collision(void) {
  return (
    Bird.y < 0 ||
    Bird.y + Bird.height >= Ground.y ||
    CheckCollisionRecs(Bird, pipes[next_pipe].top) ||
    CheckCollisionRecs(Bird, pipes[next_pipe].bottom)
  );
}

static void check_score(void) {
  if (Bird.x > pipes[next_pipe].top.x + PIPE_WIDTH) {
    score++;
    next_pipe = (next_pipe + 1) % MAX_PIPES;
  }
}

static void state_run(float dt) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    bird_speed = FLAP;
  }

  bird_apply_gravity(&Bird, &Ground, dt);
  maybe_spawn_pipe();
  move_pipes(dt);

  if (check_collision()) {
    gs = IDLE;
  } else {
    check_score();
  }

} 

static void reset(void) {
  Bird = (Rectangle){BIRD_STARTX, BIRD_STARTY, BIRD_WIDTH, BIRD_HEIGHT};
  score = 0;
  bird_speed = 0;
  curr_pipe = 0;
  next_pipe = 0;
  spawn_pipe();
}

static void state_idle(float dt) {
  if (!resetted) {
    bool pipe_visible = pipes[curr_pipe].alive &&
                       (pipes[curr_pipe].top.x + PIPE_WIDTH > 0);
    bool bird_visible = (Bird.x + Bird.width > 0);

    if (pipe_visible)
      move_pipes(dt);

    if (bird_visible) {
      if (Bird.y + Bird.height == Ground.y) {
        Bird.x -= SCROLL_SPEED * dt;
      } else {
        bird_apply_gravity(&Bird, &Ground, dt);
      }
    }

    if (!pipe_visible && !bird_visible) {
      reset();
      resetted = true;
    }

  } else {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      gs = RUN;
      resetted = false;
    }
  }
}

static void render(void) {
  BeginTextureMode(target);
  ClearBackground(RAYWHITE);
  draw_pipes();
  DrawRectangleRec(Bird, BLUE);
  DrawRectangleRec(Ground, GREEN);
  DrawText(TextFormat("Score: %d", score), 10, 10, 25, BLACK);
  EndTextureMode();

  BeginDrawing();
  DrawTexturePro(target.texture, src, dest, origin, 0, WHITE);
  EndDrawing();
}

static void init_renderer(void) {
  InitWindow(0, 0, TITLE);

  int monitor = GetCurrentMonitor();
  int screen_height = GetMonitorHeight(monitor);
  int screen_width = GetMonitorWidth(monitor);

  window_height = screen_height * W_HEIGHT_PERC;
  window_width = window_height * ((float)WIDTH / HEIGHT);

  int x = (screen_width - window_width) / 2;
  int y = (screen_height - window_height) / 2;

  SetWindowSize(window_width, window_height);
  SetWindowPosition(x, y);

  SetTargetFPS(FPS);

  target = LoadRenderTexture(WIDTH, HEIGHT);
  SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
  dest = (Rectangle){0, 0, window_width, window_height};
}

static void game_loop(void) {
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    switch (gs) {
      case RUN:
        state_run(dt);
        break;
      case IDLE:
        state_idle(dt);
        break;
    }
    render();
  }
} 

static void end_renderer(void) {
  UnloadRenderTexture(target);
  CloseWindow();
}

int main() {
  init_renderer();
  game_loop();
  end_renderer();
  return 0;
}
