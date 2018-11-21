#pragma once

#include <chipmunk/chipmunk_private.h>
#include "Vec.h"

typedef Vec(cpVect) cpPolyline;

typedef struct mgRoom mgRoom;
typedef struct mgLink {
    mgRoom* a;
    mgRoom* b;
    float length;
} mgLink;
typedef Vec(mgLink) mgLinkV;

