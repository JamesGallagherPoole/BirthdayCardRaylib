#include "card.h"
#include "raylib.h"
#include "utils.h"

Card *CreateCard(Arena *arena, CardParams cardParams) {
  Card *card = arena_alloc(arena, sizeof(Card));

  card->cardType = cardParams.cardType;
  card->cardData = cardParams.cardData;
  card->texture = cardParams.cardTexture;
  card->pos = (Vector2){0, 0};
  card->showState = INVISIBLE;
  card->isFinished = false;

  return card;
}

void UpdateCard(Letter *letter, Card *card) {
  if (card->showState == DONE) {
    return;
  }

  switch (card->cardType) {
  case CARD_ENVELOPE:

    break;
  case CARD_TEXT:
    if (!card->isFinished) {
      card->isFinished = true;
    }
    break;
  case CARD_IMAGE:
    break;
  }

  switch (card->showState) {

  case INVISIBLE:
  case ENTER:
    break;
  case VISIBLE:

  case EXIT:
    // Animate Out
  case DONE:
    break;
  }
}

void DrawCard(Letter *letter, Card *card) {}
