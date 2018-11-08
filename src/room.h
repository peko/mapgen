#pragma once

#include "globals.h"

typedef struct mgRoom mgRoom;

typedef kvec_t(mgRoom*) mgRoomVec;

typedef struct mgLevel mgLevel;
typedef struct mgRoom {
    struct cpBody;
    mgRoomVec links;
} mgRoom;

mgRoom* mgRoomAlloc(void);
mgRoom* mgRoomInit(mgRoom* room, mgLevel* level, cpVect pos);
mgRoom* mgRoomNew(mgLevel* level, cpVect pos);
void    mgRoomDestroy(mgRoom* room);
void    mgRoomFree(mgRoom* room);
void    mgRoomLinkTogether(mgRoom* a, mgRoom* b);
