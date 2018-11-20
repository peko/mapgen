#include <iostream>

#include <string.h>
#include <inttypes.h>

#include "../mgLevel.h"
#include "clipper.h"
#include "clipper_c.h"

#define CLIPPING_SCALE 10000

using namespace clipperlib;

static void MgToPaths(mgClipPoly* poly, Paths& paths);
static void PathsToMg (Paths& paths, mgClipPoly* poly);

extern "C" void 
mgClipShapes(
    mgClipPoly* subj,
    mgClipPoly* clip,
    mgClipPoly* result) {

    ClipType clipType = ctIntersection;
    FillRule fillRule = frNonZero;
    Paths _subj, _clip, _result;

    MgToPaths(subj,   _subj  );
    MgToPaths(clip,   _clip  );
    MgToPaths(result, _result);
    
    Clipper clipper;
    clipper.AddPaths(_subj, ptSubject);
    clipper.AddPaths(_clip, ptClip);
    clipper.Execute(clipType, _result, fillRule);
    PathsToMg(_result, result);
}

static void 
MgToPaths(
    mgClipPoly* poly, 
    Paths& paths) {

    paths.resize(poly->n);
    // iterate through poly
    for(size_t i=0; i<poly->n; i++) {
        // copy every path to vector
        mgClipPath* path = &poly->a[i];
        paths[i].assign(
            (Point64*) (poly->a           ), 
            (Point64*) (poly->a + poly->n));
    }
}

static void 
PathsToMg(
    Paths& paths, 
    mgClipPoly* poly) {

    if(poly->n != 0 || poly->a !=0) mgClipPolyFree(poly);
    for(size_t i=0; i<paths.size(); i++) {
        mgClipPath* _path = mgClipPolyAddNewPath(poly);
        VecResize(mgClipPoint, *_path, paths[i].size());
        poly->m = poly->n = paths[i].size();
        memcpy(poly->a, paths[i].data(), sizeof(mgClipPoint)*(poly->n));
    }
}

// mgClipPath

mgClipPath*
mgClipPathNew() {
    mgClipPath* path = (mgClipPath*) calloc(1, sizeof(mgClipPath));
    return path;
}

void
mgClipPathFree(mgClipPath* path) {
    if(path->a == NULL) return;
    free(path->a);
    // cleanup pointer
    *path = (mgClipPath) {0, 0, NULL};
}

void
mgClipPathAddPoint(
    mgClipPath*  path, 
    mgClipPoint* point) {
    VecPush(mgClipPoint, *path, *point);
}

void 
mgClipPathPrint(mgClipPath* path) {
    for(size_t i=0; i<path->n; i++) {
        printf("\t%d: {%" PRId64 ",%" PRId64 "}\n", i, path->a[i].x, path->a[i].y);
    }
}

// mgClipPoly

mgClipPoly* 
mgClipPolyNew() {
    mgClipPoly* poly = (mgClipPoly*) calloc(1, sizeof(mgClipPoly));
    return poly;
}

// Free poly
void 
mgClipPolyFree(mgClipPoly* poly) {
    if(poly->a == NULL) return;
    // Iterateh through all paths and free
    for(size_t i=0; i<poly->n; i++) {
        mgClipPathFree(&poly->a[i]);
    }
    // free vector of paths
    free(poly->a);
    // cleanup vector
    *poly = (mgClipPoly) {0, 0, NULL};
}

// Add to poly new existing path
void
mgClipPolyAddPath(
    mgClipPoly* poly, 
    mgClipPath* path) {
    VecPush(mgClipPath, *poly, *path);
}

// Create new path in poly and return it
mgClipPath* 
mgClipPolyAddNewPath(mgClipPoly* poly) {
    // insert to poly new empty path
    VecPush(mgClipPath, *poly, ((mgClipPath){0,0,0}));
    // return last path
    return &poly->a[poly->n-1];
}

void
mgClipPathPrint(mgClipPoly* poly) {
    for(size_t i=0; i<poly->n; i++) {
        printf("path[%d]", i);
        mgClipPathPrint(&poly->a[i]);
    }
}

static Paths
levelToPoly(mgLevel* level) {
    cpSpace* space = (cpSpace*)level;
    cpArray* bodies = space->dynamicBodies;
    Paths paths;
    paths.clear(); 
    for(int i=0; i<bodies->num; i++){
        cpBody* b = (cpBody*)bodies->arr[i];
        cpShape* s = &b->shapeList[0];        
        int count = cpPolyShapeGetCount(s);
        Path p;
        for(int i=0; i<count; i++) {
            cpVect v = cpBodyLocalToWorld(s->body, cpPolyShapeGetVert(s, i));
            p.push_back(Point64(v.x*CLIPPING_SCALE, v.y*CLIPPING_SCALE));
        }
        paths.push_back(p);
    }
    return paths;
}

