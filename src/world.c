#include <stdlib.h>

#include "globals.h"
#include "nanovg.h"
#include "world.h"

#define w_space (world->space)

extern  NVGcontext* vg;
extern void spaceDraw(cpSpace* space);

static cpBool onAntCollision(cpArbiter* arb, cpSpace* space, void* data);

world_t* 
worldNew(int ants_count, int food_count) {

    world_t* world = calloc(sizeof(world_t), 1);
    w_space = cpSpaceNew();

    cpSpaceSetDamping(w_space, SPACE_DAMPING);
    
    
    // walls
    { 
        cpShape *shape;
        cpBody *staticBody = cpSpaceGetStaticBody(w_space);

        // Create segments around the edge of the screen.
        shape = cpSpaceAddShape(w_space, cpSegmentShapeNew(staticBody, cpv(0,0), cpv(0, HEIGHT-0), 0.0f));
        cpShapeSetElasticity(shape, 1.0f);
        cpShapeSetFriction  (shape, 1.0f);
        cpShapeSetCollisionType(shape, WALL);

        shape = cpSpaceAddShape(w_space, cpSegmentShapeNew(staticBody, cpv(0,0), cpv(WIDTH-0, 0), 0.0f));
        cpShapeSetElasticity(shape, 1.0f);
        cpShapeSetFriction  (shape, 1.0f);
        cpShapeSetCollisionType(shape, WALL);
        
        shape = cpSpaceAddShape(w_space, cpSegmentShapeNew(staticBody, cpv(WIDTH-0,0), cpv(WIDTH-0, HEIGHT-0), 0.0f));
        cpShapeSetElasticity(shape, 1.0f);
        cpShapeSetFriction  (shape, 1.0f);
        cpShapeSetCollisionType(shape, WALL);

        shape = cpSpaceAddShape(w_space, cpSegmentShapeNew(staticBody, cpv(0, HEIGHT-0), cpv(WIDTH-0, HEIGHT-0), 0.0f));
        cpShapeSetElasticity(shape, 1.0f);
        cpShapeSetFriction  (shape, 1.0f);
        cpShapeSetCollisionType(shape, WALL);
    }

    cpCollisionHandler* h = cpSpaceAddWildcardHandler(w_space, ANT);
    h->beginFunc = onAntCollision; 

    return world;
}


static cpBool 
onAntCollision(cpArbiter* arb, cpSpace* space, void* data) {
    // if(arb->b->type == FOOD) {
    //     arb->b->body->p.x = rnd(WIDTH);
    //     arb->b->body->p.y = rnd(HEIGHT);
    //     return cpFalse;
    // }
    return cpTrue;
}

void 
worldFree(world_t* world) {

    cpSpaceFree(world->space);

    free(world);
}

void 
worldUpdate(world_t* world, float dt) {
    // cpSpaceStep(w_space, dt);
    cpSpaceStep(w_space, 1.0/60.0);
}

// Move 

void worldUp(world_t* world){

};

void worldDown(world_t* world){

};

void worldLeft(world_t* world){

};

void worldRight(world_t* world){

};

// PRIVATE

static inline cpSpaceDebugColor 
RGBAColor(float r, float g, float b, float a){
    cpSpaceDebugColor color = {r, g, b, a};
    return color;
}

void 
worldRender(world_t* world) {
    spaceDraw(w_space);
}

static inline void 
drawVector(double x, double y, double dx, double dy, NVGcolor c) {
    nvgBeginPath(vg);
    nvgMoveTo(vg, x, y);
    nvgLineTo(vg, x+dx, y+dy);
    nvgStrokeColor(vg, c);
    nvgStroke(vg);
}

