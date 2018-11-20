#ifndef AC_KVEC_H
#define AC_KVEC_H

#include <stdlib.h>

#define kv_roundup32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

#define Vec(type) struct { size_t n, m; type *a; }
#define VecInit(v) ((v).n = (v).m = 0, (v).a = 0)
#define VecDestroy(v) free((v).a)
#define VecA(v, i) ((v).a[(i)])
#define VecPop(v) ((v).a[--(v).n])
#define VecSize(v) ((v).n)
#define VecMax(v) ((v).m)

#define VecResize(type, v, s)  ((v).m = (s), (v).a = (type*)realloc((v).a, sizeof(type) * (v).m))

#define VecCopy(type, v1, v0) do {                          \
        if ((v1).m < (v0).n) VecResize(type, v1, (v0).n);   \
        (v1).n = (v0).n;                                    \
        memcpy((v1).a, (v0).a, sizeof(type) * (v0).n);      \
    } while (0)                                             \

#define VecPush(type, v, x) do {                                    \
        if ((v).n == (v).m) {                                       \
            (v).m = (v).m? (v).m<<1 : 2;                            \
            (v).a = (type*)realloc((v).a, sizeof(type) * (v).m);    \
        }                                                           \
        (v).a[(v).n++] = (x);                                       \
    } while (0)

#define VecPushp(type, v) (((v).n == (v).m)?                            \
                           ((v).m = ((v).m? (v).m<<1 : 2),              \
                            (v).a = (type*)realloc((v).a, sizeof(type) * (v).m), 0) \
                           : 0), ((v).a + ((v).n++))

#define Veca(type, v, i) (((v).m <= (size_t)(i)? \
                          ((v).m = (v).n = (i) + 1, kv_roundup32((v).m), \
                           (v).a = (type*)realloc((v).a, sizeof(type) * (v).m), 0) \
                          : (v).n <= (size_t)(i)? (v).n = (i) + 1 \
                          : 0), (v).a[(i)])

#endif
