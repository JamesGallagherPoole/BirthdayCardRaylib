#ifndef CUSTOM_ARRAYS_H
#define CUSTOM_ARRAYS_H

#include "types.h"
#include <stdint.h>

CardArray *CardArray_Create(Arena *arena, int32_t initialCapacity);
Card *CardArray_Get(CardArray *array, int32_t index);
Card *CardArray_At(CardArray *array, int32_t index);
void CardArray_Push(CardArray *array, Card card);

#endif
