#include "card.h"
#include "animation.h"
#include "custom_arrays.h"
#include "raylib.h"
#include "raymath.h"
#include "types.h"
#include "utils.h"
#include <stdint.h>

Card CreateCard(CardParams cardParams) {
  Card card;

  card.cardType = cardParams.cardType;
  card.cardData = cardParams.cardData;
  card.texture = cardParams.cardTexture;
  card.pos = (Vector2){0, 0};
  card.showState = INVISIBLE;
  card.isFinished = false;

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
        CardArray_At(letter->cards, letter->current_card_index + 1)->showState =
            ENTER;
        // letter->cards[letter->current_card_index + 1]->showState = ENTER;
      }

      if (IsKeyPressed(KEY_SPACE)) {
        PlayAnimation(letter->animation);
        PlaySound(letter->sounds.tear);
      }

      if (IsKeyReleased(KEY_SPACE)) {
        StopAnimation(letter->animation);
        PauseSound(letter->sounds.tear);
      }

      if (letter->animation->is_finished) {
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
    if (!card->isFinished) {
      card->isFinished = true;
    }
    break;
  case CARD_BOAT: {
    AskoyBoatData *data = &card->cardData.askoyBoatData;

    // TODO: Make this contentRec global and quit this duplication nonsense
    Vector2 globalPos = Vector2Add(letter->pos, card->pos);
    Vector2 desired_dimensions =
        GetScaledUpDimensions(200, card->texture.height);
    int half_padding = GetWindowPadding() / 2;
    Rectangle contentRec = (Rectangle){
        half_padding, globalPos.y, desired_dimensions.x, desired_dimensions.y};
    int maxX = contentRec.width - (contentRec.width / 3);

    if (!card->isFinished && data->boatPosX >= maxX && IsKeyUp(KEY_SPACE)) {
      card->isFinished = true;
    }

    if (IsKeyDown(KEY_SPACE) && data->boatPosX < maxX) {
      data->boatVelX += data->accel * GetFrameTime();
    } else {
      // Apply friction
      int32_t sign;
      if (data->boatVelX >= 0) {
        sign = 1;
      }
      if (data->boatVelX < 0) {
        sign = -1;
      }
      data->boatVelX -= sign * data->friction * GetFrameTime();
    }

    data->boatVelX = Clamp(data->boatVelX, 0, data->boatTopSpeed);
    data->boatPosX += data->boatVelX * GetFrameTime();
    break;
  }
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

        // Little jammed in
        if (card->cardType == CARD_ENVELOPE) {
          if (!IsSoundPlaying(letter->sounds.intro)) {
            SetSoundVolume(letter->sounds.intro, 0.7);
            PlaySound(letter->sounds.intro);
          }
        }

        if (letter->current_card_index < letter->numberOfCards - 1) {
          CardArray_At(letter->cards, letter->current_card_index + 1)
              ->showState = ENTER;
        }
      }
    }
    break;
  case EXIT:
    // Animate Out
    if (card->pos.y < screen_height + 150) {
      card->pos.y += letter->slideSpeed * GetFrameTime();
    } else {
      card->showState = DONE;

      if (letter->current_card_index < letter->numberOfCards - 1) {
        letter->current_card_index++;
        CardArray_At(letter->cards, letter->current_card_index)->showState =
            VISIBLE;
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
  case CARD_IMAGE: {
    Texture2D tex = card->cardData.cardImageData.texture;

    Vector2 size = GetScaledUpDimensions(200, card->texture.height);

    Rectangle cardRect = (Rectangle){half_padding, globalPos.y, size.x, size.y};

    // Inner content area
    int32_t inner_pad = 50;
    Rectangle content = (Rectangle){
        cardRect.x + inner_pad, cardRect.y + inner_pad,
        cardRect.width - 2 * inner_pad, cardRect.height - 2 * inner_pad};

    // Fit image into content box, preserving aspect
    float sx = content.width / (float)tex.width;
    float sy = content.height / (float)tex.height;
    float s = (sx < sy) ? sx : sy;

    float w = tex.width * s;
    float h = tex.height * s;
    float x = content.x + (content.width - w) * 0.5f;
    float y = content.y + (content.height - h) * 0.5f;

    // Draw full texture -> fitted destination
    Rectangle src = (Rectangle){0, 0, (float)tex.width, (float)tex.height};
    Rectangle dst = (Rectangle){x, y, w, h};

    DrawTexturePro(tex, src, dst, (Vector2){0, 0}, 0, WHITE);

    Vector2 textPos =
        Vector2Add(globalPos, ScalePointBasedOnRef(200, (Vector2){0, 100}));
    DrawText(card->cardData.cardImageData.text, textPos.x, textPos.y, 20,
             card->cardData.cardImageData.textColour);
    break;
  case CARD_BOAT: {
    AskoyBoatData *data = &card->cardData.askoyBoatData;

    Vector2 globalPos = Vector2Add(letter->pos, card->pos);

    Vector2 desired_dimensions =
        GetScaledUpDimensions(200, card->texture.height);
    int half_padding = GetWindowPadding() / 2;
    Rectangle contentRec = (Rectangle){
        half_padding, globalPos.y, desired_dimensions.x, desired_dimensions.y};

    DrawTexturePro(data->oceanBackground,
                   (Rectangle){0, 0, (float)data->oceanBackground.width,
                               (float)data->oceanBackground.height},
                   contentRec, (Vector2){0, 0}, 0, WHITE);

    Vector2 globalBoatPos = (Vector2){-data->boatPosX, 20};

    DrawTexturePro(data->boatTex,
                   (Rectangle){0, 0, (float)data->oceanBackground.width,
                               (float)data->oceanBackground.height},
                   contentRec, globalBoatPos, 0, WHITE);

    // DrawTexture(data->boatTex, globalBoatPos.x, globalBoatPos.y, WHITE);

    if (card->showState == VISIBLE) {
      DrawText("Trykk til å kjøre til Askøy...", 50, GetScreenHeight() - 50, 20,
               DARKBLUE);
    }
    break;
  }
  }
  }
}
