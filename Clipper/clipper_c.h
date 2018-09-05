#pragma once

#include "kvec.h"

extern "C" clip(
    uint64_t* subj, 
    size_t subj_cnt, 
    uint64_t* clip
    size_t clip_cnt,
    // output
    uint64_t** result,
    size_t* result_cnt);
