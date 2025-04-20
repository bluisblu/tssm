#include <types.h>
#include "xWad3.h"

struct xShadowSimplePoly
{
    xVec3 vert[3];
    xVec3 norm;
};

struct xShadowSimpleCache
{
    U16 flags;
    U8 alpha;
    U8 pad;
    U32 collPriority;
    xVec3 pos;
    xVec3 at;
    F32 tol_movement;
    F32 radiusOptional;
    xEnt* castOnEnt;
    xShadowSimplePoly poly;
    F32 envHeight;
    F32 shadowHeight;
    union
    {
        U32 raster;
        RwRaster* ptr_raster;
    };
    F32 dydx;
    F32 dydz;
    F32 corner[4];
    void* collSkipsItem;
};
