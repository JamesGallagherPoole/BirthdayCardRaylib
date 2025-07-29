#include "animation.h"
#include "raylib.h"
#include <stdlib.h>

Animation *CreateAnimation(AnimationParams params) {
  Animation *animation = calloc(1, sizeof(Animation));

  animation->frame_rec =
      (Rectangle){params.frame_rec.x, params.frame_rec.y,
                  params.frame_rec.width, params.frame_rec.height};
  animation->frame_time = params.frame_time;
  animation->frame_count = params.frame_count;
  animation->loop = params.loop;

  return animation;
}

void FreeAnimation(Animation *animation) { free(animation); }
void PlayAnimation(Animation *animation) { animation->is_playing = true; }

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
      }
    }
  }
  animation->frame_rec.x =
      animation->current_frame * animation->frame_rec.width;
}
