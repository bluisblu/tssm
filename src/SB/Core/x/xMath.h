#ifndef XMATH_H
#define XMATH_H

#include "xMem.h"

struct xVec2
{
    F32 x;
    F32 y;
};

struct xVec3
{
    union
    {
        RwV3d m_RwV3d;
        F32 x;
    };
    F32 y;
    F32 z;
    F32 a[3];
};

struct xBox
{
    xVec3 upper;
    xVec3 lower;
};

struct xQuat
{
    xVec3 v;
    F32 s;
};

struct _class_20
{
    xVec3* verts;
};

struct xMat3x3
{
    xVec3 right;
    S32 flags;
    xVec3 up;
    U32 pad1;
    xVec3 at;
    U32 pad2;
};

struct xMat4x3 : xMat3x3
{
    xVec3 pos;
    U32 pad3;
};

struct xRot
{
    xVec3 axis;
    F32 angle;
};

struct xCylinder
{
    xVec3 center;
    F32 r;
    F32 h;
};

struct xSphere
{
    xVec3 center;
    F32 r;
};

struct xCoef
{
    F32 a[4];
};

struct xCoef3
{
    xCoef x;
    xCoef y;
    xCoef z;
};

struct xSpline3
{
    U16 type;
    U16 flags;
    U32 N;
    U32 allocN;
    xVec3* points;
    F32* time;
    xVec3* p12;
    xVec3* bctrl;
    F32* knot;
    xCoef3* coef;
    U32 arcSample;
    F32* arcLength;
};

struct xParabola
{
    xVec3 initPos;
    xVec3 initVel;
    F32 gravity;
    F32 minTime;
    F32 maxTime;
};

#endif
