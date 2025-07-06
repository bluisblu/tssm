#ifndef XFXHIGHDYNAMICRANGE_H
#define XFXHIGHDYNAMICRANGE_H

#include "xFX.h"
#include "xGlobals.h"
#include "xModelBlur.h"
#include "xFX.h"

struct downsample_pass_info
{
    xVec2* offset;
    S32 resamples;
};

struct interpolator
{
    F32 value;
    F32 start;
    F32 end;
    F32 t;
    F32 trate;
};

#endif
