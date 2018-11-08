#pragma once

#include <kvec.h>

typedef struct {int64_t x, y;} kpoint_t;
typedef kvec_t(kpoint_t) kpath_t;
typedef kvec_t(kpath_t) kpaths_t;

#ifdef __cplusplus
extern "C" {
#endif
    void clipp_kpaths(
       kpaths_t* subj,
       kpaths_t* clip,
       kpaths_t* result);

    kpath_t* kpath_new();
    void kpath_free(kpath_t* kpath);
    void kpath_add_point(kpath_t* kpath, kpoint_t* kpoint);
    void kpath_print(kpath_t* kpath);
    
    kpaths_t* kpaths_new();
    void kpaths_free(kpaths_t* kpaths);
    void kpaths_add_path(kpaths_t* kpaths, kpath_t* kpath);
    kpath_t* kpaths_add_new_path(kpaths_t* kpaths);
    void kpaths_print(kpaths_t* kpaths);
#ifdef __cplusplus
}
#endif
