#include "letter.h"
#include "animation.h"
#include "card.h"
#include "custom_arrays.h"
#include "raylib.h"
#include "types.h"
#include "utils.h"
#include <stddef.h>
#include <string.h>

Letter *CreateLetter(Arena *arena) {
  size_t numberOfCards = 3;
  Letter *letter = arena_alloc(arena, sizeof(Letter));

  letter->pos = (Vector2){0, -200};
  letter->slideSpeed = 400;
  letter->showState = INVISIBLE;
  letter->current_card_index = 0;
  letter->cards = CardArray_Create(arena, numberOfCards);

  letter->numberOfCards = numberOfCards;
  AnimationParams animationParams = {{0, 0, 200, 120}, 26, 0.03, false};
  letter->animation = CreateAnimation(arena, animationParams);

  // Envelope
  CardData cardData;
  strcpy(cardData.cardEnvelopeData.title, "Happy Birthday");
  strcpy(cardData.cardEnvelopeData.subtitle, "This is a subtitle text");
  CardParams envelopeParams = {CARD_ENVELOPE, cardData,
                               LoadTexture("assets/envelope.png")};

  CardArray_Push(letter->cards, CreateCard(envelopeParams));

  // First Card
  // TODO: Move card background to a shared handle or pointer
  CardData firstCardData;
  strcpy(firstCardData.cardTextData.text, "First Card!");
  CardParams firstCardParams = {CARD_TEXT, firstCardData,
                                LoadTexture("assets/inner_card.png")};
  CardArray_Push(letter->cards, CreateCard(firstCardParams));

  // Second Card
  CardData secondCardData;
  secondCardData.cardImageData.texture = LoadTexture("assets/james.jpg");
  strcpy(secondCardData.cardImageData.text, "This is an image caption");
  CardParams secondCardParams = {CARD_IMAGE, secondCardData,
                                 LoadTexture("assets/inner_card.png")};

  CardArray_Push(letter->cards, CreateCard(secondCardParams));

  return letter;
}

void UpdateLetter(Letter *letter) {
  UpdateAnimation(letter->animation);

  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  Vector2 desired_dimensions = GetScaledUpDimensions(
      200, CardArray_At(letter->cards, 0)->texture.height);

  int center_y = (int)(screen_height / 2) - (int)(desired_dimensions.y / 2);

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
      CardArray_At(letter->cards, 0)->showState = VISIBLE;
    }
    break;
  case VISIBLE:
    UpdateCard(letter, CardArray_At(letter->cards, letter->current_card_index));
    break;
  case EXIT:
    break;
  case DONE:
    break;
  }
}

void DrawLetter(Letter *letter) {
  if (letter->current_card_index < letter->numberOfCards - 1) {
    if (CardArray_At(letter->cards, letter->current_card_index + 1)
            ->showState == ENTER) {
      DrawCard(letter,
               CardArray_At(letter->cards, letter->current_card_index + 1));
    }
  }

  DrawCard(letter, CardArray_At(letter->cards, letter->current_card_index));
}

void DrawLetterUi(Letter *letter) {
  int screenHeight = GetScreenHeight();
  if (CardArray_At(letter->cards, letter->current_card_index)->showState ==
          VISIBLE &&
      CardArray_At(letter->cards, letter->current_card_index)->isFinished) {
    DrawText("Press Space to Continue", 40, screenHeight - 30, 20, DARKGRAY);
  }

  // TODO: Call Card specific Ui
}
