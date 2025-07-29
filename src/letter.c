#include "letter.h"
#include "animation.h"
#include "raylib.h"
#include "utils.h"

Letter *CreateLetter(Arena *arena) {
  Letter *letter = arena_alloc(arena, sizeof(Letter));

  letter->envelopeTexture = LoadTexture("assets/envelope.png");
  letter->pos = (Vector2){0, -200};
  letter->slideSpeed = 400;
  letter->showState = INVISIBLE;

  AnimationParams animationParams = {{0, 0, 200, 120}, 26, 0.03, false};

  letter->animation = CreateAnimation(arena, animationParams);

  return letter;
}

void UpdateLetter(Letter *letter) {
  UpdateAnimation(letter->animation);

  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  int center_y =
      (int)(screen_height / 2) - (int)(letter->envelopeTexture.height / 2);

  letter->pos.x = (int)(screen_width / 2) - (int)(200 / 2);
  switch (letter->showState) {
  case INVISIBLE:
    letter->showState = ENTER;
    break;
  case ENTER:
    if (letter->pos.y < center_y) {
      letter->pos.y += letter->slideSpeed * GetFrameTime();
    } else {
      letter->showState = VISIBLE;
    }
    break;
  case VISIBLE:
    if (IsKeyPressed(KEY_SPACE)) {

      PlayAnimation(letter->animation);
    }

    if (letter->animation->current_frame > 0 &&
        !letter->animation->is_playing) {
      letter->showState = EXIT;
    }
    break;
  case EXIT:
    if (letter->pos.y < screen_height + 200) {
      letter->pos.y += letter->slideSpeed * GetFrameTime();
    } else {
      letter->showState = DONE;
    }
    break;
  case DONE:
    break;
  }
}

void DrawLetter(Letter *letter) {
  DrawTextureRec(letter->envelopeTexture, letter->animation->frame_rec,
                 letter->pos, WHITE);
}
