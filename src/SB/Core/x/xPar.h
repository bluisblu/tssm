#ifndef XPAR_H
#define XPAR_H

#include "xMath.h"
#include "xEnt.h"

struct xPECircle
{
    F32 radius;
    F32 deflection;
    xVec3 dir;
};

struct xPESphere
{
    F32 radius;
};

struct xPERect
{
    F32 x_len;
    F32 z_len;
};

struct xPELine
{
    xVec3 pos1;
    xVec3 pos2;
    F32 radius;
};

struct xPEVolume
{
    U32 emit_volumeID;
};

struct xPEOffsetPoint
{
    xVec3 offset;
};

struct xPEVCyl
{
    F32 height;
    F32 radius;
    F32 deflection;
};

struct xPEEntBone
{
    U8 flags;
    U8 type;
    U8 bone;
    U8 pad1;
    xVec3 offset;
    F32 radius;
    F32 deflection;
};

struct xPEEntBound
{
    U8 flags;
    U8 type;
    U8 pad1;
    U8 pad2;
    F32 expand;
    F32 deflection;
};

struct xParEmitterAsset : xBaseAsset
{
    U8 emit_flags;
    U8 emit_type;
    U16 pad;
    U32 propID;
    union
    {
        xPECircle e_circle;
        xPESphere e_sphere;
        xPERect e_rect;
        xPELine e_line;
        xPEVolume e_volume;
        xPEOffsetPoint e_offsetp;
        xPEVCyl e_vcyl;
        xPEEntBone e_entbone;
        xPEEntBound e_entbound;
    };
    U32 attachToID;
    xVec3 pos;
    xVec3 vel;
    F32 vel_angle_variation;
    U32 cull_mode;
    F32 cull_dist_sqr;
};

struct xParGroup
{
};

struct xParInterp
{
    F32 val[2];
    U32 interp;
    F32 freq;
    F32 oofreq;
};

struct xParEmitterPropsAsset : xBaseAsset
{
    U32 parSysID;
    union
    {
        xParInterp rate;
        xParInterp value[1];
    };
    xParInterp life;
    xParInterp size_birth;
    xParInterp size_death;
    xParInterp color_birth[4];
    xParInterp color_death[4];
    xParInterp vel_scale;
    xParInterp vel_angle;
    xVec3 vel;
    U32 emit_limit;
    F32 emit_limit_reset_time;
};

struct xParEmitterCustomSettings : xParEmitterPropsAsset
{
    U32 custom_flags;
    U32 attachToID;
    xVec3 pos;
    xVec3 last_emit_position;
    xVec3 vel;
    xVec3 last_emit_velocity;
    F32 vel_angle_variation;
    U8 rot[3];
    U8 padding;
    F32 radius;
    F32 emit_interval_current;
    xBase* emit_volume;
};

struct xParSys
{
};

struct xParEmitter : xBase
{
    xParEmitterAsset* tasset;
    xParGroup* group;
    xParEmitterPropsAsset* prop;
    U8 rate_mode;
    xVec3 last_emit_position;
    xVec3 last_emit_velocity;
    F32 rate;
    F32 rate_time;
    F32 rate_fraction;
    F32 rate_fraction_cull;
    U8 emit_flags;
    U8 emit_pad[3];
    U8 rot[3];
    xModelTag tag;
    F32 oocull_distance_sqr;
    F32 distance_to_cull_sqr;
    void* attachTo;
    xParSys* parSys;
    xBase* emit_volume;
    xVec3 last_attach_loc;
};

struct zParEmitter : xParEmitter
{
};

#endif
