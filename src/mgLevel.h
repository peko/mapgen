#pragma once

#include "primitives.h"
#include "clipper_c.h"

typedef struct mgLevel {
    cpSpace space;
    mgLinkV links;
    mgClipPoly clipPoly;
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
void mgLevelSimulate(mgLevel* level);
void mgLevelUpdate(mgLevel* level, float dt);
void mgLevelRender(mgLevel* level);

void mgLevelKey(mgLevel* level, int key, int action);
