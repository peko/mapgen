#include "tunnel.h"
#include "world.h"
#include "globals.h"

extern cpSpace* space;

tunnel_t* tunnelNew(world_t* world) {
    tunnel_t* tunnel = calloc(1,sizeof(tunnel_t));
  
    cpFloat size = 30.0;
    cpFloat mass =  1.0;

    int w = 10;
    int h = 40;
    int x = rand()%WIDTH;
    int y = rand()%HEIGHT;

    for(int i=0; i<5; i++) {

        cpFloat m = cpMomentForSegment(mass, cpv(0,-h/2.0), cpv(0,h/2.0), 0.0f);
        cpBody *body = cpSpaceAddBody(world->space, cpBodyNew(mass, m));
    	cpBodySetPosition(body, cpv(x,y));
    	
    	cpShape *shape = cpSpaceAddShape(world->space, cpSegmentShapeNew(body, cpv(x,y-h*i), cpv(x,y-h*(i+1.0)), 10.0f));
    	cpShapeSetElasticity(shape, 0.0f);
    	cpShapeSetFriction(shape, 0.7f);
    	cpShapeSetFilter(shape, cpShapeFilterNew(1, CP_ALL_CATEGORIES, CP_ALL_CATEGORIES));
    	
    } 
    return tunnel;
};

void tunnelFree(tunnel_t* tunnel) {
    free(tunnel);
};

void tunnelSetA(tunnel_t* tunnel, room_t* a) {
    tunnel->a = a;
}

void tunnelSetB(tunnel_t* tunnel, room_t* b) {
    tunnel->b = b;
}
