#include <raylib.h>
#include <stdbool.h>
#include "settings.h"
#include "bg_gnd.h"
#include "pipe.h"
#include "bird.h"
#include "score.h"

/*
The game initially renders to a RenderTexture2D (offscreen buffer) of WIDTH * HEIGHT (defined in settings.h)
and then scales the buffer to window size which is determined at runtime based on monitor size.
*/

/* Gamestates enum.
The game starts in IDLE then -> RUN when started
and then -> IDLE state after GAMEOVER.
This goes on and on...
*/
typedef enum {
  RUN,
  IDLE
} GameState;

static const char *TITLE = "FlappyBird";

// Window width, height to be determined at runtime.
static int window_width, window_height;

// RenderTexture2D stuff.
static RenderTexture2D target;
static Vector2 origin = (Vector2){0, 0};
static Rectangle src = (Rectangle){0, 0, WIDTH, -HEIGHT};
static Rectangle dest;
/*
target, dest will be setup at runtime as they depend on
window width, height which has to be determined at runtime.
*/

// Textures for flappybird icon.
static Texture2D flappy_bird_tx;

// Sounds.
static Sound hit_snd;
static Sound point_snd;
static Sound wing_snd;

// Game context variables.
static GameState gs = IDLE;
static bool start_screen = true;

static bool check_collision(void) {
  return (
    bird_collision_roof() ||
    bird_collision_gnd() ||
    pipe_collision_bird(bird_get_rect())
  );
}

static void check_score(void) {
  if (pipe_is_passed(bird_get_rect())) {
    PlaySound(point_snd);
    score_inc();
  }
}

static void state_run(float dt) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    PlaySound(wing_snd);
    bird_flap();
  }

  pipe_maybe_spawn();
  pipe_move_all(dt);
  bird_apply_gravity(dt);
  bird_animate(dt);

  if (check_collision()) {
    PlaySound(hit_snd);
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

  /*
  Wait till the dead bird and spawned pipes are out of the screen
  before resetting and going to start screen.
  */
  
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

static void start_screen_load_assets(void) {
  flappy_bird_tx = LoadTexture("sprites/flappy-bird.png");
}

static void start_screen_unload_assets(void) {
  UnloadTexture(flappy_bird_tx);
}

static void start_screen_draw(void) {
  DrawTexture(flappy_bird_tx, (WIDTH - flappy_bird_tx.width)/2, HEADER_Y, WHITE);
}

static void render(void) {
  // Render to RenderTexture2D.
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

  // Scale RenderTexture2D to window size.
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

static void load_audio(void) {
  hit_snd = LoadSound("audio/hit.wav");
  point_snd = LoadSound("audio/point.wav");
  wing_snd = LoadSound("audio/wing.wav");
}

static void init_renderer(void) {
  InitWindow(0, 0, TITLE);
  InitAudioDevice();

  int monitor = GetCurrentMonitor();
  int screen_height = GetMonitorHeight(monitor);
  int screen_width = GetMonitorWidth(monitor);

  // Calculate window diamensions.
  window_height = screen_height * W_HEIGHT_PERC;
  window_width = window_height * ((float)WIDTH / HEIGHT);

  // Calculate (x, y) to center the window.
  int x = (screen_width - window_width) / 2;
  int y = (screen_height - window_height) / 2;

  // Resize and Center the window.
  SetWindowSize(window_width, window_height);
  SetWindowPosition(x, y);

  Image icon = LoadImage("FlappyBird.png");
  SetWindowIcon(icon);
  UnloadImage(icon);

  // Limit FPS.
  SetTargetFPS(FPS);

  // Setup RenderTexture2D.
  target = LoadRenderTexture(WIDTH, HEIGHT);
  SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
  dest = (Rectangle){0, 0, window_width, window_height};

  load_assets();
  load_audio();
}

static void init_game(void) {
  /*
  Set a random Background and Bird texture.
  */
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
    gnd_update(dt); // Ground will scroll always regardless of GameState.
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

static void unload_audio(void) {
  UnloadSound(hit_snd);
  UnloadSound(point_snd);
  UnloadSound(wing_snd);
}

static void end_renderer(void) {
  UnloadRenderTexture(target);
  unload_assets();
  unload_audio();
  CloseWindow();
}

int main() {
  init_renderer();
  init_game();
  game_loop();
  end_renderer();
  return 0;
}
