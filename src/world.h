#pragma once

#include <chipmunk/chipmunk_private.h>
#include "clipper_c.h"

typedef struct room_t room_t;

typedef struct world_t {
    cpSpace* space;
} world_t;

world_t* worldNew();
void worldFree(world_t* world);

void worldUpdate(world_t* world, float dt);
void worldRender(world_t* world);

room_t* worldSelectRoom(world_t* world, cpVect vect);

void worldUp   (world_t* world);
void worldDown (world_t* world);
void worldLeft (world_t* world);
void worldRight(world_t* world);
