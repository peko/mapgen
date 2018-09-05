#pragma once

#include <chipmunk/chipmunk_private.h>

typedef struct ant_t ant_t;

typedef struct world_t {
    cpSpace* space;
} world_t;

world_t* worldNew(int ants_count, int food_count);
void worldFree   (world_t* world);

void worldUpdate (world_t* world, float dt);
void worldRender (world_t* world);

void worldUp     (world_t* world);
void worldDown   (world_t* world);
void worldLeft   (world_t* world);
void worldRight  (world_t* world);