#include "custom_arrays.h"
#include "arena.h"
#include "types.h"
#include <assert.h>
#include <stdint.h>

CardArray *CardArray_Create(Arena *arena, int32_t initialCapacity) {

  CardArray *cardArray = arena_alloc(arena, sizeof(CardArray));
  cardArray->length = 0;
  cardArray->capacity = initialCapacity;
  cardArray->items = arena_alloc(arena, sizeof(Card) * initialCapacity);

  return cardArray;
}

Card *CardArray_At(CardArray *array, int32_t index) {
  Card *card = CardArray_Get(array, index);

#ifndef NDEBUG
  assert(card && "CardArray_At: index out of range");
#endif
  return card;
}

Card *CardArray_Get(CardArray *array, int32_t index) {
  if (index >= 0 && index < array->length) {
    return &array->items[index];
  }
  return NULL;
}

void CardArray_Push(CardArray *array, Card card) {
  if (array->length < array->capacity) {
    array->items[array->length] = card;
    array->length++;
  }

  // TODO: How to handle expanding automatically when the limit is hit?
}
