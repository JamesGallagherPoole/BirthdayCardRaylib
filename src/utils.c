#include "utils.h"
#include "raymath.h"

// TODO: This is v rough. Tidy up and clarify naming
Vector2 ScalePointBasedOnRef(int startWidth, Vector2 pointToScale) {
  int screen_width = GetScreenWidth();

  int padding = GetWindowPadding();

  int desired_width = screen_width - padding;

  int ratio = desired_width / startWidth;

  return Vector2Scale(pointToScale, ratio);
}

Vector2 GetScaledUpDimensions(int start_width, int start_height) {
  int screen_width = GetScreenWidth();

  int padding = GetWindowPadding();

  int desired_width = screen_width - padding;

  int ratio = desired_width / start_width;

  int desired_height = start_height * ratio;

  return (Vector2){desired_width, desired_height};
}

int GetWindowPadding(void) {
  int screen_width = GetScreenWidth();
  int padding = screen_width / 8;
  return padding;
}
