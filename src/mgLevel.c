#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "primitives.h"
#include "nanovg.h"
#include "mgLevel.h"
#include "mgRoom.h"

#include "clipper_c.h"
#include "delaunay.h"
#include "kvec.h"

extern NVGcontext* vg;
extern void spaceDraw(cpSpace* space);

////////////////////////////////////////////////////////////////////////////////

mgLevel*
mgLevelAlloc(void) {
    return cpcalloc(1, sizeof(mgLevel)); 
}

mgLevel*
mgLevelInit(mgLevel* level) {
    cpSpace* space = (cpSpace*) level;
    cpSpaceInit(space);

    cpSpaceSetDamping(space, SPACE_DAMPING);
    cpSpaceSetIterations(space, 10);
    return level;
}

mgLevel* 
mgLevelNew() {
    return mgLevelInit(mgLevelAlloc());
}

mgLevel*
mgLevelDestroy(mgLevel* level) {
    mgLevelClearLinks(level);
}

void 
mgLevelFree(mgLevel* level) {
    if(level) {
        mgLevelDestroy(level);
        cpfree(level);
     }
}

////////////////////////////////////////////////////////////////////////////////

void
mgLevelCreateRandomRoom(mgLevel* level) {
    mgRoomNew(level, 
        cpv(rand()%80*10, rand()%80*10), 
        cpv(rand()%10*10+40, rand()%10*10+40));
    mgLevelSimulate(level);
    mgLevelLinkAllRooms(level);
}

void 
mgLevelUpdate(mgLevel* level, float dt) {
    // cpSpace* space = (cpSpace*) level;
    // cpSpaceStep(space, 0.1);
}

void 
mgLevelSimulate(mgLevel* level) {
    cpSpace* space = (cpSpace*) level;
    for(int i=0; i<10; i++) cpSpaceStep(space, 0.1);
}

void mgLevelClearLinks(mgLevel* level){
    kv_destroy(level->links);
    level->links = (mgLinkVec){0}; 
    // clear links from bodies
    cpArray* bodies = level->dynamicBodies;
    for(int i=0; i<bodies->num; i++){
        mgRoom* r = (mgRoom*)bodies->arr[i];
        mgRoomClearLinks(r); 
    };
}

void
mgLevelLinkTwoRooms(mgLevel* level, mgRoom* a, mgRoom* b) {
    mgLink link = {a, b};
    kv_push(mgLink, level->links, link);
    mgRoomLinkTogether(a, b);
}

void 
mgLevelLinkAllRooms(mgLevel* level) {    
    mgLevelClearLinks(level);
    cpSpace* space = (cpSpace*) level;
    cpSpaceLock(space); {
        cpArray* bodies = space->dynamicBodies;
        for(int i=0; i<bodies->num; i++){
            // body a
            cpBody* a = bodies->arr[i];
            cpShapeFilter filter = {1, CP_ALL_CATEGORIES, CP_ALL_CATEGORIES};
            a->shapeList[0].filter = filter;
            {
                // shape point disntace gradient    
                cpPointQueryInfo out = {0};
                cpSpacePointQueryNearest(space, a->p, 1000.0, filter, &out);
                if(out.shape != NULL) {
                    cpBody* b = out.shape->body;
                    mgLevelLinkTwoRooms(level, (mgRoom*)a, (mgRoom*)b);
                }
            }
            a->shapeList[0].filter = (cpShapeFilter){CP_NO_GROUP, CP_ALL_CATEGORIES, CP_ALL_CATEGORIES};
        }
    } cpSpaceUnlock(space, cpTrue);
}

void 
mgLevelKey(mgLevel* level, int key, int action) {
    if(key==32 && action == 1) mgLevelCreateRandomRoom(level);
}

void
mgLevelDrawLinks(mgLevel* level) {
    nvgBeginPath(vg);
    for(int i=0; i<level->links.n; i++) {
        mgLink* l = &level->links.a[i];
        mgRoom* a = l->a;
        mgRoom* b = l->b;
        nvgMoveTo(vg, a->p.x, a->p.y);
        nvgLineTo(vg, b->p.x, b->p.y);
    }
    nvgStrokeColor(vg, nvgRGBf(0.2, 0.2, 0.2));
    nvgStrokeWidth(vg, 1.0);
    nvgStroke(vg);
    nvgClosePath(vg);
}

void 
mgLevelRender(mgLevel* level) {
    spaceDraw((cpSpace*)level);
    mgLevelDrawLinks(level);
}

