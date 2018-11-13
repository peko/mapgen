#pragma once

#include <chipmunk/chipmunk_private.h>
#include "kvec.h"

typedef struct {int64_t x, y;} mgClippPoint;
typedef kvec_t(mgClippPoint) mgClippPath;
typedef kvec_t(mgClippPath ) mgClippPaths;

typedef kvec_t(cpVect) cpVectVec;

typedef struct mgRoom mgRoom;
typedef struct mgLink {
    mgRoom* a;
    mgRoom* b;
    float length;
} mgLink;
typedef kvec_t(mgLink) mgLinkVec;

