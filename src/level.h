#pragma once

#include <chipmunk/chipmunk_private.h>
#include "kvec.h"
#include "clipper_c.h"

typedef struct mgRoom mgRoom;
typedef struct mgLink {
    mgRoom* a;
    mgRoom* b;
} mgLink;

typedef kvec_t(mgLink) mgLinkVec;

typedef struct mgLevel {
    struct cpSpace;
    mgLinkVec links;
} mgLevel;

mgLevel* mgLevelAlloc(void);
mgLevel* mgLevelInit(mgLevel* level);
mgLevel* mgLevelNew(void);

void mgLelevDestroy(mgLevel* level);
void mgLevelFree(mgLevel* level);

void mgLevelCreateRandomRoom(mgLevel* level);
void mgLevelLinkTwoRooms(mgLevel* level, mgRoom* a, mgRoom* b);
void mgLevelLinkAllRooms(mgLevel* level);
void mgLevelClearLinks(mgLevel* level);

void mgLevelUpdate(mgLevel* level, float dt);
void mgLevelRender(mgLevel* level);

void mgLevelKey(mgLevel* level, int key, int action);
