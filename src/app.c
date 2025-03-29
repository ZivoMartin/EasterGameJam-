#include "app.h"
#include "player.h"
#include "map.h"

App* init_app(Map* map) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Failed to init SDL");
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
        
        SDL_Event e;
        while (SDL_PollEvent (&e)) {
            switch (e.type) {
            case SDL_QUIT:
                app->close = 1;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    p->pos.y -= 5;
                    break;
                case SDLK_DOWN:
                    p->pos.y += 5;
                    break;
                case SDLK_LEFT:
                    p->pos.x -= 5;
                    break;
                case SDLK_RIGHT:
                    p->pos.x += 5;
                    break;
                default:
                    ;
                }
                break;
            case SDL_MOUSEMOTION:
                /* printf("%d\n", e.motion.xrel); */
                if (loop_count % 10 == 0) p->dir = normalize_angle(p->dir + e.motion.xrel);
                break;
            default:
                ;
            }
            
        }
        SDL_RenderClear(app->ren);
        actualise_w_size(app);
        /* render_map(app); */
        display_player(p, app);

        SDL_SetRenderDrawColor(app->ren, APP_BG);
        SDL_RenderPresent(app->ren);
 
        SDL_Delay(1000 / 60);
    }
    return 0;
}


void render_map(App* app) {
    int tw = tile_width(app);
    int th = tile_height(app);
    
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
                SDL_SetRenderDrawColor(app->ren, APP_BG);
                break;
            case Wall:
                SDL_SetRenderDrawColor(app->ren, 0, 0, 0, 255);
                break;
            }
            SDL_RenderFillRect(app->ren, &r);
        }
    }

}


int tile_width(App* app) {
    return app->w_size.x / app->map->dim.x;
}

int tile_height(App* app) {
    return app->w_size.y / app->map->dim.y;
}

bool is_valid_point(App* app, Vec2 v) {
    Vec2 norm_v = vec2_div(v, (Vec2) {tile_width(app), tile_height(app)});
    return v.x >= 0 && v.y >= 0 && norm_v.x < app->map->dim.x && norm_v.y < app->map->dim.y;
}

void actualise_w_size(App* app) {
    int w, h;
    SDL_GetWindowSize(app->win, &w, &h);
    
    app->w_size = (Vec2) {
        .x = w,
        .y = h
    };
}
