#include "room.h"
#include "tunnel.h"
#include "world.h"
#include "globals.h"

#define r_pos   (room->pos)
#define r_body  (room->body)
#define r_shape (room->shape)
#define r_w     (room->w)
#define r_h     (room->h)

room_t* 
roomNew(world_t* world, cpVect cp) {

    room_t* room = calloc(1, sizeof(room_t));
    
    cpFloat size = 30.0;
    cpFloat mass =  1.0;

    r_w = rand()%10*8+40;
    r_h = rand()%10*8+40;
    cpVect p = cpv(rand()%WIDTH, rand()%HEIGHT);
    r_body = cpSpaceAddBody(
        world->space, 
        cpBodyNew(mass, cpMomentForBox(mass, r_w, r_h)));
    cpBodySetPosition(r_body, p);    
    r_shape = cpSpaceAddShape(
        world->space, 
        cpBoxShapeNew(r_body, r_w, r_h, 5.0));

    return room;
}

void 
roomFree(room_t* room) {
    cpShapeFree(r_shape);
    cpBodyFree(r_body);
    free(room);
}

tunnel_t* roomsConnect(world_t* world, room_t* a, room_t* b) {
    tunnel_t* tunnel = tunnelNew(world);
    tunnelSetA(tunnel, a);
    tunnelSetB(tunnel, b);
    return tunnel;
}

/*    
        cpBody* body = createRoom(world);
        
        // avoiding self ray-query by temporary disabling collision 
        cpShape* shape = body->shapeList;
        cpShapeFilter f = shape->filter;
        shape->filter = CP_SHAPE_FILTER_NONE;
    
        cpPointQueryInfo nearestInfo = {};
        cpSpacePointQueryNearest(w_space, body->p, 500.0, CP_SHAPE_FILTER_ALL, &nearestInfo);
        if(nearestInfo.shape) {
            cpVect* p = &nearestInfo.shape->body->p;
            cpConstraint* constr = cpDampedSpringNew(body, nearestInfo.shape->body, cpv(0,0), cpv(-0,0), 50.0f, 5.0f, 0.3f);
            //cpConstraint* constr =  cpPivotJointNew(body, nearestInfo.shape->body, cpvmult(cpvadd(body->p, *p), 0.5));
            cpSpaceAddConstraint(w_space, constr);
        }
        shape->filter = f;
*/
