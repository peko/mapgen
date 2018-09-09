#include <time.h>
#include <GLFW/glfw3.h>

#include "engine.h"
#include "world.h"

world_t* world = NULL;

void update(float dt){
    if(world) worldUpdate(world, dt);
}

void render(float dt) {
    if(world) worldRender(world);
}

void key(int key, int action) {
    if(!world) return; 
    if (key == GLFW_KEY_KP_8) worldUp   (world);
    if (key == GLFW_KEY_KP_5) worldDown (world);
    if (key == GLFW_KEY_KP_4) worldLeft (world);
    if (key == GLFW_KEY_KP_6) worldRight(world);
}

int main(int argc, char* argv[]) {

    srand(time(NULL));

    world = worldNew();

    engine_init();
    engine_update = update;
    engine_render = render; 
    engine_key    = key; 
    engine_start();
    engine_deinit();
    
    worldFree(world);

    return 0;
}

