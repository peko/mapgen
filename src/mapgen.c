#include <time.h>
#include <GLFW/glfw3.h>

#include "engine.h"
#include "level.h"

mgLevel* level = NULL;

void 
update(float dt){
    if(level) mgLevelUpdate(level, dt);
}

void 
render(float dt) {
    if(level) mgLevelRender(level);
}

void 
key(int key, int action) {
    if(!level) return; 
    mgLevelKey(level, key, action);
}

void
mouseClick(int button, int action,int mods){
    mgLevelCreateRandomRoom(level);
}

int main(int argc, char* argv[]) {

    srand(time(NULL));

    level = mgLevelNew();

    engine_init();
    engine_update     = update;
    engine_render     = render; 
    engine_key        = key;
    engine_mouseClick = mouseClick;

    engine_start();
    engine_deinit();
    
    mgLevelFree(level);

    return 0;
}

