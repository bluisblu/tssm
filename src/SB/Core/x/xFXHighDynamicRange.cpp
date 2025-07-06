#include "xFXHighDynamicRange.h"

// char buffer[16];
// char buffer[16];
// F32 scale;
U8 HDR_brightening;
// F32 overbrighten;
// F32 overbrighten_decay;
// S32 downsamples;
// U8 blur_faster;
// F32 colorize_red;
// F32 colorize_green;
// F32 colorize_blue;
// F32 colorize_alpha;
// U8 enabled;
// S32 background_glow_normal;
// S32 background_darken_normal;
// interpolator background_glow;
// interpolator background_darken;
// S32 saved_alpha_test;
// //<unknown fundamental type(0xa510)>* blur_packet[5];
// xVec2 diagonal_00_11_offsets[2];
// xVec2 diagonal_01_10_offsets[2];
// xVec2 square_offsets[4];
// downsample_pass_info downsample_passes_fast[5];
// downsample_pass_info downsample_passes_slow[5];
// xFXCameraTexture blurbuffer[5];
// RwRaster* backbuffer_raster;
// xFXCameraTexture downbuffer[5];
// S32 old_alpha_test;
// S32 our_alpha_test;
// S32 _rpPTankAtomicDataOffset;
// //<unknown fundamental type(0xa510)>* _rwDMAGateSlot;
// xColor_tag g_WHITE;
// U32 ourGlobals[4096];
// xColor_tag g_BLACK;
// xGlobals* xglobals;
// xColor_tag g_MAGENTA;
// U32 FB_YRES;
// U32 FB_XRES;

void xFXHighDynamicRangeResetGlowClamp()
{
    S32 alpha_test;
}

void xFXHighDynamicRangeSetGlowClamp(S32 glow_min, bool dest_test)
{
    S32 iVar1;

    iVar1 = 0xff;
    if (glow_min < 0xff)
    {
        iVar1 = glow_min;
    }
    if (iVar1 < 0)
    {
        iVar1 = 0;
    }
    else
    {
        iVar1 = 0xff;
        if (glow_min < 0xff)
        {
            iVar1 = glow_min;
        }
    }
    RwGameCubeSetAlphaCompare(7, 0, 1, 6, (char)iVar1);
    _rwDlRenderStateSetZCompLoc(FALSE);
}
