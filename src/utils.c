#include "utils.h"
#include "raymath.h"

// TODO: This is v rough. Tidy up and clarify naming
Vector2 ScalePointBasedOnRef(int startWidth, Vector2 pt) {
  int screenWidth = GetScreenWidth();
  int padding = GetWindowPadding();
  int desiredW = screenWidth - padding;

  if (startWidth <= 0)
    return (Vector2){0, 0};

  float scale = (float)desiredW / (float)startWidth;
  return Vector2Scale(pt, scale);
}

// Return the scaled card dimensions (keep aspect from the original texture
// dims).
Vector2 GetScaledUpDimensions(int start_width, int start_height) {
  int screenWidth = GetScreenWidth();
  int padding = GetWindowPadding();
  int desiredW = screenWidth - padding;

  if (start_width <= 0 || start_height <= 0)
    return (Vector2){0, 0};

  float scale = (float)desiredW / (float)start_width;
  float outW = (float)desiredW;
  float outH = (float)start_height * scale;

  return (Vector2){outW, outH};
}

int GetWindowPadding(void) {
  int screen_width = GetScreenWidth();
  int padding = screen_width / 8;
  return padding;
}
