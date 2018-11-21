#pragma once

#include "../Vec.h"

// Path - vector of points
// Poly - vector of paths

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct mgClipPoint {int64_t x, y;} mgClipPoint;
    typedef Vec(mgClipPoint) mgClipPath;
    typedef Vec(mgClipPath ) mgClipPoly;

    void mgClipShapes(
       mgClipPoly* subj,
       mgClipPoly* clip,
       mgClipPoly* result);

    // Clip Path
    mgClipPath* kpath_new();
    void mgClipPathFree    (mgClipPath* path);
    void mgClipPathAddPoint(mgClipPath* path, mgClipPoint* point);
    void mgClipPathPrint   (mgClipPath* path);

    // Clip Paths
    mgClipPoly* mgClipPolyNew();
    mgClipPath*  mgClipPolyAddNewPath(mgClipPoly* paths);
    void mgClipPolyFree   (mgClipPoly* paths);
    void mgClipPolyAddPath(mgClipPoly* paths, mgClipPath* path);
    void mgClipPolyPrint  (mgClipPoly* paths);

#ifdef __cplusplus
}
#endif
