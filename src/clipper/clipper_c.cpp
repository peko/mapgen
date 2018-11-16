#include <iostream>

#include <string.h>
#include <inttypes.h>

#include "clipper.h"
#include "clipper_c.h"

using namespace clipperlib;

static void MgToPaths(mgClippPaths* kpaths, Paths& paths);
static void PathsToMg (Paths& paths, mgClippPaths* kpaths);

extern "C" void 
mgClippShapes(
    mgClippPaths* subj,
    mgClippPaths* clip,
    mgClippPaths* result) {

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
    mgClippPaths* paths, 
    Paths&        outPaths) {

    outPaths.resize(paths->n);
    // iterate through paths
    for(size_t i=0; i<paths->n; i++) {
        // copy every path to vector
        mgClippPath* path = &paths->a[i];
        outPaths[i].assign(
            (Point64*) (paths->a           ), 
            (Point64*) (paths->a + paths->n));
    }
}

static void 
PathsToMg(
    Paths&        paths, 
    mgClippPaths* outPaths) {

    if(outPaths->n != 0 || outPaths->a !=0) mgClippPathsFree(outPaths);
    for(size_t i=0; i<paths.size(); i++) {
        mgClippPath* _path = mgClippPathsAddNewPath(outPaths);
        kv_resize(mgClippPoint, *_path, paths[i].size());
        _path->n = paths[i].size();
        memcpy(_path->a, paths[i].data(), sizeof(mgClippPoint)*(_path->n));
    }
}

// mgClippPath

mgClippPath*
mgClippPathNew() {
    mgClippPath* kpath = (mgClippPath*) calloc(1, sizeof(mgClippPath));
    return kpath;
}

void 
mgClippPathFree(mgClippPath* path) {
    free(path->a);
    *path = (mgClippPath) {0,0,0};
}

void
mgClippPathAddPoint(
    mgClippPath*  path, 
    mgClippPoint* point) {
    kv_push(mgClippPoint, *path, *point);
}

void 
mgClippPathPrint(mgClippPath* kpath) {
    for(size_t i=0; i<kpath->n; i++) {
        printf("\t%d: {%" PRId64 ",%" PRId64 "}\n", i, kpath->a[i].x, kpath->a[i].y);
    }
}

// mgClippPaths

mgClippPaths* 
mgClippPathsNew() {
    mgClippPaths* paths = (mgClippPaths*) calloc(1, sizeof(mgClippPaths));
    return paths;
}

void 
mgClippPathFree(mgClippPaths* paths) {
    for(size_t i=0; i<paths->n; i++) {
        mgClippPathFree(&paths->a[i]);
    }
    free(paths->a);
    *paths = (mgClippPaths) {0,0,0};
}

void
mgClippPathAddPath(
    mgClippPaths* paths, 
    mgClippPath*      path) {
    kv_push(mgClippPath, *paths, *path);
}

mgClippPath* 
mgClippPathAddNewPath(mgClippPaths* paths) {
    kv_push(mgClippPath, *paths, ((mgClippPath){0,0,0}));
    return &paths->a[paths->n-1];
}

void
mgClippPathPrint(mgClippPaths* paths) {
    for(size_t i=0; i<paths->n; i++) {
        printf("path[%d]", i);
        mgClippPathPrint(&paths->a[i]);
    }
}
