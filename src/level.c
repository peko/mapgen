#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "nanovg.h"
#include "level.h"
#include "room.h"

#include "clipper_c.h"
#include "delaunay.h"
#include "kvec.h"

extern NVGcontext* vg;
extern void spaceDraw(cpSpace* space);

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

void
mgLevelCreateRandomRoom(mgLevel* level) {
    mgRoomNew(level, 
        cpv(rand()%80*10, rand()%80*10), 
        cpv(rand()%10*10+40, rand()%10*10+40));
}

void 
mgLevelUpdate(mgLevel* level, float dt) {
    cpSpace* space = (cpSpace*) level;
    cpSpaceStep(space, 0.1);
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
            // put self to fitler group 1, to ignore in query
            a->shapeList[0].filter = filter;
            // shape point disntace gradient    
            cpPointQueryInfo out = {0};
            cpSpacePointQueryNearest(space, a->p, 1000.0, filter, &out);
            cpBody* b = out.shape->body;

            mgLevelLinkTwoRooms(level, (mgRoom*)a, (mgRoom*)b);
            
            // return self to no group
            a->shapeList[0].filter = (cpShapeFilter){CP_NO_GROUP, CP_ALL_CATEGORIES, CP_ALL_CATEGORIES};
        }
    } cpSpaceUnlock(space, cpTrue);
}

// Events

void 
mgLevelKey(mgLevel* level, int key, int action) {
    if(key==32 && action == 1) mgLevelCreateRandomRoom(level);
}

void 
mgLevelRender(mgLevel* level) {
    spaceDraw((cpSpace*)level);
}

