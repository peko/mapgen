#include "globals.h"
#include "primitives.h"
#include "mgRoom.h"
#include "mgLevel.h"

mgRoom*
mgRoomAlloc(void) {
    return cpcalloc(1, sizeof(mgRoom));
}

mgRoom* 
mgRoomNew(mgLevel* level, cpVect pos, cpVect size) {
   return mgRoomInit(mgRoomAlloc(), level, pos, size);
}

mgRoom*
mgRoomInit(
    mgRoom* room, 
    mgLevel* level, 
    cpVect pos, 
    cpVect size) {

    cpSpace* space = (cpSpace*)level;
    cpBody* body = (cpBody*) room;
    cpFloat moment = cpMomentForBox(1.0, size.x, size.y);
    cpBodyInit(body, 1.0, moment);
    cpBodySetPosition(body, pos);
    cpSpaceAddBody(space, body);
    cpShape* shape = cpBoxShapeNew(body, size.x, size.y, 10.0);
    cpSpaceAddShape(space, shape);

    return room;
}

void 
mgRoomDestroy(mgRoom* room) {
    mgRoomClearLinks(room);
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

void
mgRoomClearLinks(mgRoom* room) {
    kv_destroy(room->links);
    room->links = (mgRoomVec){0};
}
