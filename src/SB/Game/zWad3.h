#ifndef ZWAD3_H
#define ZWAD3_H

#include <PowerPC_EABI_Support\MSL_C\MSL_Common/printf.h>

#include "zGlobals.h"
#include "xString.h"
#include "string.h"
#include "xstransvc.h"
#include "zVolume.h"
#include "xEvent.h"
#include "zVar.h"
#include "xutil.h"
#include "zSurface.h"
#include "zEnt.h"
#include "zEntPickup.h"
#include "xScrFx.h"
#include "zNPCMgr.h"
#include "xPar.h"
#include "xParMgr.h"
#include "zParPTank.h"
#include "xPtankPool.h"
#include "xFX.h"
#include "zParSys.h"
#include "zActionLine.h"
#include "zLight.h"
#include "zLightning.h"
#include "xShadow.h"
#include "zDiscoFloor.h"
#include "xCutscene.h"
#include "zNPCGlyph.h"
#include "zNPCHazard.h"
#include "zEntButton.h"
#include "zCutsceneMgr.h"
#include "zUIFont.h"
#include "zUI.h"
#include "zTalkBox.h"
#include "xFog.h"
#include "xTimer.h"
#include "xDecal.h"
#include "xEntBoulder.h"
#include "zGrid.h"
#include "zMusic.h"
#include "xMarkerAsset.h"
#include "zCamera.h"
#include "zCamMarker.h"
#include "zCameraFly.h"
#include "zCameraTweak.h"
#include "zEntTeleportBox.h"
#include "xSystem.h"
#include "zDispatcher.h"
#include "zConditional.h"
#include "zAnimList.h"
#include "zParCmd.h"
#include "zEntDestructObj.h"
#include "zRenderState.h"
#include "xModel.h"
#include "zScene.h"
#include "xScene.h"
#include "zSaveLoad.h"
#include "iTRC.h"
#include "iSystem.h"
#include "zHud.h"
#include "zRewards.h"
#include "xHudMeter.h"

// Needs moved to another file because zWad2 and zWad3 use this struct
// not everything is here?
struct zPlayerSB : zPlayer
{
    // padded to get  GetTreasureChestHud to match.
    // All structs that are used here are updated to be correct.
    // Assumably the dwarf is simply wrong
    U64 padding;
    U32 padding2;
    U8 inited;
    ManlinessMeter manlinessMeter;
    KrabbyPattyMeter krabbyPattyMeter;
    TreasureChest treasureChest;
    GoofyGooberMeter goofyGooberMeter;
    NitroMeter nitroMeter;

    TreasureChest* GetTreasureChestHud();
    void SetCamera();
    void LoadCheckPoint();
    void StoreCheckPoint();
    void HandleEvent(xBase* from, U32 thisEvent, F32* toParam, xBase* toParamWidget,
                     U32 toParamWidgetID);
    void RenderEffects();
    void Render();
    void Update(F32 dt);
    void Reset();
    void Init(xEntAsset* asset);
    void Init(xEntAsset* asset, S32 playerIdx);
};

#endif
