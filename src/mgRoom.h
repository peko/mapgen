#pragma once

#include "globals.h"

typedef struct mgRoom mgRoom;

typedef kvec_t(mgRoom*) mgRoomVec;

typedef struct mgLevel mgLevel;
typedef struct mgRoom {
    cpBody    body;
    mgRoomVec links;
} mgRoom;

mgRoom* mgRoomAlloc(void);
mgRoom* mgRoomInit(mgRoom* room, mgLevel* level, cpVect pos, cpVect size);
mgRoom* mgRoomNew(mgLevel* level, cpVect pos, cpVect size);
void    mgRoomDestroy(mgRoom* room);
void    mgRoomFree(mgRoom* room);
void    mgRoomLinkTogether(mgRoom* a, mgRoom* b);
void    mgRoomClearLinks(mgRoom* room);
