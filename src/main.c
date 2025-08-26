#include <raylib.h>
#include <stdbool.h>
#include "settings.h"
#include "pipe.h"
#include "bird.h"

// Game state enum.
typedef enum {
  RUN,
  IDLE
} GameState;

// Title.
static const char *TITLE = "Floopy";

// Bird and ground rectangles.
static Rectangle Bird = {0};
static Rectangle Ground = {0, HEIGHT - GROUND_HEIGHT, WIDTH, GROUND_HEIGHT};

static int score = 0;
static bool resetted = false; // Game only starts when this is true.
static GameState gs = IDLE;

// Check collision bw roof, ground and pipes then return true/false.
static bool check_collision(void) {
  return (
    Bird.y < 0 ||
    Bird.y + Bird.height >= Ground.y ||
    CheckCollisionRecs(Bird, pipes[next_pipe].top) ||
    CheckCollisionRecs(Bird, pipes[next_pipe].bottom)
  );
}

// Count score when a bird successfully goes through a pipe.
static void check_score(void) {
  if (Bird.x > pipes[next_pipe].top.x + PIPE_WIDTH) {
    score++;
    next_pipe = (next_pipe + 1) % MAX_PIPES;
  }
}

// RUN state.
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

// Reset game variables.
static void reset(void) {
  Bird = (Rectangle){BIRD_STARTX, BIRD_STARTY, BIRD_SIZE, BIRD_SIZE};
  score = 0;
  bird_speed = 0;
  curr_pipe = 0;
  next_pipe = 0;
  spawn_pipe();
}

// IDLE state.
static void state_idle(float dt) {
  /* Before allowing the user to start the game
   * clear all objects from the screen (Pipes, bird).
   * Once that's done, reset game variables and now
   * recieve user input to start the game.
  */
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

// Render the frame.
static void render(void) {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  draw_pipes();
  DrawRectangleRec(Bird, BLUE);
  DrawRectangleRec(Ground, GREEN);
  DrawText(TextFormat("Score: %d", score), 10, 10, 40, BLACK);
  EndDrawing();
}

// Initialize the renderer.
static void init_renderer(void) {
  InitWindow(WIDTH, HEIGHT, TITLE);
  SetTargetFPS(FPS);
}

// Main game loop.
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

// End renderer.
static void end_renderer(void) {
  CloseWindow();
}

// MAIN.
int main() {
  init_renderer();
  game_loop();
  end_renderer();
  return 0;
}
