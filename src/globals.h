#pragma once

#define WIDTH  800
#define HEIGHT 800

#define PI  3.1415926
#define PI2 PI*2.0

#define VISION_RESOLUTION 64
#define VISION_LAYERS     2
#define VISION_ANGLE      PI/2.0
#define VISION_RANGE      500.0
#define VISION_DAMPING    0.1
#define SPACE_DAMPING     0.1
#define FORCE_MULTIPLIER  100.0
#define Y_COUNT           3
#define X_COUNT           (VISION_RESOLUTION*VISION_LAYERS)
#define CORTEX_SIZE       (Y_COUNT+X_COUNT)
#define MEMORY_SIZE       5000

enum COLLISION_TYPES {
   ANT = 1,
   WALL,
   BOX,
   FOOD,
};

