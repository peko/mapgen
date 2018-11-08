#include "room.h"
#include "level.h"
#include "globals.h"

mgRoom*
mgRoomAlloc(void) {
    return cpcalloc(1, sizeof(mgRoom));
}

mgRoom* 
mgRoomNew(mgLevel* level, cpVect pos) {
   return mgRoomInit(mgRoomAlloc(), level, pos);
}

mgRoom*
mgRoomInit(mgRoom* room, mgLevel* level, cpVect pos) {

    cpSpace* space = (cpSpace*)level;
    cpBody* body = (cpBody*) room;
    cpShape* shape;

    cpFloat size = 30.0;
    cpFloat mass =  1.0;

    int w = rand()%10*4+40;
    int h = rand()%10*4+40;
    
    cpBodyInit(body, mass, cpMomentForBox(mass, w, h));
    cpBodySetPosition(body, cpv(rand()%WIDTH, rand()%HEIGHT));    
    shape = cpSpaceAddShape(space, cpBoxShapeNew(body, w, h, 5.0));

    return room;
}

void 
mgRoomDestroy(mgRoom* room) {
    kv_destroy(room->links);
}

void 
mgRoomFree(mgRoom* room) {
   if(room) {
       mgRoomDestroy(room);
       cpfree(room);
   }
}

void
mgRoomLinkTogether(mgRoom* a, mgRoom* b) {
   if(a && b) {
      kv_push(mgRoom*, a->links, b);
      kv_push(mgRoom*, b->links, a);
   }
}
