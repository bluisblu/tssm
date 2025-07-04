#include "zWadUI.h"
#include "zUI.h"

// I believe this is correct?
// Aren't these initialized to 0 if no initialization is given?

zUIController* UIControl = 0;
zUIControllerAsset* UIControlAsset = 0;
zUIUserString* zUIString = 0;
zUIUserStringAsset* zUIStringAsset = 0;

unsigned char zUICustomConditionallyDisabled::PreRender()
{
    return m_bCondition; // struct sees to be missing some data
}

unsigned char zUICustomConditionallyDisabled::PreAutoMenu()
{
    return m_bCondition; // struct sees to be missing some data
}

void zUICustomImageSlideshow::SetTextureList(xGroup* m_TextureGroup)
{
    xGroupGetCount(m_TextureGroup);
}

void zUICustom::PostApplyMotionFrame(zUIMotionFrame*)
{
}

void zUICustom::PreApplyMotionFrame(zUIMotionFrame*)
{
}

void zUICustom::PostInitMotion()
{
}

void zUICustom::PreInitMotion()
{
}

void zUICustom::Exit()
{
}

void zUICustom::PostRender()
{
}

U8 zUICustom::PreAutoMenu()
{
    return 1;
}

U8 zUICustom::PreRender()
{
    return 1;
}

void zUICustom::PostUpdate(float)
{
}

void zUICustom::PreUpdate(float)
{
}

U8 zUICustom::HandleEvent(xBase*, unsigned int, const float*, xBase*, unsigned int)
{
    return 1;
}

void zUICustom::ResetMotion()
{
}

void zUICustom::Reset()
{
}

void zUICustom::Setup()
{
}

void zUICustom::Init()
{
}

void zUIController_Init(zUIController*, zUIControllerAsset*)
{
}

void zUIController_Init(xBase&, xDynAsset&, unsigned long)
{
    zUIController* UIControl = 0;
    zUIControllerAsset* UIControlAsset = 0;
    zUIController_Init(UIControl, UIControlAsset);
}

void zUIController::DoRender() const
{
}

void zUIUserString_Init(zUIUserString*, zUIUserStringAsset*)
{
}

void zUIUserString_Init(xBase&, xDynAsset&, unsigned long)
{
    zUIUserString* zUIString = 0;
    zUIUserStringAsset* zUIStringAsset = 0;
    zUIUserString_Init(zUIString, zUIStringAsset);
}

U32 zUIUserString::GetSortKey() const
{
    return 0;
}
