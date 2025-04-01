#include "app.h"
#include "player.h"
#include "map.h"
#include <SDL2/SDL_image.h>

#define MAP_SIZE 5
void render_map(App* app, Player* p, DisplayData d) {
    int tw = tile_width(app) / MAP_SIZE;
    int th = tile_height(app) / MAP_SIZE;
    
    for (int j = 0; j < app->map->dim.y; j++) {
        for (int i = 0; i < app->map->dim.x; i++) {
            SDL_Rect r = (SDL_Rect) {
                .x = i * tw,
                .y = j * th,
                .w = tw,
                .h = th,
            };
            switch (app->map->map[j][i]) {
            case Nothing:
                SDL_SetRenderDrawColor(app->ren, MAP_BG);
                break;
            case Wall:
                SDL_SetRenderDrawColor(app->ren, 0, 0, 0, 255);
                break;
            case Flower:
                SDL_SetRenderDrawColor(app->ren, 0, 255, 0, 255);
                break;

            }
            SDL_RenderFillRect(app->ren, &r);
        }
    }
    int px = p->pos.x / MAP_SIZE, py = p->pos.y / MAP_SIZE;
    SDL_Rect p_rect = (SDL_Rect) {
        .x = px,
        .y = py,
        .h = PLAYER_H,
        .w = PLAYER_W
    };
    
    SDL_SetRenderDrawColor(app->ren, 255, 0, 0, 255);
    SDL_RenderFillRect(app->ren, &p_rect);
    for (int i = 0; i < d.n; i++) {
        SDL_RenderDrawLine(app->ren, px, py, d.points[i].x / MAP_SIZE, d.points[i].y / MAP_SIZE);
    }
    free(d.points);
}



App* init_app(Map* map) {
   
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Failed to init SDL");
        exit(1);
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "could not initialize sdl2_image: %s\n", IMG_GetError());
        exit(1);
     }
    
    App* app = malloc(sizeof(App));

    app->map = map;
    app->win = SDL_CreateWindow("GAME",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                1000, 1000, 0);
    app->ren = SDL_CreateRenderer(app->win, -1, 0);
    app->close = false;
    SDL_Surface* s = IMG_Load(FLOWER_PATH);
    app->flower = SDL_CreateTextureFromSurface(app->ren, s);
    SDL_FreeSurface(s);
    
    actualise_w_size(app);
    
    return app;
}

void free_app(App* app) {
    free_map(app->map);   
    free(app);
}

int main_loop(App* app) {
    Player* p = malloc(sizeof(Player));
    p->pos = (Vec2) {
        .x = app->map->player_base_pos.x * tile_width(app) + tile_width(app) / 2,
        .y = app->map->player_base_pos.y * tile_height(app) + tile_height(app) / 2,
    };
    p->dir = 90;
    int loop_count = 0;
    while (!app->close) {
        loop_count ++;
        float angle;
        int to_add, x, y, additional ;
        SDL_Event e;
        while (SDL_PollEvent (&e)) {
            switch (e.type) {
            case SDL_QUIT:
                app->close = 1;
                break;
            case SDL_KEYDOWN:

                switch (e.key.keysym.sym) {
                case SDLK_UP: case SDLK_z:
                    angle = p->dir;
                    break;
                case SDLK_DOWN: case SDLK_s:
                    angle = normalize_angle(p->dir + 180);
                    break;
                case SDLK_LEFT: case SDLK_q:
                    angle = normalize_angle(p->dir - 90);
                    break;
                case SDLK_RIGHT: case SDLK_d:
                    angle = normalize_angle(p->dir + 90);
                    break;
                default:
                    ;
                }
                Vec2 new_pos = vec2_move(p->pos, PLAYER_SPEED, angle);
                x = new_pos.x / tile_width(app), y = new_pos.y / tile_height(app);
                if (is_valid_point(app, new_pos)) {
                    switch (app->map->map[y][x]) {
                    case Flower:
                        app->map->map[y][x] = Nothing;
                    case Nothing:
                        p->pos = new_pos;
                        break;
                    case Wall:
                        ;
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                additional = 1;
                if(e.motion.xrel < 0) additional = -1;
                to_add = ((e.motion.xrel * 360) / app->w_size.x) + additional;
                p->dir = normalize_angle(p->dir + to_add);
                break;
            default:
                ;
            }
            
        }
        SDL_RenderClear(app->ren);
        actualise_w_size(app);
        
        DisplayData d = display_player(p, app);
        render_map(app, p, d);
        
        SDL_SetRenderDrawColor(app->ren, APP_BG);
        SDL_RenderPresent(app->ren);
 
        SDL_Delay(1000 / 60);
    }
    return 0;
}




int tile_width(App* app) {
    return app->w_size.x / app->map->dim.x;
}

int tile_height(App* app) {
    return app->w_size.y / app->map->dim.y;
}

bool is_valid_point(App* app, Vec2 v) {
    Vec2 norm_v = vec2_div(v, (Vec2) {tile_width(app), tile_height(app)});
    return v.x > 0 && v.y > 0 && norm_v.x < app->map->dim.x && norm_v.y < app->map->dim.y;
}

void actualise_w_size(App* app) {
    int w, h;
    SDL_GetWindowSize(app->win, &w, &h);
    
    app->w_size = (Vec2) {
        .x = w,
        .y = h
    };
}
