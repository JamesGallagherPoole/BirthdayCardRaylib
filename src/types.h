#ifndef TYPES_H
#define TYPES_H

#include "animation.h"
#include "raylib.h"
#include "utils.h"

/*
 * Cards
 */
typedef enum {
  CARD_ENVELOPE,
  CARD_TEXT,
  CARD_IMAGE,
} CardType;

typedef struct {
  char text[256];
} CardTextData;

typedef struct {
  char title[128];
  char subtitle[128];
} CardEnvelopeData;

typedef union {
  CardTextData cardTextData;
  CardEnvelopeData cardEnvelopeData;
} CardData;

typedef struct {
  Texture2D texture;
  Vector2 pos;
  CardType cardType;
  CardData cardData;
  ShowState showState;
  bool isFinished;
} Card;

/*
 * Letter
 */
typedef struct {
  Vector2 pos;
  float slideSpeed;
  ShowState showState;
  Animation *animation; // TODO: To be moved inside the envelope data
  int current_card_index;
  int numberOfCards;
  Card *cards[];
} Letter;

#endif
