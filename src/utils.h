#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"
typedef enum { INVISIBLE, ENTER, VISIBLE, EXIT, DONE } ShowState;

Vector2 GetScaledUpDimensions(int original_width, int original_height);
int GetWindowPadding(void);

#endif
