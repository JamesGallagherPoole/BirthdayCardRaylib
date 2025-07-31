#ifndef LETTER_H
#define LETTER_H

#include "arena.h"
#include "types.h"

Letter *CreateLetter(Arena *arena);
void UpdateLetter(Letter *letter);
void DrawLetter(Letter *letter);
void DrawLetterUi(Letter *letter);

#endif
