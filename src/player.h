#ifndef PLAYER_SAFE
#define PLAYER_SAFE

#include "vec2.h"
#include "app.h"

#define PLAYER_COLOR 255, 0, 0, 255
#define PLAYER_W 6
#define PLAYER_H PLAYER_W

typedef struct Player {
    Vec2 pos; // x, y position of the player upon the map
    int dir; // Angle of player's sight
} Player;

void display_player(Player* p, App* app);
int normalize_angle(int angle);

#endif
