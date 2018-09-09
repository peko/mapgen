#pragma once

#include <chipmunk/chipmunk_private.h>

typedef struct world_t world_t;
typedef struct tunnel_t tunnel_t;

typedef struct room_t {
    cpVect*  pos;
    cpBody*  body;
    cpShape* shape;
} room_t;

room_t* roomNew(world_t* world, cpVect pos);
void roomFree(room_t* room);

tunnel_t* roomsConnect(world_t* world, room_t* a, room_t* b);
