#ifndef XENV_H
#define XENV_H

#include "xModel.h"
#include "xJSP.h"

struct iEnvMatOrder
{
    U16 jspIndex;
    U16 nodeIndex;
    S32 matGroup;
    RpAtomic* atomic;
    xJSPNodeInfo* nodeInfo;
};

struct iEnv
{
    RpWorld* world;
    RpWorld* collision;
    RpWorld* fx;
    RpWorld* camera;
    S32 jsp_count;
    U32* jsp_aid;
    xJSPHeader** jsp_list;
    xBox* jsp_bound;
    S32* jsp_visibilityCount;
    S32 jspMatOrderCount;
    iEnvMatOrder* jspMatOrderList;
    RpLight* light[2];
    RwFrame* light_frame[2];
    S32 memlvl;
    U16 numOpaque;
    U16 numTransparent;
};

struct xEnv
{
    iEnv* geom;
    iEnv ienv;
    xLightKit* lightKit;
};

#endif
