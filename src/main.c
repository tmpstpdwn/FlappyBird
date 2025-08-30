#include <raylib.h>
#include <stdbool.h>
#include "settings.h"
#include "bg_gnd.h"
#include "pipe.h"
#include "bird.h"
#include "score.h"

typedef enum {
  RUN,
  IDLE
} GameState;

static int window_width, window_height;

static const char *TITLE = "FlappyBird";

static GameState gs = IDLE;
static bool start_screen = true;

static RenderTexture2D target;
static Rectangle src;
static Vector2 origin;
static Rectangle dest;

Texture2D flappy_bird_tx;
Texture2D tap_tx;

static bool check_collision(void) {
  return (
    bird_collision_roof() ||
    bird_collision_gnd() ||
    pipe_collision_bird(bird_get_rect())
  );
}

static void check_score(void) {
  if (pipe_is_passed(bird_get_rect()))
    score_inc();
}

static void state_run(float dt) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    bird_flap();
  }

  pipe_maybe_spawn();
  pipe_move_all(dt);
  bird_apply_gravity(dt);
  bird_animate(dt);

  if (check_collision()) {
    gs = IDLE;
  } else {
    check_score();
  }
} 

static void reset(void) {
  bird_reset();
  score_reset();
  pipe_reset();
}

static void state_idle(float dt) {
  if (start_screen) {
    bird_animate(dt);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      gs = RUN;
      start_screen = false;
      pipe_spawn();
    }

  } else {
    bool pipe_is_visible = pipe_visible();
    bool bird_is_visible = bird_visible();

    if (pipe_is_visible)
      pipe_move_all(dt);

    if (bird_is_visible) {
      if (bird_collision_gnd()) {
        bird_scroll(dt);
      } else {
        bird_apply_gravity(dt);
      }
    }

    if (!pipe_is_visible && !bird_is_visible) {
      reset();
      start_screen = true;
    }
  }
}

void start_screen_load_assets(void) {
  flappy_bird_tx = LoadTexture("sprites/flappy-bird.png");
  tap_tx = LoadTexture("sprites/tap.png");
}

void start_screen_unload_assets(void) {
  UnloadTexture(flappy_bird_tx);
  UnloadTexture(tap_tx);
}

void start_screen_draw(void) {
  DrawTexture(flappy_bird_tx, (WIDTH - flappy_bird_tx.width)/2, HEADER_Y, WHITE);
  DrawTexture(tap_tx, (WIDTH - tap_tx.width)/2, HEIGHT/4 + (HEIGHT - tap_tx.height)/2, WHITE);
}

static void render(void) {
  BeginTextureMode(target);
  ClearBackground(RAYWHITE);

  bg_draw();
  gnd_draw();

  if (start_screen)
    start_screen_draw();
  else {
    pipe_draw_all();
    score_draw();
  }

  bird_draw();
    
  EndTextureMode();

  BeginDrawing();
  DrawTexturePro(target.texture, src, dest, origin, 0, WHITE);
  EndDrawing();
}

static void load_assets(void) {
  bg_gnd_load_assets();
  start_screen_load_assets();
  score_load_assets();
  pipe_load_assets();
  bird_load_assets();
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
  src = (Rectangle){0, 0, WIDTH, -HEIGHT};
  origin = (Vector2){0, 0};
  dest = (Rectangle){0, 0, window_width, window_height};

  load_assets();
}

void init_game(void) {
  Time t = GetRandomValue(DAY, NIGHT);
  bg_set_tx(t);
  pipe_set_tx(t);
  bird_set_tx();
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
    gnd_update(dt);
    render();
  }
} 

static void unload_assets(void) {
  bg_gnd_unload_assets();
  start_screen_unload_assets();
  score_unload_assets();
  pipe_unload_assets();
  bird_unload_assets();
}

static void end_renderer(void) {
  UnloadRenderTexture(target);
  unload_assets();
  CloseWindow();
}

int main() {
  init_renderer();
  init_game();
  game_loop();
  end_renderer();
  return 0;
}
