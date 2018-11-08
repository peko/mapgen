#include <stdlib.h>
#include <stdio.h>

#include "clipper_c.h"

int main(int argc, char** argv) {
    kpaths_t* subj   = kpaths_new();
    kpaths_t* clip   = kpaths_new();
    kpaths_t* result = kpaths_new();
    {
        // no need to free, automaticaly cleared by kpaths_free
        kpath_t* subj_path = kpaths_add_new_path(subj);
        kpath_t* clip_path = kpaths_add_new_path(clip);
        for(size_t i=0; i<10; i++) {
            kpath_add_point(subj_path,&(kpoint_t){rand()%100, rand()%100});
            kpath_add_point(clip_path,&(kpoint_t){rand()%100, rand()%100});
        }
        clipp_kpaths(subj, clip, result);
        printf("Clippig result:\n");
        kpaths_print(result);
    }
    
    kpaths_free(result );
    kpaths_free(clip   );
    kpaths_free(subj   );
}
