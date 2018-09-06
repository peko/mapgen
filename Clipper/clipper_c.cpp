#include <string.h>

#include "clipper.h"
#include "clipper_c.h"

using namespace clipperlib;

void kpathsToPaths(kpaths_t* kpaths, Paths& paths);
void pathsToKpaths(Paths& paths, kpaths_t* kpaths);

extern "C" void clip(
    kpaths_t* ksubj,
    kpaths_t* kclip,
    kpaths_t* kresult) {

    ClipType clip_type = ctIntersection;
    FillRule fill_rule = frNonZero;
    Paths subj, clip, result;

    kpathsToPaths(ksubj,   subj  );
    kpathsToPaths(kclip,   clip  );
    kpathsToPaths(kresult, result);
    
    Clipper clipper;
    clipper.AddPaths(subj, ptSubject);
    clipper.AddPaths(clip, ptClip);
    clipper.Execute(clip_type, result, fill_rule);

    pathsToKpaths(result, kresult);
}

void kpathsToPaths(
    kpaths_t* kpaths, 
    Paths& paths) {

    paths.resize(kpaths->n);
    // iterate through paths
    for(size_t pid=0; pid<kpaths->n; pid++) {
        // copy every path to vector
        kpath_t* kpath = &kpaths->a[pid];
        paths[pid].assign(
            (Point64*) (kpath->a           ), 
            (Point64*) (kpath->a + kpath->n));
    }
}

void pathsToKpaths(Paths& paths, kpaths_t* kpaths) {
    if(kpaths->n != 0 || kpaths->a !=0) kpaths_free(kpaths);
    for(size_t pid=0; pid<paths.size(); pid++) {
        Path& path = paths[pid];
        kpath_t* kpath = kpath_new();
        kv_resize(kpoint_t, *kpath, path.size());
        memcpy(kpath->a, path.data(), sizeof(kpoint_t)*(kpath->n));
    }
}

// kpath - point collection

kpath_t* kpath_new() {
    kpath_t* kpath = (kpath_t*) calloc(1, sizeof(kpath_t));
    return kpath;
}

void kpath_free(kpath_t* kpath) {
    free(kpath->a);
}

void kpath_add_point(kpath_t* kpath, kpoint_t* kpoint) {
    kv_push(kpoint_t, *kpath, *kpoint);
}

// kpaths - path collection

kpaths_t* kpaths_new() {
    kpaths_t* kpaths = (kpaths_t*) calloc(1, sizeof(kpaths_t));
    return kpaths;
}

void kpaths_free(kpaths_t* kpaths) {
    for(size_t pid=0; pid<kpaths->n; pid++) {
        kpath_free(&kpaths->a[pid]);
    }
    free(kpaths->a);
    *kpaths = (kpaths_t) {0,0,0};
}

void kpaths_add_path(kpaths_t* kpaths, kpath_t* kpath) {
    kv_push(kpath_t, *kpaths, *kpath);
}

