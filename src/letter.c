#include "letter.h"
#include "animation.h"
#include "card.h"
#include "raylib.h"
#include "types.h"
#include "utils.h"
#include <stddef.h>
#include <string.h>

Letter *CreateLetter(Arena *arena) {
  size_t numberOfCards = 2;
  Letter *letter =
      arena_alloc(arena, sizeof(Letter) + sizeof(Card *) * numberOfCards);

  letter->pos = (Vector2){0, -200};
  letter->slideSpeed = 400;
  letter->showState = INVISIBLE;
  letter->current_card_index = 0;

  letter->numberOfCards = numberOfCards;
  AnimationParams animationParams = {{0, 0, 200, 120}, 26, 0.03, false};
  letter->animation = CreateAnimation(arena, animationParams);

  // Envelope
  CardData cardData;
  strcpy(cardData.cardEnvelopeData.title, "Happy Birthday");
  strcpy(cardData.cardEnvelopeData.subtitle, "This is a subtitle text");
  CardParams envelopeParams = {CARD_ENVELOPE, cardData,
                               LoadTexture("assets/envelope.png")};
  letter->cards[0] = CreateCard(arena, envelopeParams);

  // First Card
  CardData firstCardData;
  strcpy(firstCardData.cardTextData.text, "First Card!");
  CardParams firstCardParams = {CARD_TEXT, firstCardData,
                                LoadTexture("assets/inner_card.png")};
  letter->cards[1] = CreateCard(arena, firstCardParams);

  return letter;
}

void UpdateLetter(Letter *letter) {
  UpdateAnimation(letter->animation);

  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  int center_y =
      (int)(screen_height / 2) - (int)(letter->cards[0]->texture.height / 2);

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
      // Set the first card (envelope) as visible
      letter->cards[0]->showState = VISIBLE;
    }
    break;
  case VISIBLE:
    UpdateCard(letter, letter->cards[letter->current_card_index]);
    break;
  case EXIT:
    break;
  case DONE:
    break;
  }
}

void DrawLetter(Letter *letter) {
  if (letter->current_card_index < letter->numberOfCards - 1) {
    if (letter->cards[letter->current_card_index + 1]->showState == ENTER) {
      DrawCard(letter, letter->cards[letter->current_card_index + 1]);
    }
  }

  DrawCard(letter, letter->cards[letter->current_card_index]);
}
