#pragma once

#include "globals.h"

typedef struct mgLevel mgLevel;
typedef struct tunnel_t tunnel_t;

typedef struct room_t {
    cpVect*  pos;
    cpBody*  body;
    cpShape* shape;
    cpFloat  w, h;
} room_t;

room_t* roomNew(mgLevel* level, cpVect pos);
void roomFree(room_t* room);

tunnel_t* roomsConnect(mgLevel* level, room_t* a, room_t* b);
