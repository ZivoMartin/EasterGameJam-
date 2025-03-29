#include "map.h"
#include "app.h"
#include "stdlib.h"

void map_display_ascii(Map* map) {
    for (int j = 0; j < map->dim.y; j++) {
        for (int i = 0; i < map->dim.x; i++) {
            printf("%d ", map->map[j][i]);
        }
        printf("\n");
    }
}


Map* init_map(char* map_path) {
    Map* map = malloc(sizeof(Map));
    FILE* fd = fopen(map_path, "r");
    if (!fd) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int width = 0, height = 0;
    int current_col = 0;
    char c;

    int capacity = 8;
    map->map = malloc(sizeof(Tile*) * capacity);

    while (fread(&c, 1, 1, fd)) {
        if (c == '\n') {
            if (height == 0) {
                width = current_col;
            } else if (current_col != width) {
                fprintf(stderr, "Error while loading the map %s: inconsistent line width (%d instead of %d)\n", map_path, current_col, width);
                exit(EXIT_FAILURE);
            }

            if (height >= capacity) {
                capacity *= 2;
                map->map = realloc(map->map, capacity * sizeof(Tile*));
            }

            height++;
            current_col = 0;
        } else {
            if (current_col == 0) {
                map->map[height] = malloc(sizeof(Tile) * 256); 
            }

            switch (c) {
            case '0':
                map->map[height][current_col++] = Nothing;
                break;
            case '1':
                map->map[height][current_col++] = Wall;
                break;
            case 'p':
                map->player_base_pos = (Vec2) {
                    .x = current_col,
                    .y = height,
                };
                map->map[height][current_col++] = Nothing;
                break;
            default:
                fprintf(stderr, "Error while loading the map %s: unsupported character '%c'\n", map_path, c);
                exit(EXIT_FAILURE);
            }
        }
    }
    map->dim = (Vec2) {
        .x = width,
        .y = height,
    };
    map_display_ascii(map);
    fclose(fd);
    return map;
}

void free_map(Map* map) {
    for (int i = 0; i < map->dim.y; i++)
        free(map->map[i]);
    free(map->map);
}
