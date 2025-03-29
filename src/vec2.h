#ifndef VEC2_SAFE
#define VEC2_SAFE

#include <SDL2/SDL.h>
#include <math.h>

typedef SDL_FPoint Vec2;


Vec2 vec2_add(Vec2 v1, Vec2 v2);

Vec2 vec2_sub(Vec2 v1, Vec2 v2);

Vec2 vec2_mul(Vec2 v1, Vec2 v2);

Vec2 vec2_div(Vec2 v1, Vec2 v2);

Vec2 vec2_norm(Vec2 v1, Vec2 v2, float dist);

Vec2 vec2_scale(Vec2 v1, float s);

Vec2 vec2_move(Vec2 v, double s, double angle_deg);

Vec2 vec2_move_toward(Vec2 v, Vec2 dir, float speed);

float vec2_dist(Vec2 v1, Vec2 v2);

#endif
