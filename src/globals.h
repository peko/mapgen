#pragma once

#include <chipmunk/chipmunk_private.h>
#include "kvec.h"

#define WIDTH  800
#define HEIGHT 800

#define PI  3.1415926
#define PI2 PI*2.0

#define TUNNEL_R 10.0

#define SPACE_DAMPING     0.1
#define FORCE_MULTIPLIER  100.0
#define Y_COUNT           3
#define X_COUNT           (VISION_RESOLUTION*VISION_LAYERS)
#define CORTEX_SIZE       (Y_COUNT+X_COUNT)
#define MEMORY_SIZE       5000

