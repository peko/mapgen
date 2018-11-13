#pragma once

#include "primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

    void mgClippShapes(
       mgClippPaths* subj,
       mgClippPaths* clip,
       mgClippPaths* result);

    // Clip Path
    mgClippPath* kpath_new();
    void mgClippPathFree    (mgClippPath* paths);
    void mgClippPathAddPoint(mgClippPath* paths, mgClippPoint* point);
    void mgClippPathPrint   (mgClippPath* paths);

    // Clip Paths
    mgClippPaths* mgClippPathsNew();
    mgClippPath*  mgClippPathsAddNewPath(mgClippPaths* paths);
    void mgClippPathsFree   (mgClippPaths* paths);
    void mgClippPathsAddPath(mgClippPaths* paths, mgClippPath* path);
    void mgClippPathsPrint  (mgClippPaths* paths);
    
#ifdef __cplusplus
}
#endif
