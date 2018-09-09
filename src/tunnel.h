#pragma once

#include <chipmunk/chipmunk_private.h>

typedef struct room_t room_t;
typedef struct world_t world_t;

typedef struct tunnel_t {
    room_t* a;
    room_t* b;
} tunnel_t;

tunnel_t* tunnelNew(world_t* world);
void tunnelFree(tunnel_t* tunnel);

void tunnelSetA(tunnel_t* tunnel, room_t* a);
void tunnelSetB(tunnel_t* tunnel, room_t* b);
