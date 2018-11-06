#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "nanovg.h"
#include "level.h"
#include "room.h"

#include "clipper_c.h"
#include "delaunay.h"
#include "kvec.h"
#include "khash.h"

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


// body link + body center for dellanuay
typedef struct {
    kvec_t(cpBody*      ) bodies;
    kvec_t(del_point2d_t) centers;
} bodies_v;

void 
eachBody(cpBody* body, void* data) {
    bodies_v* bodies = data;
    kv_push(
        cpBody*,
        bodies->bodies,
        body);
    kv_push(
        del_point2d_t, 
        bodies->centers, 
        ((del_point2d_t){body->p.x, body->p.y}));
}


// HASH SET with uniq pairs
KHASH_SET_INIT_INT(uniq);

static void
addLinkToSet(khash_t(uniq)* s, unsigned int a, unsigned int b) {
    if(a>b) { 
        unsigned int t=b; b=a; a=t;
    }
    unsigned int v = (a<<16) | b;
    int absent; kh_put(uniq, s, v, &absent);
    // printf("%d %d\n", a, b);
}

void
mgLevelUpdate(mgLevel* level, float dt) {

    cpSpaceStep(l_space, 1.0/60.0);

    bodies_v bodies ={0};
    // iterate over all bodies, collect centers
    cpSpaceEachBody(l_space, eachBody, &bodies);

    // calculate delaunay
    delaunay2d_t* delaunay = delaunay2d_from(bodies.centers.a, bodies.centers.n);
    unsigned int* faces = delaunay->faces; 
    // set of id pairs a-b, where a>b
    khash_t(uniq)* s = kh_init(uniq);
    
    for(int fid=0; fid<delaunay->num_faces; fid++) {
        unsigned int vcnt=*faces++;
        unsigned int a, b, f;
        f = a = *faces++;
        while(--vcnt>0) {
            b = *faces++;
            addLinkToSet(s, a, b);
            a = b;
        }
        addLinkToSet(s, b, f);
    }

    nvgBeginPath(vg);
    for(khint_t i=kh_begin(s); i!=kh_end(s); ++i) {
        if(kh_exist(s, i)) {
            unsigned int v = kh_key(s, i);
            unsigned int a, b;
            a = v >> 16;
            b = v &  0xFFFF;
            nvgMoveTo(vg, bodies.centers.a[a].x, bodies.centers.a[a].y);
            nvgLineTo(vg, bodies.centers.a[b].x, bodies.centers.a[b].y);
            //printf("%d [%d\t%d]\n", i, a, b);
        }
    }
    nvgStrokeWidth(vg, 1);
    nvgStrokeColor(vg, nvgRGBAf(1.0,1.0,1.0,0.2));
    nvgStroke(vg);

    // release resources
    delaunay2d_release(delaunay);
    kh_destroy(uniq, s);
    kv_destroy(bodies.centers);
    kv_destroy(bodies.bodies );
    
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
