#include "player.h"
#include "app.h"
#include "vec2.h"

#define STEP_SIZE 1
#define SCREEN_WIDTH 45

int normalize_angle(int angle) {
    angle %= 360;
    if (angle < 0) angle += 360;
    return angle;
}

void display_player(Player* p, App* app) {
    int bande_width = app->w_size.x / SCREEN_WIDTH;
    printf("angle: %d\n", p->dir);
    for (
        int i = 0, angle = normalize_angle(p->dir - SCREEN_WIDTH);
        angle != normalize_angle(p->dir + SCREEN_WIDTH);
        angle = normalize_angle(angle + 1), i++
        ) {
        int dist;
        if (i > SCREEN_WIDTH) dist = i - SCREEN_WIDTH;
        else dist = SCREEN_WIDTH - i; 
        Vec2 base_point = vec2_move(p->pos, dist, angle);
        base_point.x += PLAYER_W / 2;
        base_point.y += PLAYER_H / 2;
        
        Vec2 point = base_point;

        int tw = tile_width(app), th = tile_height(app);
        for(int d = dist;; d++) {
            point = vec2_move(point, STEP_SIZE, p->dir);
            if (point.x > app->w_size.x) point.x = app->w_size.x - 1;
            if (point.y > app->w_size.y) point.y = app->w_size.y - 1;
            if (point.x < 0) point.x = 0;
            if (point.y < 0) point.y = 0;

            
            int x = point.x / tw, y = point.y / th;

            if (!is_valid_point(app, point) || app->map->map[y][x] == Wall) {
                Vec2 bord = point;
                while (is_valid_point(app, bord)) {
                    /* printf("%f %f\n", bord.x, bord.y); */
                    bord = vec2_move(bord, STEP_SIZE, p->dir);
                }

                int extern_band_height = ((d * app->w_size.y) / vec2_dist(bord, p->pos)) / 2;
                int middle_band_height = app->w_size.y - extern_band_height * 2;
                
                SDL_Rect r = (SDL_Rect) {
                    .x = i * bande_width,
                    .y = 0,
                    .w = bande_width,
                    .h = extern_band_height
                };
                SDL_SetRenderDrawColor(app->ren, EXTERN_COLOR);
                SDL_RenderFillRect(app->ren, &r);
                r.y = middle_band_height + extern_band_height;
                SDL_RenderFillRect(app->ren, &r);

                SDL_SetRenderDrawColor(app->ren, INTERN_COLOR);

                r.y = extern_band_height;
                r.h = middle_band_height;
                SDL_RenderFillRect(app->ren, &r);
                break;
            }
        }

    }
}
