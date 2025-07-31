#include "card.h"
#include "raylib.h"
#include "raymath.h"
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
  int screen_height = GetScreenHeight();

  switch (card->cardType) {
  case CARD_ENVELOPE:
    if (card->showState == VISIBLE) {

      if (letter->current_card_index < letter->numberOfCards - 1) {
        letter->cards[letter->current_card_index + 1]->showState = ENTER;
      }

      if (IsKeyPressed(KEY_SPACE)) {
        PlayAnimation(letter->animation);
      }

      if (letter->animation->current_frame > 0 &&
          !letter->animation->is_playing) {
        card->isFinished = true;
      }
    }

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
    if (card->isFinished) {
      // Press again to dismiss
      if (IsKeyPressed(KEY_SPACE)) {
        card->showState = EXIT;

        if (letter->current_card_index < letter->numberOfCards - 1) {
          letter->cards[letter->current_card_index + 1]->showState = ENTER;
        }
      }
    }
    break;
  case EXIT:
    // Animate Out
    if (card->pos.y < screen_height + 200) {
      card->pos.y += letter->slideSpeed * GetFrameTime();
    } else {
      card->showState = DONE;

      if (letter->current_card_index < letter->numberOfCards - 1) {
        letter->current_card_index++;
        letter->cards[letter->current_card_index]->showState = VISIBLE;
      }
    }
  case DONE:
    break;
  }
}

void DrawCard(Letter *letter, Card *card) {
  Vector2 globalPos = Vector2Add(letter->pos, card->pos);

  Vector2 desired_dimensions = GetScaledUpDimensions(200, card->texture.height);
  int half_padding = GetWindowPadding() / 2;
  Rectangle expandedRec = (Rectangle){
      half_padding, globalPos.y, desired_dimensions.x, desired_dimensions.y};

  DrawTexturePro(card->texture, letter->animation->frame_rec, expandedRec,
                 (Vector2){0, 0}, 0, WHITE);

  switch (card->cardType) {
  case CARD_ENVELOPE: {
    Vector2 scaledPos = ScalePointBasedOnRef(200, (Vector2){0, 50});
    Vector2 titlePos =
        Vector2Add(globalPos, (Vector2){scaledPos.x, scaledPos.y});
    DrawText(card->cardData.cardEnvelopeData.title, titlePos.x, titlePos.y, 30,
             BLACK);
    break;
  }
  case CARD_TEXT: {
    Vector2 textPos =
        Vector2Add(globalPos, ScalePointBasedOnRef(200, (Vector2){0, 30}));
    DrawText(card->cardData.cardTextData.text, textPos.x, textPos.y, 15, BLACK);
    break;
  }
  case CARD_IMAGE:
    break;
  }
}
