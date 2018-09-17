#include "tunnel.h"

#include "room.h"
#include "level.h"
#include "globals.h"

extern cpSpace* space;

tunnel_t* tunnelNew(mgLevel* level) {
    tunnel_t* tunnel = calloc(1,sizeof(tunnel_t));

    cpFloat size = 30.0;
    cpFloat mass =  1.0;

    int w = 10;
    int h = 40;
    int x = 200; // rand()%WIDTH;
    int y = 200; // rand()%HEIGHT;
    
    cpBody* body = 0;
    cpBody* old  = 0;

    cpShapeFilter filter = cpShapeFilterNew(1, CP_ALL_CATEGORIES, CP_ALL_CATEGORIES);
    int n = 5;
    for(int i=0; i<n; i++) {

        cpVect a = cpv(0, -h/2.0);
        cpVect b = cpv(0,  h/2.0);
        
        cpFloat m = cpMomentForSegment(mass, a, b, TUNNEL_R);
        body = cpSpaceAddBody(level->space, cpBodyNew(mass, m));
        cpBodySetPosition(body, cpv(x, y+i*h));

        cpShape *shape = cpSpaceAddShape(level->space, cpSegmentShapeNew(body, a, b, TUNNEL_R));
        cpShapeSetElasticity(shape, 0.0f);
        cpShapeSetFriction(shape, 0.7f);

        cpShapeSetFilter(shape, filter);
        
        if(old == 0) {
            tunnel->a_body = body;
        } else {
            cpConstraint* j =  cpPivotJointNew(body, old, cpvadd(cpv(x,y+h*i),a));
            cpSpaceAddConstraint(level->space, j);
            j = cpDampedRotarySpringNew(
                body, old, 
                0.0f, 30000.0f, 60.0f);
            cpSpaceAddConstraint(level->space, j);
        } 
        old = body;
    }
    tunnel->b_body = body;
    
    return tunnel;
};

void tunnelFree(tunnel_t* tunnel) {
    free(tunnel);
};

void tunnelSetA(tunnel_t* tunnel, room_t* a) {
    tunnel->a_room = a; 
    cpVect p = cpvadd(a->body->p, cpv(0,-a->h/2.0));

    cpBodySetPosition(tunnel->a_body, cpvadd(p, cpv(0.0,-20.0-15.0)));
    cpBodySetAngle(tunnel->a_body, cpBodyGetAngle(a->body)+3.141);
    
    cpConstraint* j;
    //j = cpPivotJointNew(
    //    tunnel->a_body, 
    //    tunnel->a_room->body, 
    //    p);
    j = cpGrooveJointNew(
        tunnel->a_room->body,
        tunnel->a_body, 
        cpv( a->w/2.0-TUNNEL_R, -a->h/2.0-15.0),
        cpv(-a->w/2.0+TUNNEL_R, -a->h/2.0-15.0), 
        cpv(0,-20.0));
    cpSpaceAddConstraint(tunnel->a_body->space, j);
    j = cpDampedRotarySpringNew(
        tunnel->a_body,
        tunnel->a_room->body,
        3.141f, 30000.0f, 60.0f);
    cpSpaceAddConstraint(tunnel->a_body->space, j);
}

void tunnelSetB(tunnel_t* tunnel, room_t* b) {
    tunnel->b_room = b;
    cpVect p = cpvadd(b->body->p, cpv(0,b->h/2.0));

    cpBodySetPosition(tunnel->b_body, cpvadd(p, cpv(0.0,20.0+15.0)));
    cpBodySetAngle(tunnel->b_body, cpBodyGetAngle(b->body)+3.141);

    cpConstraint* j;
    j = cpGrooveJointNew(
        tunnel->b_room->body,
        tunnel->b_body, 
        cpv( b->w/2.0-TUNNEL_R, b->h/2.0+15.0),
        cpv(-b->w/2.0+TUNNEL_R, b->h/2.0+15.0), 
        cpv(0, 20.0));
    // j = cpPivotJointNew(
    //     tunnel->b_body, 
    //     tunnel->b_room->body, 
    //     p);
    cpSpaceAddConstraint(tunnel->b_body->space, j);
    j = cpDampedRotarySpringNew(
        tunnel->b_body,
        tunnel->b_room->body,
        3.141f, 30000.0f, 60.0f);
    cpSpaceAddConstraint(tunnel->b_body->space, j);
}
