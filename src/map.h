#ifndef MAP_SAFE
#define MAP_SAFE

#include "vec2.h"
#include "SDL2/SDL.h"

#define DEFAULT_MAP_PATH "map.txt"


typedef enum Tile {
    Nothing,
    Wall,
    Flower
} Tile;

typedef struct Map {
    Tile** map;
    Vec2 dim;
    Vec2 player_base_pos;
} Map;

Map* init_map();
void free_map();
Tile map_get(Map* map, Vec2 pos);
void map_display_ascii(Map* map);

#endif
