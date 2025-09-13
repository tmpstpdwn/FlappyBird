#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include "settings.h"
#include "score.h"

/*
- The score is rendered to a RenderTexture2D (offscreen buffer)
  whose width spans the entire WIDTH and height spans the height of
  a single digit texture.
- This RenderTexture2D is only updated when score_box_update is called.
- In the game, this RenderTexture2D is drawn every frame but is only updated
  when the score have changed. 
*/

static unsigned int score = 0;
static Texture2D score_tx[SCORE_TEXTURES];
static RenderTexture2D score_box;

static void score_box_update(void) {
  const char *score_str = TextFormat("%d", score);
  int num_digits = strlen(score_str);

  int total_width = 0;
  for (int i = 0; i < num_digits; i++)
    total_width += score_tx[score_str[i] - '0'].width;
  int x = (WIDTH - total_width) / 2;

  BeginTextureMode(score_box);
  ClearBackground(BLANK);

  int x_offset = 0;
  for (int i = 0; i < num_digits; i++) {
    int digit = score_str[i] - '0';
    DrawTexture(score_tx[digit], x + x_offset, 0, WHITE);
    x_offset += score_tx[digit].width + SCORE_SPACING;
  }

  EndTextureMode();
}

void score_load_assets(void) {
  for (int i = 0; i < SCORE_TEXTURES; i++)
    score_tx[i] = LoadTexture(TextFormat("sprites/%d.png", i));
  score_box = LoadRenderTexture(WIDTH, score_tx[0].height);
  score_box_update();
}

void score_inc(void) {
  score++;
  score_box_update();
}

void score_reset(void) {
  score = 0;
  score_box_update();
}

void score_draw(void) {
  Rectangle source = {
    0, 0,
    (float)score_box.texture.width,
    -(float)score_box.texture.height
  };
  Vector2 position = { (WIDTH - score_box.texture.width)/2, HEADER_Y};
  DrawTextureRec(score_box.texture, source, position, WHITE);
}

void score_unload_assets(void) {
  for (int i = 0; i < SCORE_TEXTURES; i++)
    UnloadTexture(score_tx[i]);
  UnloadRenderTexture(score_box);
}
