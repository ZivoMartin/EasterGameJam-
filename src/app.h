#ifndef APP_SAFE
#define APP_SAFE

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "map.h"
#include "vec2.h"

#define APP_BG 255, 0, 0, 255
#define MAP_BG 200, 200, 255, 255
#define EXTERN_COLOR 255, 255, 255, 255
#define INTERN_COLOR 0, 0, 0, 255
#define FLOWER_PATH "flower.png"

typedef struct App {
    SDL_Renderer* ren;
    SDL_Window* win;
    bool close;
    Map* map;
    Vec2 w_size;
    SDL_Texture* flower;
} App;

App* init_app();

void free_app(App* app);
    
int main_loop(App* app);

void actualise_w_size(App* app);
int tile_width(App* app);
int tile_height(App* app);

bool is_valid_point(App* app, Vec2 v);

#endif
