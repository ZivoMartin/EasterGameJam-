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

typedef struct FlowerData {
    int band;
    Vec2 point;
} FlowerData;

DisplayData display_player(Player* p, App* app) {
      Vec2* res = malloc(sizeof(Vec2) * SCREEN_WIDTH*2);
      Vec2* bands = malloc(sizeof(Vec2) * SCREEN_WIDTH*2);

      int f_capacity = 10, nf = 0;
      FlowerData* flowers = malloc(sizeof(FlowerData) * f_capacity);
      int tw = tile_width(app), th = tile_height(app);

      int bande_width = app->w_size.x / SCREEN_WIDTH;
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
          
          for(int d = dist;; d++) {
              point = vec2_move(point, STEP_SIZE, p->dir);
              if (point.x > app->w_size.x) point.x = app->w_size.x - 1;
              if (point.y > app->w_size.y) point.y = app->w_size.y - 1;
              if (point.x < 0) point.x = 0;
              if (point.y < 0) point.y = 0;
            
              int x = point.x / tw, y = point.y / th;

              if (!is_valid_point(app, point) || app->map->map[y][x] == Wall) {
                  
                
                  Vec2 bord = point;
                  while (is_valid_point(app, bord)) bord = vec2_move(bord, STEP_SIZE, p->dir);

                  int extern_band_height = ((d * app->w_size.y) / vec2_dist(bord, p->pos)) / 2;
                  int middle_band_height = app->w_size.y - extern_band_height * 2;
                  if (!is_valid_point(app, point)) {
                      extern_band_height = app->w_size.y / 2;
                      middle_band_height = 0;
                  }

                  res[i] = point;
                  bands[i] = (Vec2) { extern_band_height, middle_band_height };
                  

                  int band_color_h = (app->w_size.y / 2) / 13;

                  SDL_Rect r = (SDL_Rect) {
                      .x = i * bande_width,
                      .y = 0,
                      .w = bande_width,
                      .h = extern_band_height
                  };
                  
                  SDL_SetRenderDrawColor(app->ren, 100, 100, 255, 255);
                  SDL_RenderFillRect(app->ren, &r);                  

                  r.h = band_color_h;
                  r.y = app->w_size.y;
                  int color = 125;
                  while (r.y > extern_band_height + middle_band_height) {
                      r.y -= band_color_h;
                      SDL_SetRenderDrawColor(app->ren, 100, color, 100, 255);
                      SDL_RenderFillRect(app->ren, &r);
                      color += 10;
                  }
                  
                  int gray = (d * 255) / vec2_dist(bord, p->pos);
                  
                  if (gray > 200) gray = 200;
                
                  SDL_SetRenderDrawColor(app->ren, gray, gray, gray, 255);

                  r.y = extern_band_height;
                  r.h = middle_band_height;
                  SDL_RenderFillRect(app->ren, &r);
                  break;
              } else if (app->map->map[y][x] == Flower) {
                  
                  flowers[nf ++] = (FlowerData) {
                      .point = point,
                      .band = i,
                  };
                  app->map->map[y][x] = Nothing; // To avoid storing two times the same flower
                  if (nf == f_capacity) {
                      f_capacity *= 2;
                      flowers = realloc(flowers, f_capacity * sizeof(FlowerData));
                      if (!flowers) {
                          fprintf(stderr, "Running out of mem\n");
                          exit(1);
                      }
                  }
              }
          }
        
      }
      for (int i = 0; i < nf; i++) {
          int extern_band_height = bands[flowers[i].band].x, intern_band_height = bands[flowers[i].band].y;
          
          Vec2 bord = flowers[i].point;
          while (is_valid_point(app, bord)) bord = vec2_move(bord, STEP_SIZE, p->dir);

          int y_padding = extern_band_height * 0.3;
          int size = (extern_band_height - (vec2_dist(flowers[i].point, p->pos) * extern_band_height) / vec2_dist(bord, p->pos)) / 3;
          if (size < 10) size = 10;
          
          SDL_Rect f_rect = (SDL_Rect) {
              .x = flowers[i].band * bande_width + bande_width / 2,
              .y = extern_band_height + intern_band_height + y_padding,
              .w = size,
              .h = size,
          };
          SDL_RenderCopy(app->ren, app->flower, NULL, &f_rect); 
          app->map->map[(int) flowers[i].point.y / th][(int) flowers[i].point.x / tw] = Flower;
      }
      free(flowers);
      free(bands);
      return (DisplayData) {
          .points = res,
          .n = 2*SCREEN_WIDTH
      };
}
