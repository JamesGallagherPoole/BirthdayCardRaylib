#ifndef LETTER_H
#define LETTER_H

#include "animation.h"
#include "raylib.h"
#include "utils.h"

typedef struct {
  Texture2D envelopeTexture;
  Vector2 pos;
  float slideSpeed;
  ShowState showState;
  Animation *animation;
} Letter;

Letter *CreateLetter(void);
void FreeLetter(Letter *letter);
void UpdateLetter(Letter *letter);
void DrawLetter(Letter *letter);

#endif
