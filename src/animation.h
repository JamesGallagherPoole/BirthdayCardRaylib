#ifndef CARD_ANIMATION_H
#define CARD_ANIMATION_H

#include "arena.h"
#include "raylib.h"

typedef struct {
  Rectangle frame_rec;
  int frame_count;
  int current_frame;
  float frame_time;
  float time;
  bool is_playing;
  bool is_finished;
  bool loop;
} Animation;

typedef struct {
  Rectangle frame_rec;
  int frame_count;
  float frame_time;
  bool loop;
} AnimationParams;

Animation *CreateAnimation(Arena *arena, AnimationParams params);
void PlayAnimation(Animation *animation);
void StopAnimation(Animation *animation);
void UpdateAnimation(Animation *animation);

#endif
