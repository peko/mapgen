#include <string.h>
#include <inttypes.h>

#include "clipper.h"
#include "clipper_c.h"

using namespace clipperlib;

static void kpaths_to_paths(kpaths_t* kpaths, Paths& paths);
static void paths_to_kpaths(Paths& paths, kpaths_t* kpaths);

extern "C" void clipp_kpaths(
    kpaths_t* ksubj,
    kpaths_t* kclip,
    kpaths_t* kresult) {

    ClipType clip_type = ctIntersection;
    FillRule fill_rule = frNonZero;
    Paths subj, clip, result;

    kpaths_to_paths(ksubj,   subj  );
    kpaths_to_paths(kclip,   clip  );
    kpaths_to_paths(kresult, result);
    
    Clipper clipper;
    clipper.AddPaths(subj, ptSubject);
    clipper.AddPaths(clip, ptClip);
    clipper.Execute(clip_type, result, fill_rule);

    paths_to_kpaths(result, kresult);
}

static void kpaths_to_paths(
    kpaths_t* kpaths, 
    Paths& paths) {

    paths.resize(kpaths->n);
    // iterate through paths
    for(size_t i=0; i<kpaths->n; i++) {
        // copy every path to vector
        kpath_t* kpath = &kpaths->a[i];
        paths[i].assign(
            (Point64*) (kpath->a           ), 
            (Point64*) (kpath->a + kpath->n));
    }
}

static void paths_to_kpaths(Paths& paths, kpaths_t* kpaths) {
    if(kpaths->n != 0 || kpaths->a !=0) kpaths_free(kpaths);
    for(size_t i=0; i<paths.size(); i++) {
        Path& path = paths[i];
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

void kpath_print(kpath_t* kpath) { 
    for(size_t i=0; i<kpath->n; i++) {
        printf("\t%d: {%" PRId64 ",%" PRId64 "}\n", i, kpath->a[i].x, kpath->a[i].y);
    }
}

// kpaths - path collection

kpaths_t* kpaths_new() {
    kpaths_t* kpaths = (kpaths_t*) calloc(1, sizeof(kpaths_t));
    return kpaths;
}

void kpaths_free(kpaths_t* kpaths) {
    for(size_t i=0; i<kpaths->n; i++) {
        kpath_free(&kpaths->a[i]);
    }
    free(kpaths->a);
    *kpaths = (kpaths_t) {0,0,0};
}

void kpaths_add_path(kpaths_t* kpaths, kpath_t* kpath) {
    kv_push(kpath_t, *kpaths, *kpath);
}

void kpaths_print(kpaths_t* kpaths) {
    for(size_t i=0; i<kpaths->n; i++) {
        printf("path[%d]", i);
        kpath_print(&kpaths->a[i]);
    }
}
