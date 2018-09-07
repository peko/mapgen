#include <stdlib.h>
#include "clipper_c.h"

int main(int argc, char** argv) {
    kpaths_t* subj   = kpaths_new();
    kpaths_t* clip   = kpaths_new();
    kpaths_t* result = kpaths_new();
    {
        // no need to free, automaticaly cleared by kpaths_free
        kpath_t* subj_path = kpath_new();
        kpath_t* clip_path = kpath_new();
        for(size_t i=0; i<10; i++) {
            kpath_add_point(subj_path,&(kpoint_t){rand()%100, rand()%100});
            kpath_add_point(clip_path,&(kpoint_t){rand()%100, rand()%100});
        }
        kpaths_add_path(subj, subj_path);
        kpaths_add_path(clip, clip_path);        
        kpaths_print(subj);
        kpaths_print(clip);
        clipp_kpaths(subj, clip, result);
        kpaths_print(result);
    }
    
    kpaths_free(result );
    kpaths_free(clip   );
    kpaths_free(subj   );
}
