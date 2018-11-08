#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "nanovg.h"
#include "level.h"
#include "room.h"

#include "clipper_c.h"
#include "delaunay.h"
#include "kvec.h"

#define l_space (level->space)
#define l_links (*level->links)

extern NVGcontext* vg;
extern void spaceDraw(cpSpace* space);

static cpBool onAntCollision(cpArbiter* arb, cpSpace* space, void* data);
static void drawKpaths(kpaths_t* paths, NVGcolor c);
static cpBody* createRoom(mgLevel* level);

mgLevel* 
mgLevelNew() {

    mgLevel* level = calloc(sizeof(mgLevel), 1);
    l_space = cpSpaceNew();
    
    cpSpaceSetDamping(l_space, SPACE_DAMPING);
    //cpSpaceSetSleepTimeThreshold(l_space, 2);
    cpSpaceSetIterations(l_space, 10);
    // for(int i=0; i<40; i++) {
    //    roomNew(level, (cpVect){0.0,0.0});
    // }
    // Simulate
    // for(int i=0; i<10; i++) cpSpaceStep(l_space, 0.1);
    return level;
}

void 
mgLevelFree(mgLevel* level) {
    cpSpaceFree(level->space);
    free(level);
}

void
mgLevelAddRoom(mgLevel* level) {
    // roomNew(level, (cpVect){0.0,0.0});
    createRoom(level);
}


void 
mgLevelUpdate(mgLevel* level, float dt) {
    cpSpaceStep(l_space, 0.1);
    cpSpaceLock(l_space); {

        // Run over all bodies and link with near
        cpArray *bodies = l_space->dynamicBodies;
        for(int i=0; i<bodies->num; i++){
            cpBody* b = bodies->arr[i];
                  
            cpShapeFilter filter = {1, CP_ALL_CATEGORIES, CP_ALL_CATEGORIES};
            // put self to fitler group 1, to ignore in query
            b->shapeList[0].filter = filter;
            // shape point disntace gradient    
            cpPointQueryInfo out = {0};
            cpSpacePointQueryNearest(l_space, b->p, 1000.0, filter, &out);
            printf("%d\n", out.shape);

            // return self to no group
            b->shapeList[0].filter = (cpShapeFilter){CP_NO_GROUP, CP_ALL_CATEGORIES, CP_ALL_CATEGORIES};
        }
    } cpSpaceUnlock(l_space, cpTrue);
}


// Events

void 
mgLevelKey(mgLevel* level, int key, int action) {
    if(key==32 && action == 1) createRoom(level);
}

void 
mgLevelRender(mgLevel* level) {
    spaceDraw(l_space);
}

// PRIVATE

static inline cpSpaceDebugColor 
RGBAColor(float r, float g, float b, float a){
    cpSpaceDebugColor color = {r, g, b, a};
    return color;
}


static cpBody* 
createRoom(mgLevel* level) { 
    cpBody*  body;
    cpShape* shape;

    cpFloat size = 30.0;
    cpFloat mass =  1.0;

    int w = rand()%10*4+40;
    int h = rand()%10*4+40;
    body = cpSpaceAddBody(l_space, cpBodyNew(mass, cpMomentForBox(mass, w, h)));
    cpBodySetPosition(body, cpv(rand()%WIDTH, rand()%HEIGHT));    
    
    shape = cpSpaceAddShape(l_space, cpBoxShapeNew(body, w, h, 5.0));
    //cpShapeSetElasticity(shape, 0.0f);
    //cpShapeSetFriction(shape, 0.7f);
    //cpShapeSetCollisionType(shape, BOX);

    return body;
}

