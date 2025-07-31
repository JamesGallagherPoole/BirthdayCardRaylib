#include "animation.h"
#include "arena.h"
#include "raylib.h"

Animation *CreateAnimation(Arena *arena, AnimationParams params) {
  Animation *animation = arena_alloc(arena, sizeof(Animation));

  animation->frame_rec =
      (Rectangle){params.frame_rec.x, params.frame_rec.y,
                  params.frame_rec.width, params.frame_rec.height};
  animation->frame_time = params.frame_time;
  animation->frame_count = params.frame_count;
  animation->time = 0.0;
  animation->loop = params.loop;
  animation->is_playing = false;
  animation->is_finished = false;

  return animation;
}

void PlayAnimation(Animation *animation) { animation->is_playing = true; }
void StopAnimation(Animation *animation) {
  animation->is_playing = false;
  animation->time = 0.0;
}

void UpdateAnimation(Animation *animation) {
  if (animation->is_playing) {
    animation->time += GetFrameTime();
  }
  if (animation->time >= animation->frame_time) {
    animation->time = 0.0;
    animation->current_frame += 1;

    if (animation->current_frame >= animation->frame_count) {
      if (animation->loop) {
        animation->current_frame = 0;
      } else {
        animation->current_frame = animation->frame_count - 1;
        animation->is_playing = false;
        animation->is_finished = true;
      }
    }
  }
  animation->frame_rec.x =
      animation->current_frame * animation->frame_rec.width;
}
