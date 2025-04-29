#include "iScrFX.h"
#include "rwcore.h"

extern RwRaster* g_rast_gctapdance;
extern S32 g_alreadyTriedAlloc;
extern _iMotionBlurData sMBD;
extern U32 sMotionBlurEnabled;

void iScrFxLensFlare_Reset()
{
}

S32 iScrFxCameraDestroyed(RwCamera* pCamera)
{
    if (sMBD.motionBlurFrontBuffer != NULL)
    {
        RwRasterDestroy(sMBD.motionBlurFrontBuffer);
        sMBD.motionBlurFrontBuffer = NULL;
        return 1;
    }
    return 0;
}

void iScrFxCameraEndScene(RwCamera* pCamera)
{
}

void iScrFxCameraCreated(RwCamera* pCamera)
{
    sMBD.motionBlurAlpha = 0x90;
    sMBD.motionBlurFrontBuffer = NULL;
    sMBD.index[0] = 0;
    sMBD.index[1] = 1;
    sMBD.index[2] = 2;
    sMBD.index[3] = 0;
    sMBD.index[4] = 2;
    sMBD.index[5] = 3;
}

void iScrFxBegin()
{
}

void iScrFxInit()
{
}
