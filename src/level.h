#pragma once

#include <chipmunk/chipmunk_private.h>
#include "kvec.h"
#include "clipper_c.h"

typedef struct {
    cpBody* a;
    cpBody* b;
} link_t;

typedef kvec_t(link_t) link_v;

typedef struct mgLevel {
    cpSpace* space;
	link_v links;
} mgLevel;

typedef struct {
	struct cpBody;
	link_v links;
} mgRoom;

mgLevel* mgLevelNew();
void mgLevelFree(mgLevel* level);

void mgLevelAddRoom(mgLevel* level);

void mgLevelUpdate(mgLevel* level, float dt);
void mgLevelRender(mgLevel* level);

void mgLevelKey(mgLevel* level, int key, int action);
