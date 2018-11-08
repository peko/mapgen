#pragma once

#include <chipmunk/chipmunk_private.h>
#include "clipper_c.h"

typedef struct mgLevel {
    cpSpace* space;
} mgLevel;

mgLevel* mgLevelNew();
void mgLevelFree(mgLevel* level);

void mgLevelAddRoom(mgLevel* level);

void mgLevelUpdate(mgLevel* level, float dt);
void mgLevelRender(mgLevel* level);

void mgLevelKey(mgLevel* level, int key, int action);
