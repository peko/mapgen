#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "nanovg.h"
#include "level.h"
#include "room.h"

#include "clipper_c.h"

#define l_space (level->space)

extern NVGcontext* vg;
extern void spaceDraw(cpSpace* space);

static cpBool onAntCollision(cpArbiter* arb, cpSpace* space, void* data);
static void drawKpaths(kpaths_t* paths, NVGcolor c);
static cpBody* createRoom(mgLevel* level);

mgLevel* 
mgLevelNew() {

    mgLevel* level = calloc(sizeof(mgLevel), 1);
    l_space = cpSpaceNew();

    cpSpaceSetDamping(l_space, SPACE_DAMPING);
    for(int i=0; i<40; i++) {
        roomNew(level, (cpVect){0.0,0.0});
    }

    return level;
}

void 
mgLevelFree(mgLevel* level) {
    cpSpaceFree(level->space);
    free(level);
}

void eachBody(cpBody* body, void* data) {
    cpArray* a = data;
    cpVect* p = &body->p;
    printf("%f, %f\n", p->x, p->y);
}

kvec

void 
mgLevelUpdate(mgLevel* level, float dt) {
    // cpSpaceStep(l_space, dt);
    cpSpaceStep(l_space, 1.0/60.0);
    //cpArray* points = cpArrayNew();
    //buildDelaunay(mgLevel* level);
    cpSpaceEachBody(l_space, eachBody, NULL);
    // cpBB bb = {0};
    // cpSpaceEachShape(l_space, eachShape, &bb);
    // printf("%f %f %f %f\n",  
    //     bb.l,  bb.r, 
    //     bb.t,  bb.b); 
}

// Move 

void 
mgLevelUp(mgLevel* level){

};

void
mgLevelDown(mgLevel* level){

};

void
mgLevelLeft(mgLevel* level){

};

void
mgLevelRight(mgLevel* level){

};

void 
mgLevelRender(mgLevel* level) {
    spaceDraw(l_space);
}

// PRIVATE

static inline cpSpaceDebugColor 
RGBAColor(float r, float g, float b, float a){
    cpSpaceDebugColor color = {r, g, b, a};
    return color;
}


static cpBody* 
createRoom(mgLevel* level) { 
    cpBody*  body;
    cpShape* shape;

    cpFloat size = 30.0;
    cpFloat mass =  1.0;

    int w = rand()%10*4+40;
    int h = rand()%10*4+40;
    body = cpSpaceAddBody(l_space, cpBodyNew(mass, cpMomentForBox(mass, w, h)));
    cpBodySetPosition(body, cpv(rand()%WIDTH, rand()%HEIGHT));    

    shape = cpSpaceAddShape(l_space, cpBoxShapeNew(body, w, h, 5.0));
    //cpShapeSetElasticity(shape, 0.0f);
    //cpShapeSetFriction(shape, 0.7f);
    //cpShapeSetCollisionType(shape, BOX);

    return body;
}

static void 
clipping(mgLevel* level) {
    /*
    kaths_t* map  = kpaths_new();
    kaths_t* subj = kpaths_new();
    kaths_t* clip = kpaths_new();
    {
        // no need to free, automaticaly cleared by kpaths_free
        kpath_t* subj_path = kpaths_add_new_path(level->subj);
        kpath_t* clip_path = kpaths_add_new_path(level->clip);
        int n = 64;
        for(size_t i=0; i<n; i++) {
            kpoint_t p = {
                400+300.0*cos(i/(float)n*3.141*2.0*3),
                400+300.0*sin(i/(float)n*3.141*2.0*5)};
            kpath_add_point(subj_path, &p);            
            p = (kpoint_t){
                400+300.0*cos(i/(float)n*3.141*2.0*5),
                400+300.0*sin(i/(float)n*3.141*2.0*3)};
            kpath_add_point(clip_path, &p);
        }
        clipp_kpaths(level->subj, level->clip, level->map);
        printf("Clippig result:\n");
        kpaths_print(level->map);
    }
    */
}

static inline void 
drawVector(double x, double y, double dx, double dy, NVGcolor c) {
    nvgBeginPath(vg);
    nvgMoveTo(vg, x, y);
    nvgLineTo(vg, x+dx, y+dy);
    nvgStrokeColor(vg, c);
    nvgStroke(vg);
}

static void
drawKpaths(kpaths_t* paths, NVGcolor c) {
    if(paths == 0) return;
    nvgStrokeWidth(vg, 1.0);
    nvgStrokeColor(vg, c);
    nvgFillColor(vg, c);
    for(size_t i=0; i<paths->n; i++) {
        kpath_t* p = &paths->a[i];
        nvgBeginPath(vg);
        nvgMoveTo(vg, p->a[0].x, p->a[0].y);
        for(size_t j=1; j<p->n; j++) {
            nvgLineTo(vg, p->a[j].x, p->a[j].y);
        }
        nvgLineTo(vg, p->a[0].x, p->a[0].y);
        nvgFill(vg);
        nvgStroke(vg);
    }
}

static void buildDelaunay(mgLevel* level) {
	
}