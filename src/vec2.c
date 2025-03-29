#include "vec2.h"
#define PI 3.14159265358979323846

Vec2 vec2_add(Vec2 v1, Vec2 v2) {
    return (Vec2) {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
    };
}

Vec2 vec2_sub(Vec2 v1, Vec2 v2) {
    return (Vec2) {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
    };
}

Vec2 vec2_mul(Vec2 v1, Vec2 v2) {
    return (Vec2) {
        .x = v1.x * v2.x,
        .y = v1.y * v2.y,
    };
}

Vec2 vec2_div(Vec2 v1, Vec2 v2) {
    return (Vec2) {
        .x = v1.x / v2.x,
        .y = v1.y / v2.y,
    };
}

Vec2 vec2_norm(Vec2 v1, Vec2 v2, float dist) {
    return (Vec2) {
        .x = (v2.x - v1.x) / dist,
        .y = (v2.y - v1.y) / dist,
    };
}

Vec2 vec2_scale(Vec2 v, float s) {
    return (Vec2) {
        .x = v.x * s,
        .y = v.y * s,
    };
}

Vec2 vec2_move(Vec2 v, double s, double angle_deg) {
    double angle_rad = angle_deg * PI / 180.0;
    return (Vec2) {
        .x = v.x + s * cos(angle_rad),
        .y = v.y + s * sin(angle_rad),
    };
}
        
Vec2 vec2_move_toward(Vec2 v, Vec2 dir, float speed) {
    float dist = vec2_dist(v, dir);
    if (dist > 30) {
        Vec2 norm = vec2_norm(v, dir, dist); 
        return vec2_add(v, vec2_scale(norm, 30));
    } else return dir;

}

float vec2_dist(Vec2 v1, Vec2 v2) {
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

