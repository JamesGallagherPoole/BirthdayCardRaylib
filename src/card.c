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
  DrawTextureRec(card->texture, letter->animation->frame_rec,
                 Vector2Add(letter->pos, card->pos), WHITE);
}
