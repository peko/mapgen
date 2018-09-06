#pragma once

#include "kvec.h"

typedef struct {int64_t x, y;} kpoint_t;
typedef kvec_t(kpoint_t) kpath_t;
typedef kvec_t(kpath_t) kpaths_t;

#ifdef __cplusplus
extern "C" {
#endif
    void clip(
       kpaths_t* subj,
       kpaths_t* clip,
       kpaths_t* result);

    kpath_t* kpath_new();
    void kpath_free(kpath_t* kpath);
    void kpath_add_point(kpath_t* kpath, kpoint_t* kpoint);

    kpaths_t* kpaths_new();
    void kpaths_free(kpaths_t* kpaths);
    void kpaths_add_path(kpaths_t* kpaths, kpath_t* kpath);
#ifdef __cplusplus
}
#endif
