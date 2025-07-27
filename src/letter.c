#include "letter.h"
#include "raylib.h"
#include "stdlib.h"
#include "utils.h"

Letter *CreateLetter(void) {
  Letter *letter = malloc(sizeof(Letter));

  letter->envelopeTexture = LoadTexture("assets/envelope.png");
  letter->pos = (Vector2){0, -200};
  letter->ShowState = INVISIBLE;

  return letter;
}

void FreeLetter(Letter *letter) { free(letter); }

void UpdateLetter(Letter *letter) {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  int center_y =
      (int)(screen_height / 2) - (int)(letter->envelopeTexture.height / 2);

  letter->pos.x = (int)(screen_width / 2) - (int)(200 / 2);
  switch (letter->ShowState) {
  case INVISIBLE:
    letter->ShowState = ENTER;
    break;
  case ENTER:
    if (letter->pos.y < center_y) {
      letter->pos.y += 200 * GetFrameTime();
    } else {
      letter->ShowState = VISIBLE;
    }
    break;
  case VISIBLE:
    if (IsKeyPressed(KEY_SPACE)) {
      letter->ShowState = EXIT;
    }
    break;
  case EXIT:
    if (letter->pos.y < screen_height + 200) {
      letter->pos.y += 200 * GetFrameTime();
    } else {
      letter->ShowState = DONE;
    }
    break;
  case DONE:
    break;
  }
}

void DrawLetter(Letter *letter) {
  DrawTextureRec(letter->envelopeTexture,
                 (Rectangle){0, 0, 200, letter->envelopeTexture.height},
                 letter->pos, WHITE);
}
