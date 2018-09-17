#pragma once

#include <chipmunk/chipmunk_private.h>

typedef struct room_t room_t;
typedef struct mgLevel mgLevel;

typedef struct tunnel_t {
    room_t* a_room;
    room_t* b_room;
    
    cpArray* shapes;
    cpArray* bodies;

    cpBody* a_body;
    cpBody* b_body;
} tunnel_t;

tunnel_t* tunnelNew(mgLevel* level);
void tunnelFree(tunnel_t* tunnel);

void tunnelSetA(tunnel_t* tunnel, room_t* a);
void tunnelSetB(tunnel_t* tunnel, room_t* b);
