#ifndef ZWADUI_H
#define ZWADUI_H

#include "zEnt.h"

#include "xMgr.h"
#include "xIni.h"
#include "xDynAsset.h"
#include "iColor.h"
#include "xCM.h"
#include "xGroup.h"
#include "zTextbox.h"
#include "zVar.h"
#include "zConditional.h"

struct zUIAsset : xDynAsset
{
    F32 x;
    F32 y;
    F32 z;
    F32 width;
    F32 height;
    S32 flags;
    U32 color;
    U32 selectedMotion;
    U32 unselectedMotion;
    U8 brightness;
    U8 pad[3];
    U32 autoMenuUp;
    U32 autoMenuDown;
    U32 autoMenuLeft;
    U32 autoMenuRight;
    U32 custom;
    U32 customWidget;
};

struct zUIMotionAsset : xBaseAsset
{
    U8 cmdCount;
    U8 in;
    U8 pad[2];
    U32 cmdSize;
    F32 totalTime;
    F32 loopTime;
};

struct zUI;

struct zUIMotionFrame
{
    F32 offsetX;
    F32 offsetY;
    F32 scaleX;
    F32 scaleY;
    F32 centerScaleX;
    F32 centerScaleY;
    F32 textScaleX;
    F32 textScaleY;
    F32 rotation;
    F32 offsetU;
    F32 offsetV;
    xColor_tag color;
    U8 brightness;
};

struct zUICustom
{
    zUI* ui;

    void* __nw(U32 size);
    void PostApplyMotionFrame(zUIMotionFrame*);
    void PreApplyMotionFrame(zUIMotionFrame*);
    void PostInitMotion();
    void PreInitMotion();
    void Exit();
    void PostRender();
    U8 PreAutoMenu();
    U8 PreRender();
    void PostUpdate(float);
    void PreUpdate(float);
    U8 HandleEvent(xBase*, unsigned int, const float*, xBase*, unsigned int);
    void ResetMotion();
    void Reset();
    void Setup();
    void Init();
    U8 IsSystem();
    U8 IsSignalAllowed();
};

struct zUICustomConditionallyDisabled : zUICustom
{
    S32 padding; // aligns 2 functions
    _zConditional* m_pCondition;
    U8 m_bCondition;

    U8 PreRender();
    U8 PreAutoMenu();
    void PreUpdate();
};

struct State
{
    F32 x;
    F32 y;
    F32 width;
    F32 height;
    xColor_tag color;
    U8 brightness;
    U8 pad[3];
};

struct zUI : xBase
{
    zUIAsset* asset;
    State current;
    State startMovement;
    F32 z;
    zUIMotionAsset* selectedMotion;
    zUIMotionAsset* unselectedMotion;
    zUICustom* custom;
    U8 visible;
    U8 focus;
    U8 lastFocus;
    U8 selected;
    U8 brighten;
    U8 hdrPass;
    U8 locked;
    zUIMotionAsset* motion;
    F32 motionTime;
    U8 motionFiredEvent;
    U8 motionLoop;

    void Unselect();
    void Select();
    void Invisible();
    void Visible();
    void FocusOff();
    void FocusOn();
    void AutoMenu(void* item, U32 event);
    void DoApplyMotionFrame(zUIMotionFrame* frame);
    void DoInitMotion();
    void ApplyMotion();
    F32 CalcAcceleratedDistance(F32 totalDistance, F32 startTime, F32 endTime, F32 accelTime,
                                F32 decelTime);
    void RunMotion(zUIMotionAsset* asset, U8 loop, U8 reset);
    void DoExit();
    void DoUpdate(F32 dt, U32 buttonsPressed);
    void DoHandleEvent(U32 toEvent, F32* toParam, xBase* toParamWidget);
    void DoResetMotion();
    void DoReset();
    void DoSetup();
    void Signal(xBase* from, U32 event);
    U8 IsUIText();
};

struct zUICustomImageSlideshow : zUICustom
{
    U32 m_CurrentImage;
    U32 m_NumImages;
    xGroup* m_TextureGroup;

    void UpdateTexture();
    void SetTextureList(xGroup* textureList);
    U8 HandleEvent(U32 toEvent);
    void Reset();
};

struct zUIControllerAsset : zUIAsset
{
};

struct zUIController : zUI
{
    void DoInit();
    U32 GetSortKey();
    void DoRender() const;
};

struct zUITextAsset : zUIAsset
{
    U32 textID;
    U8 font;
    U8 fontSizeW;
    U8 fontSizeH;
    U8 fontSpacingX;
    U8 fontSpacingY;
    U8 textBoxInsetTop;
    U8 textBoxInsetLeft;
    U8 textBoxInsetRight;
    U8 textBoxInsetBottom;
    U8 justifyX;
    U8 justifyY;
    U8 textFlags;
    xColor_tag shadowColor;
    F32 shadowOffsetX;
    F32 shadowOffsetY;
    F32 textScaleX;
    F32 textScaleY;
};

struct zUIUserStringAsset : zUITextAsset
{
    U8 hardMaxChars;
    U8 softMaxChars;
};

struct zUIText : zUI
{
    xColor_tag shadowColor;
    char* text;

    void DoApplyMotionFrame(zUIMotionFrame* frame);
    void DoHandleEvent(xBase* from, U32 toEvent, F32* toParam, xBase* toParamWidget,
                       U32 toParamWidgetID);
    void DoRender();
    void setup_textbox(xtextbox& tb, zUITextAsset* asset);
    void DoReset();
    void DoInit();
    U8 IsUIText();
    U32 GetSortKey();
};

struct zUIUserString : zUIText
{
    char* textBuffer;
    S32 length;
    S32 maxLength;

    U32 GetSortKey() const;
    void DoHandleEvent(xBase* from, U32 toEvent, F32* toParam, xBase* toParamWidget,
                       U32 toParamWidgetID);
    void DoReset();
    void DoInit();
};

#endif
