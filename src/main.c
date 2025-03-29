#include "app.h"
#include "map.h"

int main(int argc, char** argv) {
    char* path = DEFAULT_MAP_PATH;
    if (argc > 1) path = argv[1]; 

    Map* map = init_map(path);
    
    App* app = init_app(map);

    int code = main_loop(app);
    free_app(app);
    
    return code;
}
