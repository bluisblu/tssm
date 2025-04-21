#ifndef XPLAYER_H
#define XPLAYER_H

#include "xEnt.h"

struct zEnt : xEnt
{
    xAnimTable* atbl;
};

struct zPlayer : zEnt
{
    xVec3 trigLastFramePos;
    S32 zPlayerFlags;
    U32 lorezModelID;
    xModelInstance* lorezModel;
    xModelInstance* hirezModel;

    void PostRenderCleanup();
    void RenderCustomUI();
    void zPlayerEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget,
                        U32 toParamWidgetID);
    U32 TalkDoneCheck(xAnimTransition* tran);
    U32 TalkCheck(xAnimTransition* tran);
    U8 AllowInteraction();
    void AttackHit();
    xEnt* GetGrabbed();
};

#endif
