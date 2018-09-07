#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "nanovg.h"
#include "world.h"

#include "clipper_c.h"

#define w_space (world->space)

extern  NVGcontext* vg;
extern void spaceDraw(cpSpace* space);

static cpBool onAntCollision(cpArbiter* arb, cpSpace* space, void* data);
void drawKpaths(kpaths_t* paths, NVGcolor c);
cpBody* createRoom(world_t* world);

world_t* 
worldNew(int ants_count, int food_count) {

    world_t* world = calloc(sizeof(world_t), 1);
    w_space = cpSpaceNew();

    cpSpaceSetDamping(w_space, SPACE_DAMPING);
    
    
    // walls
    if(0) { 
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
    
    for(int i=0; i<40; i++) { 
        cpBody* body = createRoom(world);
        
        // avoiding self ray-query by temporary disabling collision 
        cpShape* shape = body->shapeList;
        cpShapeFilter f = shape->filter;
        shape->filter = CP_SHAPE_FILTER_NONE;
    
        cpPointQueryInfo nearestInfo = {};
        cpSpacePointQueryNearest(w_space, body->p, 500.0, CP_SHAPE_FILTER_ALL, &nearestInfo);
        if(nearestInfo.shape) {
            cpVect* p = &nearestInfo.shape->body->p;
            cpConstraint* constr = cpDampedSpringNew(body, nearestInfo.shape->body, cpv(0,0), cpv(-0,0), 50.0f, 5.0f, 0.3f);
            //cpConstraint* constr =  cpPivotJointNew(body, nearestInfo.shape->body, cpvadd(body->p, cpv(100,100)));
            cpSpaceAddConstraint(w_space, constr);
        }
        shape->filter = f;
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

static void clipping(world_t* world) {
    /*
    kaths_t* map  = kpaths_new();
    kaths_t* subj = kpaths_new();
    kaths_t* clip = kpaths_new();
    {
        // no need to free, automaticaly cleared by kpaths_free
        kpath_t* subj_path = kpaths_add_new_path(world->subj);
        kpath_t* clip_path = kpaths_add_new_path(world->clip);
        int n = 64;
        for(size_t i=0; i<n; i++) {
            kpoint_t p = {
                400+300.0*cos(i/(float)n*3.141*2.0*3),
                400+300.0*sin(i/(float)n*3.141*2.0*5)};
            kpath_add_point(subj_path, &p);            
            p = (kpoint_t){
                400+300.0*cos(i/(float)n*3.141*2.0*5),
                400+300.0*sin(i/(float)n*3.141*2.0*3)};
            kpath_add_point(clip_path, &p);
        }
        clipp_kpaths(world->subj, world->clip, world->map);
        printf("Clippig result:\n");
        kpaths_print(world->map);
    }
    */
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

void
drawKpaths(kpaths_t* paths, NVGcolor c) {
    if(paths == 0) return;
    nvgStrokeWidth(vg, 1.0);
    nvgStrokeColor(vg, c);
    nvgFillColor(vg, c);
    for(size_t i=0; i<paths->n; i++) {
        kpath_t* p = &paths->a[i];
        nvgBeginPath(vg);
        nvgMoveTo(vg, p->a[0].x, p->a[0].y);
        for(size_t j=1; j<p->n; j++) {
            nvgLineTo(vg, p->a[j].x, p->a[j].y);
        }
        nvgLineTo(vg, p->a[0].x, p->a[0].y);
        nvgFill(vg);
        nvgStroke(vg);
    }
}

cpBody* createRoom(world_t* world) { 
    cpBody*  body;
    cpShape* shape;

    cpFloat size = 30.0;
    cpFloat mass =  1.0;

    int w = rand()%10*4+40;
    int h = rand()%10*4+40;
    body = cpSpaceAddBody(w_space, cpBodyNew(mass, cpMomentForBox(mass, w, h)));
    cpBodySetPosition(body, cpv(rand()%WIDTH, rand()%HEIGHT));    

    shape = cpSpaceAddShape(w_space, cpBoxShapeNew(body, w, h, 5.0));
    cpShapeSetElasticity(shape, 0.0f);
    cpShapeSetFriction(shape, 0.7f);
    cpShapeSetCollisionType(shape, BOX);

    return body;
}
