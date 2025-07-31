#ifndef CARD_H
#define CARD_H

#include "arena.h"
#include "raylib.h"
#include "types.h"

typedef struct {
  CardType cardType;
  CardData cardData;
  Texture2D cardTexture;
} CardParams;

Card *CreateCard(Arena *arena, CardParams cardParams);
void UpdateCard(Letter *letter, Card *card);
void DrawCard(Letter *letter, Card *card);

#endif
