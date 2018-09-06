#pragma once

#include "kvec.h"

typedef struct {uint64_t x, y;} point_t;
typedef kvec_type(point_t) path_t;
typedef kvec_type(path_t) paths_t;

extern "C" clip(
    paths_t* subj,
    paths_t* clip,
    paths_t* result);
