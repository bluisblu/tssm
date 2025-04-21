#ifndef XCOLLIS_H
#define XCOLLIS_H
#include "xModel.h"

struct tri_data_0
{
    U32 index;
    F32 r;
    F32 d;
};

struct _class_8
{
    F32 t;
    F32 u;
    F32 v;
};

struct xCollis
{
    U32 flags;
    U32 oid;
    void* optr;
    xModelInstance* mptr;
    F32 dist;
    F32 test_dist;
    xVec3 norm;
    xVec3 tohit;
    xVec3 depen;
    xVec3 hdng;
    union
    {
        _class_8 tuv;
        tri_data_0 tri;
    };
};

struct anim_coll_data
{
};

#endif
