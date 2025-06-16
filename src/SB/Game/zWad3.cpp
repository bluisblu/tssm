#include "zWad3.h"

//                              zVolume

struct PreCalcOcclude
{
    xVec4 DepthVec;
    xVec4 FrustVec[4];
};

extern zVolume* vols;
extern U16 nvols;

extern S32 gOccludeCount;
extern zVolume* gOccludeList[10];
extern S32 gOccludeCalcCount;
extern PreCalcOcclude gOccludeCalc[10];

extern float lbl_803CDFB8;

S32 zVolumeEventCB(xBase*, xBase* to, U32 toEvent, const F32*, xBase*, U32)
{
    zVolume* vol = (zVolume*)to;
    S32 i;

    switch (toEvent)
    {
    case eEventReset:
    {
        vol->Reset();
        break;
    }
    case eEventOccludeOn:
    {
        if (gOccludeCount == 10)
        {
            return 1;
        }

        for (i = 0; i < gOccludeCount; i++)
        {
            if (gOccludeList[i] == vol)
            {
                return 1;
            }
        }

        gOccludeList[gOccludeCount] = vol;
        gOccludeCount++;

        break;
    }
    case eEventOccludeOff:
    {
        for (i = 0; i < gOccludeCount; i++)
        {
            if (gOccludeList[i] == vol)
            {
                gOccludeList[gOccludeCount] = gOccludeList[gOccludeCount - 1];
                gOccludeCount--;

                return 1;
            }
        }

        break;
    }
    }

    return 1;
}

void zVolume_OccludePrecalc(xVec3* camPos)
{
    S32 i;
    S32 j;
    xVec3 corner[5];
    zVolume* vol;
    xVolumeAsset* a;
    F32 c;
    F32 s;
    PreCalcOcclude* calc;
    xVec3 d1;
    xVec3 d2;
    xVec4 locFrustVec[4];
    F32 depthdot;
    F32 camdot;
    F32 testdot1;
    F32 testdot2;

    gOccludeCalcCount = 0;

    for (i = 0; i < gOccludeCount; i++)
    {
        vol = gOccludeList[i];
        a = vol->asset;

        c = icos(a->rot);
        s = isin(a->rot);

        calc = &gOccludeCalc[gOccludeCalcCount];

        corner[0].x = c * (a->bound.box.box.lower.x - a->xpivot) + a->xpivot;
        corner[0].y = a->bound.box.box.lower.y;
        corner[0].z = s * (a->bound.box.box.lower.x - a->xpivot) + a->bound.box.box.lower.z;

        corner[1].y = a->bound.box.box.lower.y;
        corner[1].x = c * (a->bound.box.box.upper.x - a->xpivot) + a->xpivot;
        corner[1].z = s * (a->bound.box.box.upper.x - a->xpivot) + a->bound.box.box.lower.z;

        corner[2].x = c * (a->bound.box.box.upper.x - a->xpivot) + a->xpivot;
        corner[2].y = a->bound.box.box.upper.y;
        corner[2].z = s * (a->bound.box.box.upper.x - a->xpivot) + a->bound.box.box.lower.z;

        corner[3].x = c * (a->bound.box.box.lower.x - a->xpivot) + a->xpivot;
        corner[3].y = a->bound.box.box.upper.y;
        corner[3].z = s * (a->bound.box.box.lower.x - a->xpivot) + a->bound.box.box.lower.z;

        corner[4] = corner[0];

        xVec3Sub(&d1, &corner[1], &corner[0]);
        xVec3Sub(&d2, &corner[2], &corner[0]);

        xVec3Cross((xVec3*)calc, &d1, &d2);
        xVec3Normalize((xVec3*)calc, (xVec3*)calc);

        depthdot = xVec3Dot((xVec3*)calc, &corner[0]);
        camdot = xVec3Dot((xVec3*)calc, camPos);

        if (camdot > depthdot)
        {
            xVec3Inv((xVec3*)calc, (xVec3*)calc);

            camdot = -camdot;
            depthdot = -depthdot;
        }

        if (iabs(camdot - depthdot) >= lbl_803CDFB8)
        {
            calc->DepthVec.w = depthdot;

            for (j = 0; j < 4; j++)
            {
                xVec3Sub(&d1, &corner[j], camPos);
                xVec3Sub(&d2, &corner[j + 1], camPos);
                xVec3Cross((xVec3*)&locFrustVec[j], &d1, &d2);
                xVec3Normalize((xVec3*)&locFrustVec[j], (xVec3*)&locFrustVec[j]);
            }

            testdot1 = xVec3Dot((xVec3*)&locFrustVec[0], &corner[0]);
            testdot2 = xVec3Dot((xVec3*)&locFrustVec[0], &corner[2]);

            if (testdot1 > testdot2)
            {
                for (j = 0; j < 4; j++)
                {
                    xVec3Inv((xVec3*)&locFrustVec[j], (xVec3*)&locFrustVec[j]);
                }
            }

            for (j = 0; j < 4; j++)
            {
                locFrustVec[j].w = xVec3Dot((xVec3*)&locFrustVec[j], &corner[j]);
            }

            calc->FrustVec[0].x = locFrustVec[0].w;
            calc->FrustVec[0].y = locFrustVec[1].w;
            calc->FrustVec[0].z = locFrustVec[2].w;
            calc->FrustVec[0].w = locFrustVec[3].w;

            calc->FrustVec[1].x = locFrustVec[0].x;
            calc->FrustVec[1].y = locFrustVec[1].x;
            calc->FrustVec[1].z = locFrustVec[2].x;
            calc->FrustVec[1].w = locFrustVec[3].x;

            calc->FrustVec[2].x = locFrustVec[0].y;
            calc->FrustVec[2].y = locFrustVec[1].y;
            calc->FrustVec[2].z = locFrustVec[2].y;
            calc->FrustVec[2].w = locFrustVec[3].y;

            calc->FrustVec[3].x = locFrustVec[0].z;
            calc->FrustVec[3].y = locFrustVec[1].z;
            calc->FrustVec[3].z = locFrustVec[2].z;
            calc->FrustVec[3].w = locFrustVec[3].z;

            gOccludeCalcCount++;
        }
    }
}

//                                          zVent

//                                          zVar

char* var_text(const substr& str);
U32 zVarEntryCB_SndFXVol(void*);
U32 zVarEntryCB_SndMusicVol(void*);

extern const char zVar_strings[];

S32 zVarEntryCB_IsVisible(void* arg)
{
    return 0;
}

S32 zVarEntryCB_IsEnabled(void* arg)
{
    return 0;
}

S32 zVarEntryCB_CounterValue(void* arg)
{
    // Given the void argument type, no idea what type this actually is.
    // What we do know that at an offset 5 words in there's a signed S16.
    struct something
    {
        U32 stuff[5];
        S16 theValue;
    };
    return ((something*)arg)->theValue;
}

S32 zVarEntryCB_Minute(void*)
{
    return iGetMinute();
}

S32 zVarEntryCB_Hour(void*)
{
    return iGetHour();
}

S32 zVarEntryCB_Date(void*)
{
    return iGetDay() + iGetMonth() * 0x64;
}

S32 zVarEntryCB_CurrentRoom(void*)
{
    char buffer[16];
    sprintf(buffer, &zVar_strings[0xbaa] /*"%s"*/,
            xUtil_idtag2string(globals.sceneCur->sceneID, 0));

    S32 room = (buffer[2] - '0') * 10 + (buffer[3] - '0');

    // Convert the most significant char to uppercase
    // The lowercase variable doesn't exist in the ps4 symbols but I can't
    // figure out how to get a match otherwise.
    U32 mostSignificantChar = buffer[0];
    bool lowercase = false;
    if (mostSignificantChar >= 'a' && mostSignificantChar <= 'z')
    {
        lowercase = true;
    }
    if (lowercase)
    {
        mostSignificantChar -= 0x20;
    }

    return room + ((mostSignificantChar - 'A') + 1) * 100;
}

S32 zVarEntryCB_CurrentSceneLetter(void*)
{
    char buffer[16];
    sprintf(buffer, &zVar_strings[0x10] /*"%s"*/, xUtil_idtag2string(globals.sceneCur->sceneID, 0));

    // Convert the most significant char to uppercase
    // The lowercase variable doesn't exist in the ps4 symbols but I can't
    // figure out how to get a match otherwise.
    U32 mostSignificantChar = buffer[0];
    bool lowercase = false;
    if (mostSignificantChar >= 'a' && mostSignificantChar <= 'z')
    {
        lowercase = true;
    }
    if (lowercase)
    {
        mostSignificantChar -= 0x20;
    }

    return (mostSignificantChar - 'A') + 1;
}

U32 zVarEntryCB_VibrationOn(void* arg)
{
    // I'm pretty sure they actually and'd with a bit mask here, I couldn't find
    // another way (such as casting to bool) to get the right instructions.
    return globals.option_vibration & 0x1;
}

U32 zVarEntryCB_MCAvailable(void* arg)
{
    if (xSGGameIsEmpty(NULL, 0) == 1)
    {
        if (xSGGameIsEmpty(NULL, 1) == 1)
        {
            return 0;
        }
        if (xSGGameIsEmpty(NULL, 1) == 0)
        {
            return 2;
        }
    }
    else if (xSGGameIsEmpty(NULL, 0) == 0)
    {
        if (xSGGameIsEmpty(NULL, 1) == 1)
        {
            return 1;
        }
        if (xSGGameIsEmpty(NULL, 1) == 0)
        {
            return 3;
        }
    }

    return 0;
}

U32 zVarEntryCB_SndFXVol(void* arg)
{
    F64 f0 = 10.0;
    F64 f1 = 0.5;
    F64 f2 = gSnd.categoryVolFader[0];

    f2 *= f1;
    f2 = f0 + f2;

    return f2;
}

U32 zVarEntryCB_SndMusicVol(void* arg)
{
    F64 f0 = 10.0;
    F64 f1 = 0.5;
    F64 f2 = gSnd.categoryVolFader[2];

    f2 *= f1;
    f2 = f0 + f2;

    return f2;
}

U32 zVarEntryCB_SndMode(void* arg)
{
    return gSnd.stereo;
}

void zVarNewGame()
{
}

//                                  zSurface

extern volatile S32 sMapperCount;
extern zMaterialMapAsset* sMapper[1];

extern xSurface sDef_surf;

extern F32 lbl_803CDEE8; // 0.34906587 // @798
extern F32 lbl_803CDEE0; // 3.1415927 // pi // @796
extern F32 lbl_803CDEF0; // 176.0 // @801
extern F32 lbl_803CDEE4; // 180.0 // @797

extern F32 lbl_803CDEDC; // -1.0 // @702

extern F32 lbl_803CDED8; // 1.0 // @701

extern const char* zSurface_strings[];

xSurface& zSurfaceGetDefault()
{
    return sDef_surf;
}

void zSurfaceGetName(S32 type, char* buffer)
{
    *buffer = NULL;
    switch (type)
    {
    case 0:
        strcpy(buffer, "DEFAULT");
        break;
    case 1:
        strcpy(buffer, "TILE");
        break;
    case 2:
        strcpy(buffer, "CARPET");
        break;
    case 3:
        strcpy(buffer, "SHORTGRASS");
        break;
    case 4:
        strcpy(buffer, "LONGGRASS");
        break;
    case 5:
        strcpy(buffer, "GRAVEL");
        break;
    case 6:
        strcpy(buffer, "DIRT");
        break;
    case 7:
        strcpy(buffer, "MUD");
        break;
    case 8:
        strcpy(buffer, "THORNS");
        break;
    case 9:
        strcpy(buffer, "METAL");
        break;
    case 10:
        strcpy(buffer, "SHEETMETAL");
        break;
    case 11:
        strcpy(buffer, "CONCRETE");
        break;
    case 12:
        strcpy(buffer, "MARBLE");
        break;
    case 13:
        strcpy(buffer, "STONE");
        break;
    case 14:
        strcpy(buffer, "WOOD");
        break;
    case 15:
        strcpy(buffer, "SNOW");
        break;
    case 16:
        strcpy(buffer, "ICE");
        break;
    case 17:
        strcpy(buffer, "SHALLOWWATER");
        break;
    case 18:
        strcpy(buffer, "DEEPWATER");
        break;
    case 19:
        strcpy(buffer, "TAR");
        break;
    case 20:
        strcpy(buffer, "SPECIAL1");
        break;
    case 21:
        strcpy(buffer, "DeepAcid");
        break;
    case 22:
        strcpy(buffer, "NONE");
    }
}

F32 zSurfaceGetDamping(const xSurface* surf, F32 min_vel)
{
    return xpow(min_vel, surf->friction);
}

S32 zSurfaceGetSlickness(const xSurface* surf)
{
    return (int)(lbl_803CDED8 / surf->friction);
}

F32 zSurfaceGetOutOfBoundsDelay(const xSurface& s)
{
    if (s.moprops)
    {
        return ((zSurfaceProps*)s.moprops)->asset->oob_delay;
    }
    return lbl_803CDEDC;
}

F32 zSurfaceGetFriction(const xSurface* surf)
{
    return surf->friction;
}

U32 zSurfaceGetStandOn(const xSurface* surf)
{
    if (surf->moprops)
    {
        return !(((zSurfaceProps*)surf->moprops)->asset->phys_flags & 8);
    }
    return 1;
}

U32 zSurfaceGetSticky(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->game_sticky;
    }
    return 0;
}

U32 zSurfaceGetDamagePassthrough(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->game_damage_flags & 1;
    }
    return 0;
}

S32 zSurfaceGetDamageType(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->game_damage_type;
    }
    return 0;
}

U32 zSurfaceGetMatchOrient(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->phys_flags & 2;
    }
    return 0;
}

F32 zSurfaceGetSlideStopAngle(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((((zSurfaceProps*)surf->moprops)->asset->sld_start) * PI) / 176.0f;
    }
    return PI / 9;
}

F32 zSurfaceGetSlideStartAngle(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((((zSurfaceProps*)surf->moprops)->asset->sld_start) * PI) / 176.0f;
    }
    return PI / 9;
}

U8 zSurfaceOutOfBounds(const xSurface& s)
{
    if (s.moprops)
    {
        return ((zSurfaceProps*)s.moprops)->asset->phys_flags >> 4 & 1;
    }
    return 0;
}

U32 zSurfaceGetStep(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->phys_flags & 4;
    }
    return 0;
}

U32 zSurfaceGetSlide(const xSurface* surf)
{
    if (surf->moprops)
    {
        return ((zSurfaceProps*)surf->moprops)->asset->phys_flags & 1;
    }
    return 0;
}

xSurface* zSurfaceGetSurface(const xCollis* coll)
{
    xSurface* surf = NULL;

    if (coll->flags & 1)
    {
        if (coll->optr)
        {
            surf = coll->mptr->Surf;
        }
        else
        {
            surf = zSurfaceGetSurface(coll->oid);
        }
    }
    if (surf != NULL)
    {
        return surf;
    }
    surf = &sDef_surf;
    return surf;
}

// TODO: Hacked to OK (with volatile sMapperCount and assignment in if), fix later
void zSurfaceRegisterMapper(U32 assetId)
{
    if (sMapperCount >= 1)
    {
        return;
    }
    if (!assetId)
    {
        return;
    }
    if (sMapper[sMapperCount] = (zMaterialMapAsset*)xSTFindAsset(assetId, 0))
    {
        sMapperCount++;
    }
}

//                                            zScene

U8 HACK_BASETYPE;
static S32 bytesNeeded;
static S32 availOnDisk;
static S32 neededFiles;
static F32 offsetx;
static F32 offsety;
static U32 enableScreenAdj;
volatile static F32 oldOffsetx;
volatile static F32 oldOffsety;
static S32 sMemDepthSceneStart = -1;
static S32 sMemDepthJustHIPStart = -1;
_zEnv* gCurEnv;
U32 gTransitionSceneID;
F32 gSceneUpdateTime;
static xVec3 sOldPosPlayer;
static xVec3 sOldPosCamera;
static U32 sSuddenMove;

struct zSceneLevel
{
    const char* desc;
    const char* prefix;
};

struct IDBasePair
{
    U32 id;
    xBase* base;
};

static IDBasePair* scobj_idbps;
static S32 scobj_size = -1;
static S32 nidbps = -1;

extern U32 _1251;
extern char byte_803D0884;
extern U32 _2014;

extern U32 _1250;
extern F32 _1373;
extern F32 _1374;
extern F32 _1375;
extern F32 _1493;
extern F32 _1494;
extern F32 _1495;
extern F32 _1496_0;
extern U32 _2013;
extern F32 _2094;
extern F32 _2095_0;
extern F32 _2096_0;
extern F32 _2097_0;
extern F32 _2242;

static void zSceneObjHashtableInit(S32 count);
static void zSceneObjHashtableExit();
static S32 zSceneObjHashtableUsage();
static void zSceneObjHashtableAdd(U32 id, xBase* base);
static xBase* zSceneObjHashtableGet(U32 id);
static xBase* zSceneExitSoundIteratorCB(xBase* b, zScene*, void*);
static void zSceneAutoSave();

U32 zScene_ScreenAdjustMode()
{
    return enableScreenAdj;
}

void zSceneSetOldScreenAdj()
{
    oldOffsetx = offsetx;
    oldOffsety = offsety;
}

void zSceneEnableScreenAdj(U32 enable)
{
    enableScreenAdj = enable;
}
void zSceneCardCheckStartup_set(S32 needed, S32 available, S32 files)
{
    bytesNeeded = needed;
    availOnDisk = available;
    neededFiles = files;
}

void zScene_UpdateFlyToInterface(F32 dt)
{
}

U32 zSceneGetLevelIndex()
{
    return zSceneGetLevelIndex(globals.sceneCur->sceneID);
}

const char* zSceneGetLevelName(U32 sceneID)
{
    char c1 = (sceneID >> 24) & 0xFF;
    char c2 = (sceneID >> 16) & 0xFF;

    // Add in the updated LevelTable to this file

    // for (S32 i = 0; i < sizeof(sLevelTable) / sizeof(sLevelTable[0]); i++)
    // {
    //     if (c1 == sLevelTable[i].prefix[0] && c2 == sLevelTable[i].prefix[1])
    //     {
    //         return sLevelTable[i].desc;
    //     }
    // }

    return "Level Not Found";
}

void zSceneMemLvlChkCB()
{
}

const char* zSceneGetName(xBase* b)
{
    if (b)
    {
        const char* n = xSTAssetName(b->id);

        if (n)
        {
            return n;
        }
    }

    return "";
}

const char* zSceneGetName(U32 gameID)
{
    xBase* b = zSceneFindObject(gameID);

    if (b)
    {
        return zSceneGetName(b);
    }

    return "";
}

xBase* zSceneFindObject(U32 gameID)
{
    return 0;
}

static void zSceneObjHashtableAdd(U32 id, xBase* base)
{
    S32 k, chkd;

    chkd = id & (scobj_size - 1);

    for (k = 0; k < scobj_size; k++)
    {
        IDBasePair* idbp = &scobj_idbps[chkd];

        if (idbp->id == 0)
        {
            idbp->id = id;
            idbp->base = base;
            nidbps++;
            break;
        }

        chkd++;

        if (chkd == scobj_size)
        {
            chkd = 0;
        }
    }
}

void zSceneRenderPreFX()
{
    // PORT CODE FROM BATTLE
}

void zSceneRenderPostFX()
{
    // PORT CODE FROM BATTLE
}

void zSceneRender()
{
    zSceneRenderPreFX();
    xScrFxRender(globals.oldSkoolCamera.lo_cam);
    zSceneRenderPostFX();
}

void zSceneUpdate(F32 dt)
{
    // REFERENCE BFBB
}

S32 zSceneSetup_serialTraverseCB(U32 clientID, xSerial* xser)
{
    char uiName[16];
    S32 val = 0;
    xBase* b;

    xser->Read_b1(&val);

    if (val == 0)
    {
        return 1;
    }

    sprintf(uiName, "%s", xUtil_idtag2string(clientID, 0));

    uiName[1] = ' ';

    strcat(uiName, " VISITED");

    b = zSceneFindObject(xStrHash(uiName));

    if (b)
    {
        b->baseFlags |= 0x1;
    }

    return 1;
}

static void ActivateCB(xBase* base)
{
    base->baseFlags &= (U8)~0x40;
}

static void DeactivateCB(xBase* base)
{
    base->baseFlags |= 0x40;
}

void zSceneSetup()
{
    // REFERENCE BFBB
}

void zSceneReset()
{
    zScene* s = globals.sceneCur;

    zEntPlayerPreReset();

    gOccludeCount = 0;

    xScrFxLetterboxReset();
    zEntPickup_FlushGrabbed();
    zEntPickup_SceneReset();
    zMusicReset();
    //zThrown_Reset();
    zShrapnel_Reset();
    zFX_SceneReset();
    xFXSceneReset();
    xShadowManager_Reset();
    //ztalkbox::reset_all();
    //zCameraTweakGlobal_Reset();
    xPadDestroyRumbleChain(0);

    globals.cmgr = NULL;

    zEntEventAll(NULL, 0, eEventRoomEnd, NULL);
    zEntEventAll(NULL, 0, eEventSceneEnd, NULL);

    zSceneSave(s, NULL);
    zSceneAutoSave();

    xSndStopAll(~SND_CAT_UI);
    xSndUpdate();
    iSndWaitForDeadSounds();

    for (U32 i = 0; i < s->num_base; i++)
    {
        if (s->base[i])
        {
            switch (s->base[i]->baseType)
            {
            case eBaseTypePickup:
            case eBaseTypeCamera:
            case eBaseTypeDoor:
            case eBaseTypeSavePoint:
            case eBaseTypeItem:
            case eBaseTypeStatic:
            case eBaseTypeDynamic:
            case eBaseTypeMovePoint:
            case eBaseTypeTimer:
            case eBaseTypeBubble:
            case eBaseTypePortal:
            case eBaseTypeGroup:
            case eBaseTypeSFX:
            case eBaseTypeFFX:
            case eBaseTypeVFX:
            case eBaseTypeButton:
            case eBaseTypeProjectile:
            case eBaseTypeSurface:
            case eBaseTypeDestructObj:
            case eBaseTypeGust:
            case eBaseTypeVolume:
            case eBaseTypeDispatcher:
            case eBaseTypeCond:
            case eBaseTypeUI:
            case eBaseTypeUIFont:
            case eBaseTypeProjectileType:
            case eBaseTypeFog:
            case eBaseTypeLight:
            case eBaseTypeParticleEmitter:
            case eBaseTypeParticleSystem:
            case eBaseTypeCutsceneMgr:
            default:
            {
                zEntEvent(NULL, 0, s->base[i], eEventReset, NULL, NULL, 1);
                break;
            }
            case eBaseTypeCounter:
            {
                //xCounterReset(s->base[i]);
                break;
            }
            case eBaseTypePlayer:
            {
                zEntPlayerReset((xEnt*)s->base[i]);
                break;
            }
            case eBaseTypePlatform:
            {
                //zPlatform_Reset((zPlatform*)s->base[i], globals.sceneCur);
                break;
            }
            case eBaseTypePendulum:
            {
                //zPendulum_Reset((_zPendulum*)s->base[i], globals.sceneCur);
                break;
            }
            case eBaseTypeHangable:
            {
                //zEntHangable_Reset((zEntHangable*)s->base[i]);
                break;
            }
            case eBaseTypeLobMaster:
            {
                zEntEvent(s->base[i], eEventReset);
                break;
            }
            case eBaseTypeEGenerator:
            {
                //zEGenerator_Reset((zEGenerator*)s->base[i], globals.sceneCur);
                break;
            }
            case eBaseTypeEnv:
            {
                _zEnv* env = (_zEnv*)s->base[i];

                if (env)
                {
                    if (zSceneFindObject(env->easset->startCameraAssetID))
                    {
                        //zCameraReset(&globals.camera);
                    }

                    xClimateInitAsset(&gClimate, env->easset);
                }
            }
            }
        }
    }

    zNPCMgr_sceneReset();
    zEntPlayer_LoadCheckPoint();
    zSceneLoad(globals.sceneCur, NULL);
    zSceneEnableVisited(s);
    xSerialTraverse(zSceneSetup_serialTraverseCB);
    xSkyDome_Setup();
    xUpdateCull_Reset(globals.updateMgr);
    xUpdateCull_Update(globals.updateMgr, 100);
    zGridReset(s);

    zEntEventAll(NULL, 0, eEventSceneBegin, NULL);
    zEntEventAll(NULL, 0, eEventRoomBegin, NULL);
    zEntEventAll(NULL, 0, eEventSceneReset, NULL);

    //zNPCTiki_InitStacking(globals.sceneCur);
    //zNPCTiki_InitFX(globals.sceneCur);

    xUpdateCull_Update(globals.updateMgr, 100);
    zLOD_Update(100);

    //zGameExtras_SceneReset();
}

void zSceneLoad(zScene* ent)
{
    // REFERENCE BFBB
}

void zSceneSave(zScene* ent)
{
    // REFERENCE BFBB
}

void zSceneSwitch(_zPortal* p, S32 forceSameScene)
{
    // REFERENCE BFBB
}

void zSceneUpdateSFXWidgets()
{
    zScene* s = globals.sceneCur;
    xSFXUpdateEnvironmentalStreamSounds((xSFX*)s->baseList[eBaseTypeSFX],
                                        s->baseCount[eBaseTypeSFX]);
}

void zSceneExit(S32 beginReload)
{
    // REFERENCE BFBB
}

void zSceneInit(U32 theSceneID, S32 reloadInProgress)
{
    F32 pdone;
    zScene* s;
    U32 i;

    U8 rgba_bkgrd[4];
    *(U32*)rgba_bkgrd = _1250;

    gTransitionSceneID = theSceneID;
    gOccludeCount = 0;

    char b[5];
    *(U32*)b = _1251;
    b[4] = byte_803D0884;

    sprintf(b, xUtil_idtag2string(theSceneID, 0));
    xStrupr(b);

    theSceneID = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];

    xUtil_idtag2string(theSceneID, 0);

    xFogClearFog();
    xSndSceneInit();

    if (!reloadInProgress)
    {
        sMemDepthSceneStart = xMemPushBase();
    }

    zGameScreenTransitionBegin();
    zParPTankInit();

    pdone = _1373;

    if (globals.useHIPHOP && !reloadInProgress)
    {
        zGameScreenTransitionUpdate(pdone, "... scene preload ...\n");

        S32 ver_hop = xSTPreLoadScene(theSceneID, NULL, 0x2);

        if (ver_hop >= 0x000A000F)
        {
            xSTQueueSceneAssets(theSceneID, 0x2);

            do
            {
                rgba_bkgrd[0] = 0;
                rgba_bkgrd[1] = 0;
                rgba_bkgrd[2] = 0;
                rgba_bkgrd[3] = 0;

                pdone += _1373;

                zGameScreenTransitionUpdate(pdone, "... scene loading ...\n", rgba_bkgrd);
            } while (xSTLoadStep(theSceneID) < _1374);

            xSTDisconnect(theSceneID, 0x2);
        }
    }

    sMemDepthJustHIPStart = xMemPushBase();
    s = (zScene*)xMemAllocSize(sizeof(zScene));

    globals.sceneCur = s;

    xSceneInit(s, 200, 2048, 2068, 250);

    s->env = (xEnv*)xMemAllocSize(sizeof(xEnv));
    s->sceneID = theSceneID;

    iTime time;

    time = iTimeGet();
    xUtil_idtag2string(theSceneID, 0);
    iTimeDiffSec(time);

    xSTPreLoadScene(theSceneID, NULL, 0x1);

    time = iTimeGet();
    xUtil_idtag2string(theSceneID, 0);
    iTimeDiffSec(time);

    xSTQueueSceneAssets(theSceneID, 0x1);

    time = iTimeGet();
    xUtil_idtag2string(theSceneID, 0);
    iTimeDiffSec(time);

    pdone += _1373;

    zGameScreenTransitionUpdate(pdone, "... scene asset queue ...\n");

    time = iTimeGet();
    xUtil_idtag2string(theSceneID, 0);
    iTimeDiffSec(time);

    do
    {
        rgba_bkgrd[0] = 0;
        rgba_bkgrd[1] = 0;
        rgba_bkgrd[2] = 0;
        rgba_bkgrd[3] = 0;

        pdone += _1373;

        zGameScreenTransitionUpdate(pdone, "... scene loading ...\n", rgba_bkgrd);
    } while (xSTLoadStep(theSceneID) < _1374);

    xSTDisconnect(theSceneID, 0x1);

    time = iTimeGet();
    xUtil_idtag2string(theSceneID, 0);
    iTimeDiffSec(time);

    pdone += _1373;

    zGameScreenTransitionUpdate(pdone, "...initializing scene - sound\n");

    zEntPlayer_LoadSounds();
    iSndInitSceneLoaded();
    xPadRumbleEnable(globals.currentActivePad, globals.option_vibration);

    xSTSwitchScene(theSceneID, NULL, NULL);

    globals.sceneCur->resolvID = zSceneFindObject;
    globals.sceneCur->id2Name = zSceneGetName;
    globals.sceneCur->base2Name = zSceneGetName;

    g_xSceneCur = globals.sceneCur;

    xModelPipeNumTables = xSTAssetCountByType('PIPT');

    for (S32 i = 0; i < xModelPipeNumTables; i++)
    {
        void* data = xSTFindAssetByType('PIPT', i, NULL);

        xModelPipeCount[i] = *(S32*)data;
        xModelPipeData[i] = (xModelPipeInfo*)((S32*)data + 1);
    }

    xModelLookupCount = 0;

    //PipeForAllSceneModels(PipeCountStuffCB);

    if (xModelLookupCount)
    {
        xModelLookupList =
            (xModelPipeLookup*)xMemAllocSize(xModelLookupCount * sizeof(xModelPipeLookup));

        xModelLookupCount = 0;

        //PipeForAllSceneModels(PipeAddStuffCB);
    }

    xModelBucket_PreCountReset();
    //PipeForAllSceneModels(xModelBucket_PreCountBucket);

    xModelBucket_PreCountAlloc(256);
    //PipeForAllSceneModels(xModelBucket_InsertBucket);

    xModelBucket_Init();
    //add_scene_tweaks();
    xPTankPoolSceneEnter();
    zParPTankSceneEnter();
    xDecalInit();
    xFXScenePrepare();
    zLasso_scenePrepare();
    zDispatcher_scenePrepare();

    S32 total_npcs = xSTAssetCountByType('VIL ');
    zNPCMgr_scenePrepare(total_npcs);

    zAnimListInit();
    zGooInit(24);

    zGameScreenTransitionUpdate(_1375, "...initializing scene - base types\n");

    for (i = 0; i < eBaseTypeCount; i++)
    {
        s->baseCount[i] = 0;
        s->baseList[i] = NULL;
    }

    zCollGeom_Init();
    zUI_Init();
    zUIFontInit();
    ztextbox::init();
    //ztalkbox::init();
    ztaskbox::init();

    xModelInstStaticAlloc = 1;
    s->num_base = 0;

    // for (i = 0; sInitTable[i].name; i++)
    // {
    //     U32 typeCount = xSTAssetCountByType(sInitTable[i].assetType);

    //     s->baseCount[sInitTable[i].baseType] = typeCount;
    //     s->num_base += typeCount;

    //     if (sInitTable[i].querySubObjects)
    //     {
    //         for (U32 j = 0; j < typeCount; j++)
    //         {
    //             s->num_base += sInitTable[i].querySubObjects(
    //                 xSTFindAssetByType(sInitTable[i].assetType, j, NULL));
    //         }
    //     }
    // }

    //add_dynamic_types(*s);

    if (s->num_base)
    {
        s->base = (xBase**)xMemAllocSize(s->num_base * sizeof(xBase*));
    }
    else
    {
        s->base = NULL;
    }

    zSceneObjHashtableInit(4096);
    xFFXPoolInit(12);
    xFFXShakePoolInit(3);
    xFFXRotMatchPoolInit(1);
    xEntSceneInit();
    xEntMotionDebugInit(s->baseCount[eBaseTypePlatform] + s->baseCount[eBaseTypePendulum] +
                        s->baseCount[eBaseTypeButton]);
    zLightSetVolume(NULL);
    //xPartitionReset();
    xFXRibbonSceneEnter();

    U32 base_idx = 0;

    // for (i = 0; sInitTable[i].name; i++)
    // {
    //     HACK_BASETYPE = sInitTable[i].baseType;

    //     if (sInitTable[i].func)
    //     {
    //         base_idx = sInitTable[i].func(s, &sInitTable[i], base_idx);
    //     }
    // }

    //init_dynamic_types(*s, base_idx);

    s->num_update_base = 0;

    for (i = 0; i < s->num_base; i++)
    {
        // if (BaseTypeNeedsUpdate(s->base[i]->baseType))
        // {
        //     s->num_update_base++;
        // }
    }

    s->update_base = (xBase**)xMemAllocSize(s->num_update_base * sizeof(xBase*));

    base_idx = 0;

    for (i = 0; i < s->num_base; i++)
    {
        // if (BaseTypeNeedsUpdate(s->base[i]->baseType))
        // {
        //     s->update_base[base_idx] = s->base[i];
        // }
    }

    xModelInstStaticAlloc = 0;

    zGameScreenTransitionEnd();
    zSceneObjHashtableUsage();
    zUI_ScenePortalInit(s);
    zLightResolveLinks();
    zRenderStateInit();
    xFXStreakInit();
    xFXShineInit();
    xFXFireworksInit("PAREMIT_FIREWORKS_TRAIL", "PAREMIT_FIREWORKS1", "PAREMIT_FIREWORKS2",
                     "Fireworks_explode", "Fireworks_trail");
    //zFeetGetIDs();
    zLightningInit();
    zParCmdFindClipVolumes();
    zEntDestructObj_FindFX();
    zShrapnel_SceneInit(globals.sceneCur);
    zCameraTweakGlobal_Reset();
    zActionLineInit();
    xScrFxLetterboxReset();
    xShadowManager_Init(eBaseTypeNPC + 10);

    S32 lkitCount = xSTAssetCountByType('LKIT');
    void* lkitData;

    for (S32 i = 0; i < lkitCount; i++)
    {
        lkitData = xSTFindAssetByType('LKIT', i, NULL);

        xLightKit_Prepare(lkitData);
    }

    xClimateInit(&gClimate);
    zSceneInitEnvironmentalSoundEffect();

    sHackSmoothedUpdate = 1;

    //FootstepHackSceneEnter();
    zEntPickup_SceneEnter();
    xFXSceneInit();
    //zGame_HackGalleryInit();
    iSndSuspendCD(0);
}

static U32 BaseTypeNeedsUpdate(U8 baseType)
{
    if (baseType == eBaseTypeUnknown || baseType == eBaseTypePlayer || baseType == eBaseTypeEnv ||
        baseType == eBaseTypeCamera || baseType == eBaseTypeStatic ||
        baseType == eBaseTypeMovePoint || baseType == eBaseTypeBubble ||
        baseType == eBaseTypePortal || baseType == eBaseTypeGroup || baseType == eBaseTypeSFX ||
        baseType == eBaseTypeFFX || baseType == eBaseTypeVFX || baseType == eBaseTypeCounter ||
        baseType == eBaseTypeProjectile || baseType == eBaseTypeGust ||
        baseType == eBaseTypeVolume || baseType == eBaseTypeDispatcher ||
        baseType == eBaseTypeCond || baseType == eBaseTypeUI ||
        baseType == eBaseTypeProjectileType || baseType == eBaseTypeLobMaster ||
        baseType == eBaseTypeCutsceneMgr || baseType == eBaseTypeHud ||
        baseType == eBaseTypeNPCProps || baseType == eBaseTypeParticleEmitterProps ||
        baseType == eBaseTypeCruiseBubble || baseType == eBaseTypeTextBox ||
        baseType == eBaseTypeTalkBox || baseType == eBaseTypeTaskBox ||
        baseType == eBaseTypeBoulderGenerator || baseType == eBaseTypeNPCSettings ||
        baseType == eBaseTypeTurret)
    {
        return 0;
    }

    return 1;
}

static void PipeForAllSceneModels(void (*pipeCB)(RpAtomic* data, U32 pipeFlags, U32 subObjects))
{
    // non-matching: wrong registers

    S32 i, j, k;
    S32 numModels = xSTAssetCountByType('MODL');

    for (i = 0; i < numModels; i++)
    {
        RpAtomic* model = (RpAtomic*)xSTFindAssetByType('MODL', i, NULL);

        if (model)
        {
            PKRAssetTOCInfo ainfo;
            U32 numSubObjects, remainSubObjBits, currSubObjBits;
            RpAtomic* tempmodel;

            xSTGetAssetInfoByType('MODL', i, &ainfo);

            numSubObjects = 0;
            tempmodel = model;

            do
            {
                numSubObjects++;
                tempmodel = iModelFile_RWMultiAtomic(tempmodel);
            } while (tempmodel);

            remainSubObjBits = (1 << numSubObjects) - 1;

            for (j = 0; j < xModelPipeNumTables; j++)
            {
                for (k = 0; k < xModelPipeCount[j]; k++)
                {
                    if (ainfo.aid == xModelPipeData[j][k].ModelHashID)
                    {
                        currSubObjBits = remainSubObjBits & xModelPipeData[j][k].SubObjectBits;

                        if (currSubObjBits)
                        {
                            pipeCB(model, xModelPipeData[j][k].PipeFlags, currSubObjBits);
                        }

                        remainSubObjBits &= ~currSubObjBits;

                        if (!remainSubObjBits)
                        {
                            goto loc_800B1698;
                        }
                    }
                }
            }

            if (remainSubObjBits)
            {
                pipeCB(model, 0, remainSubObjBits);
            }
        }

    loc_800B1698:
        continue;
    }
}

static void PipeAddStuffCB(RpAtomic* data, U32 pipeFlags, U32)
{
    if (pipeFlags)
    {
        xModelLookupList[xModelLookupCount].model = data;
        xModelLookupList[xModelLookupCount].PipeFlags = pipeFlags;
        xModelLookupCount++;
    }
}

static void PipeCountStuffCB(RpAtomic*, U32 pipeFlags, U32)
{
    if (pipeFlags)
    {
        xModelLookupCount++;
    }
}

void zSceneSet(xBase* b, S32 index)
{
    globals.sceneCur->base[index] = b;
}

static U32 zSceneInitFunc_Dispatcher(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;

    count = s->baseCount[desc->baseType];

    if (count)
    {
        st_ZDISPATCH_DATA* dpat_pool = zDispatcher_memPool(count);
        s->baseList[desc->baseType] = dpat_pool;

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = zDispatcher_getInst(dpat_pool, idx);
            xEntAsset* asset = (xEntAsset*)xSTFindAssetByType('DPAT', idx, NULL);

            zSceneSet(b, base_idx);
            zDispatcher_Init((st_ZDISPATCH_DATA*)b, asset);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_LobMaster(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    return base_idx;
}

static U32 zSceneInitFunc_Volume(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;

    count = s->baseCount[desc->baseType];

    zVolumeInit();

    if (count)
    {
        s->baseList[desc->baseType] = zVolumeGetVolume(0);

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = zVolumeGetVolume(idx);

            zLightSetVolume((zVolume*)b);

            zSceneSet(b, base_idx);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_Surface(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;

    count = s->baseCount[desc->baseType];

    if (count)
    {
        s->baseList[desc->baseType] = xSurfaceGetByIdx(0);

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = xSurfaceGetByIdx(idx);

            zSceneSet(b, base_idx);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_Camera(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;
    zCamMarker* camBlock;

    count = s->baseCount[desc->baseType];

    if (count)
    {
        camBlock = (zCamMarker*)xMemAllocSize(count * sizeof(zCamMarker));
        s->baseList[desc->baseType] = camBlock;

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = &camBlock[idx];
            xCamAsset* assetCam = (xCamAsset*)xSTFindAssetByType('CAM ', idx, NULL);

            zSceneSet(b, base_idx);
            zCamMarkerInit(b, assetCam);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_Player(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    // REFERENCE BFBB
    return 0;
}

static U32 zSceneInitFunc_MovePoint(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;
    U32 assetSize;
    zMovePoint* movpBlock;

    assetSize = 0;
    count = s->baseCount[desc->baseType];
    movpBlock = zMovePoint_GetMemPool(count);

    if (movpBlock)
    {
        s->baseList[desc->baseType] = movpBlock;

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = zMovePoint_GetInst(idx);
            xBaseAsset* basset = (xBaseAsset*)xSTFindAssetByType('MVPT', idx, &assetSize);

            zSceneSet(b, base_idx);
            zMovePointInit(zMovePoint_GetInst(idx), (xMovePointAsset*)basset);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_Default(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    U8* block;
    S32 count;
    U32 assetSize, offset;
    xBase* b;

    block = NULL;
    assetSize = 0;
    offset = desc->sizeRuntime;
    count = s->baseCount[desc->baseType];

    if (count)
    {
        block = (U8*)xMemAllocSize(count * offset);

        s->baseList[desc->baseType] = (xBase*)block;
    }

    for (S32 i = 0; i < count; i++)
    {
        void* asset = xSTFindAssetByType(desc->assetType, i, &assetSize);
        b = (xBase*)(block + i * offset);

        zSceneSet(b, base_idx);

        if (desc->objectInitFunc)
        {
            desc->objectInitFunc(b, asset);
        }

        zSceneObjHashtableAdd(b->id, b);

        base_idx++;
    }

    return base_idx;
}

static U32 zSceneInitFunc_DefaultEnt(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    U8* block;
    S32 count;
    U32 assetSize, offset;
    xBase* b;

    block = NULL;
    assetSize = 0;
    offset = desc->sizeRuntime;
    count = s->baseCount[desc->baseType];

    if (count)
    {
        block = (U8*)xMemAllocSize(count * offset);

        s->baseList[desc->baseType] = (xBase*)block;
    }

    for (S32 i = 0; i < count; i++)
    {
        void* asset = xSTFindAssetByType(desc->assetType, i, &assetSize);
        b = (xBase*)(block + i * offset);

        zSceneSet(b, base_idx);

        if (desc->objectInitFunc)
        {
            desc->objectInitFunc(b, asset);
        }

        //xSceneAddEnt(s, s->ents[base_idx]);
        zSceneObjHashtableAdd(b->id, b);

        base_idx++;
    }

    return base_idx;
}

//                                      xSGT / zSaveLoad

U32 saveSuccess;
F32 time_last;
F32 time_current;
F32 time_elapsed = 0.01f;
iTime t0;
iTime t1;
S32 promptSel;
S32 badCard;
S32 sAvailable;
S32 sNeeded;
S32 sAccessType;
U8 preAutoSaving;

S32 currentCard = -1;
S32 currentGame = -1;
F32 dontPoll = 1.0f;
S32 autoSaveCard = -1;

char currSceneStr[32] = "TEMP";
char sceneRead[32] = "0000";
zSaveLoadUI zSaveLoadUITable[62] = { { 0, 0, "ld gameslot group" },
                                     { 1, 0, "ld memcards group" },
                                     { 2, 0, "ld format prompt group" },
                                     { 3, 0, "ld mc missing group" },
                                     { 4, 0, "mnu3 ld mc1" },
                                     { 5, 0, "mnu3 ld mc2" },
                                     { 6, 0, "ld gameslot 0" },
                                     { 7, 0, "ld gameslot 1" },
                                     { 8, 0, "ld gameslot 2" },
                                     { 9, 0, "ld gameslot 3" },
                                     { 10, 0, "ld gameslot 4" },
                                     { 11, 0, "ld gameslot 5" },
                                     { 12, 0, "ld gameslot 6" },
                                     { 13, 0, "ld gameslot 7" },
                                     { 14, 0, "ld format prompt" },
                                     { 15, 0, "ld format yes" },
                                     { 16, 0, "ld format no" },
                                     { 17, 0, "ld mc missing" },
                                     { 18, 0, "ld nogames" },
                                     { 19, 0, "ld nogames" },
                                     { 20, 0, "mnu3 start group" },
                                     { 21, 0, "sv gameslot group" },
                                     { 22, 0, "sv memcards group" },
                                     { 23, 0, "sv mc1" },
                                     { 24, 0, "sv mc2" },
                                     { 25, 0, "sv gameslot 0" },
                                     { 26, 0, "sv gameslot 1" },
                                     { 27, 0, "sv gameslot 2" },
                                     { 28, 0, "sv gameslot 3" },
                                     { 29, 0, "sv gameslot 4" },
                                     { 30, 0, "sv gameslot 5" },
                                     { 31, 0, "sv gameslot 6" },
                                     { 32, 0, "sv gameslot 7" },
                                     { 33, 0, "sv format prompt" },
                                     { 34, 0, "sv overwrite" },
                                     { 35, 0, "sv overwrite damaged" },
                                     { 36, 0, "sv mc missing" },
                                     { 37, 0, "sv nospace" },
                                     { 38, 0, "sv nospacegame" },
                                     { 39, 0, "ld mc dontremove" },
                                     { 40, 0, "sv mc dontremove" },
                                     { 41, 0, "mnu4 mc dontremove" },
                                     { 42, 0, "ld badload" },
                                     { 43, 0, "sv badsave" },
                                     { 44, 0, "mnu3 badformat" },
                                     { 45, 0, "mnu3 badformatnocard" },
                                     { 46, 0, "mnu4 badformat" },
                                     { 47, 0, "mnu4 badformatnocard" },
                                     { 48, 0, "mnu3 format confirm" },
                                     { 49, 0, "mnu4 format confirm" },
                                     { 50, 0, "sv format group" },
                                     { 51, 0, "mnu3 disk free" },
                                     { 52, 0, "mnu4 disk free" },
                                     { 53, 0, "sv card damaged" },
                                     { 54, 0, "sv badsavenocard" },
                                     { 55, 0, "ld damaged card" },
                                     { 56, 0, "sv damaged card" },
                                     { 57, 0, "ld wrong device" },
                                     { 58, 0, "sv wrong device" },
                                     { 59, 0, "ld damaged save" },
                                     { 60, 0, "ld damaged save game" },
                                     { 0, 0, NULL } };

char* thumbIconMap[15] = { "ThumbIconHB", "ThumbIconJF", "ThumbIconBB", "ThumbIconGL",
                           "ThumbIconB1", "ThumbIconRB", "ThumbIconBC", "ThumbIconSM",
                           "ThumbIconB2", "ThumbIconKF", "ThumbIconGY", "ThumbIconDB",
                           "ThumbIconB3", "ThumbIconHB", "ThumbIconHB" };

S32 xSGT_LoadPrefsCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, U32 ui,
                     S32 i)
{
    U32 stereo;

    xSGReadData(xsgdata, rctxt, &stereo, 1);
    xSGReadData(xsgdata, rctxt, &gSnd.categoryVolFader[2], 1);
    xSGReadData(xsgdata, rctxt, &gSnd.categoryVolFader[0], 1);
    xSGReadData(xsgdata, rctxt, &globals.option_vibration, 1);
    if (globals.option_vibration != 0)
    {
        xPadRumbleEnable(globals.currentActivePad, 1);
    }
    else
    {
        xPadRumbleEnable(globals.currentActivePad, 0);
    }
    return 1;
}

S32 xSGT_LoadLoadCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, U32 ui,
                    S32 i)
{
    char bigbuf[32] = {};
    S32 compdiff = 0;

    xSGReadData(xsgdata, rctxt, bigbuf, 1, strlen(currSceneStr));
    if (strlen(currSceneStr) != strlen(bigbuf))
    {
        compdiff = 1;
    }
    if (compdiff == 0)
    {
        strcpy(sceneRead, bigbuf);
    }

    return compdiff == 0;
}

S32 xSGT_SaveProcPrefsCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt)
{
    int sum = 0;
    sum += xSGWriteData(xsgdata, wctxt, &gSnd.stereo, 1);
    sum += xSGWriteData(xsgdata, wctxt, &gSnd.categoryVolFader[2], 1);
    sum += xSGWriteData(xsgdata, wctxt, &gSnd.categoryVolFader[0], 1);
    sum += xSGWriteData(xsgdata, wctxt, &globals.option_vibration, 1);
    return sum + 1;
}

S32 xSGT_SaveInfoPrefsCB(void* p1, st_XSAVEGAME_DATA* data, S32* i, S32* j)
{
    *i = 16;
    *j = *i * 2;
    return 1;
}

S32 xSGT_SaveProcCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt)
{
    if (globals.sceneCur->sceneID == 'PG12')
    {
        strcpy(currSceneStr, xUtil_idtag2string('HB01', 0));
    }
    else
    {
        strcpy(currSceneStr, xUtil_idtag2string(globals.sceneCur->sceneID, 0));
    }
    return xSGWriteData(xsgdata, wctxt, currSceneStr, 1, strlen(currSceneStr)) + 1;
}

S32 xSGT_SaveInfoCB(void* vp, st_XSAVEGAME_DATA* xsgdata, S32* need, S32* most)
{
    *need = xSGWriteStrLen(currSceneStr);
    *most = *need << 1;
    return 1;
}

void zSaveLoad_DispatchCB(U32 dispatchEvent, const F32* toParam)
{
    switch (dispatchEvent)
    {
    case 0xa8:
        promptSel = 4;
        break;
    case 0xaa:
        promptSel = 3;
        break;
    case 0xab:
        currentCard = (int)*toParam;
        en_SAVEGAME_MODE mode = XSG_MODE_LOAD;
        if (gGameMode == eGameMode_Save)
        {
            mode = XSG_MODE_SAVE;
        }
        st_XSAVEGAME_DATA* data = xSGInit(mode);
        zSaveLoad_CardCheckSpaceSingle_doCheck(data, currentCard);
        xSGDone(data);
        break;
    case 0xac:
        currentGame = (int)*toParam;
        break;
    case 0xad:
        promptSel = 3;
        break;
    }
}

// Scheduling meme on the return
U32 zSaveLoad_SaveLoop()
{
    zSaveLoadInit();
    S32 state = 0;
    saveSuccess = 0;

    while (state != 6)
    {
        switch (state)
        {
        case 0:
            while (zSaveLoad_CardCount() == 0)
            {
                zSaveLoad_CardPrompt(0);
            }
            state = 1;
            break;
        case 1:
            switch (zSaveLoad_CardPick(0))
            {
            case 1:
                state = 2;
                break;
            case 4:
                if (gGameState != 0)
                {
                    gGameMode = eGameMode_Pause;
                    state = 6;

                    xBase* sendTo = zSceneFindObject(xStrHash("PAUSE OPTIONS BKG GROUP"));
                    zEntEvent(sendTo, eEventUIFocusOn_Select);

                    sendTo = zSceneFindObject(xStrHash("PAUSE OPTIONS GROUP"));
                    zEntEvent(sendTo, eEventUIFocusOn);

                    sendTo = zSceneFindObject(xStrHash("PAUSE OPTION MGR UIF"));
                    zEntEvent(sendTo, eEventUIFocusOn_Select);

                    sendTo = zSceneFindObject(xStrHash("PAUSE OPTION SAVE UIF"));
                    zEntEvent(sendTo, eEventUIFocusOn_Select);
                }
                else
                {
                    gGameMode = eGameMode_Title;
                    state = 6;

                    xBase* sendTo = zSceneFindObject(xStrHash("MNU3 START CREATE NEW GAME NO"));
                    zEntEvent(sendTo, eEventUIFocusOn);

                    sendTo = zSceneFindObject(xStrHash("MNU3 START CREATE NEW GAME NO"));
                    zEntEvent(sendTo, eEventUIUnselect);

                    sendTo = zSceneFindObject(xStrHash("MNU3 START CREATE NEW GAME YES"));
                    zEntEvent(sendTo, eEventUIFocusOn_Select);

                    sendTo = zSceneFindObject(xStrHash("MNU3 START CREATE NEW GAME"));
                    zEntEvent(sendTo, eEventUIFocusOn);

                    sendTo = zSceneFindObject(xStrHash("BLUE ALPHA 1 UI"));
                    zEntEvent(sendTo, eEventInvisible);
                }
                break;
            case 11:
                state = 10;
                currentGame = 0;
                break;
            default:
                state = 0;
            }
            break;
        case 2:
            sAccessType = 2;
            switch (zSaveLoad_GameSelect(0))
            {
            case 2:
            case 4:
            case 5:
            case 6:
            case 7:
            case 9:
                state = 1;
                break;
            case 1:
                state = 10;
                break;
            case -1:
                zSaveLoad_ErrorPrompt(0);
                state = 1;
                break;
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            break;
        case 11:
            zGameModeSwitch(eGameMode_Game);
            zGameStateSwitch(1);
            state = 6;
            break;
        case 10:
            S32 tmp = zSaveLoad_SaveGame();
            if (tmp == 1)
            {
                zGameModeSwitch(eGameMode_Game);
                zGameStateSwitch(1);
                saveSuccess = 1;
                state = 6;

                xBase* sendTo = zSceneFindObject(xStrHash("MNU4 SAVE COMPLETED"));
                zEntEvent(sendTo, eEventVisible);
            }
            else
            {
                switch (tmp)
                {
                case 7:
                    zSaveLoad_SaveDamagedErrorPrompt(0);
                    break;
                case 8:
                    zSaveLoad_CardYankedErrorPrompt(0);
                    break;
                case 10:
                    zSaveLoad_CardPromptSpace(0);
                    break;
                default:
                    zSaveLoad_ErrorPrompt(0);
                    break;
                }
                state = 1;
            }
            break;
        }
    }

    sAccessType = 0;
    return saveSuccess;
}

U32 zSaveLoad_LoadLoop()
{
    zSaveLoadInit();
    sAccessType = 1;
    S32 state = 0;
    while (state != 6)
    {
        switch (state)
        {
        case 0:
            while (zSaveLoad_CardCount() == 0)
            {
                zSaveLoad_CardPrompt(1);
            }
            zSaveLoad_UIEvent(0x14, eEventUIFocusOff_Unselect);
            state = 1;
            break;
        case 1:
            switch (zSaveLoad_CardPick(1))
            {
            case 1:
                state = 2;
                break;
            case 4:
                state = 8;
                break;
            case -1:
                state = 0;
                break;
            case 2:
                break;
            }
            break;
        case 2:
            switch (zSaveLoad_GameSelect(1))
            {
            case 2:
            case 4:
            case 5:
            case 6:
            case 7:
            case 9:
                state = 1;
                break;
            case 1:
                state = 9;
                break;
            case -1:
                zSaveLoad_ErrorPrompt(1);
                state = 1;
                break;
            }
            break;
        case 3:
            break;
        case 8:
            zGameModeSwitch(eGameMode_Title);
            zGameStateSwitch(0);
            state = 6;
            break;
        case 9:
            switch (zSaveLoad_LoadGame())
            {
            case 1:
                zGameModeSwitch(eGameMode_Game);
                zGameStateSwitch(0);
                state = 6;
                globals.autoSaveFeature = 1;
                break;
            case 7:
                zSaveLoad_DamagedSaveGameErrorPrompt(1);
                state = 1;
                break;
            default:
                zSaveLoad_ErrorPrompt(1);
                state = 1;
            }
        }
    }

    zSendEventToThumbIcon(eEventInvisible);
    return (U32)sceneRead[0] << 0x18 | (U32)sceneRead[1] << 0x10 | (U32)sceneRead[2] << 0x8 |
           (U32)sceneRead[3];
}

// Reordering, causing different register use at the end
S32 zSaveLoad_LoadGame()
{
    S32 success = false;
    S32 teststat = true;
    S32 rc;
    en_XSGASYNC_STATUS asstat = XSG_ASTAT_NOOP;
    S32 use_tgt = CardtoTgt(currentCard);
    autoSaveCard = currentCard;

    st_XSAVEGAME_DATA* xsgdata = zSaveLoadSGInit(XSG_MODE_LOAD);
    xSGTgtSelect(xsgdata, use_tgt);
    xSGGameSet(xsgdata, currentGame);

    if (xsgdata == NULL)
    {
        teststat = false;
    }

    xSGAddLoadClient(xsgdata, 'ROOM', NULL, xSGT_LoadLoadCB);
    xSGAddLoadClient(xsgdata, 'PREF', NULL, xSGT_LoadPrefsCB);
    xSerial_svgame_register(xsgdata, XSG_MODE_LOAD);

    if (!xSGSetup(xsgdata))
    {
        teststat = false;
    }

    en_XSG_WHYFAIL whyFail = XSG_WHYERR_NONE;
    if (teststat)
    {
        rc = xSGProcess(xsgdata);
        if (rc)
        {
            asstat = xSGAsyncStatus(xsgdata, 1, &whyFail, NULL);
        }
        if (!rc)
        {
            teststat = false;
        }
        else
        {
            switch (asstat)
            {
            case XSG_ASTAT_SUCCESS:
                success = true;
                break;
            case XSG_ASTAT_FAILED:
                success = false;
                break;
            case XSG_ASTAT_NOOP:
            case XSG_ASTAT_INPROG:
                break;
            }
        }
    }

    if (teststat)
    {
        rc = xSGWrapup(xsgdata);
        if (rc < 0)
        {
            teststat = false;
            whyFail = XSG_WHYERR_OTHER;
        }
        else if (rc == 0)
        {
            teststat = false;
        }
    }

    if (!zSaveLoadSGDone(xsgdata))
    {
        teststat = false;
    }

    XSGAutoData* asg = xSGAutoSave_GetCache();
    S32 use_game = currentCard;
    if (success && teststat)
    {
        S32 idx = xSGTgtPhysSlotIdx(xsgdata, use_tgt);
        asg->SetCache(use_tgt, use_game, idx);
        globals.autoSaveFeature = 1;
        return 1;
    }
    asg->Discard();

    switch (whyFail)
    {
    case XSG_WHYERR_OTHER:
        return 7;
    default:
        return -1;
    }
}

// Reordering at beginning
S32 zSaveLoad_SaveGame()
{
    S32 success = false;
    S32 teststat = true;
    en_XSGASYNC_STATUS asstat = XSG_ASTAT_NOOP;
    S32 use_tgt = CardtoTgt(currentCard);
    S32 use_game = currentGame;
    autoSaveCard = currentCard;

    st_XSAVEGAME_DATA* xsgdata = zSaveLoadSGInit(XSG_MODE_SAVE);
    if (xSGCheckMemoryCard(xsgdata, currentCard) == 0)
    {
        zSaveLoadSGDone(xsgdata);
        return 8;
    }

    xSGTgtSelect(xsgdata, use_tgt);
    xSGGameSet(xsgdata, use_game);
    if (xsgdata == NULL)
    {
        teststat = false;
    }

    zSceneSave(globals.sceneCur, NULL);

    xSGAddSaveClient(xsgdata, 'ROOM', NULL, xSGT_SaveInfoCB, xSGT_SaveProcCB);
    xSGAddSaveClient(xsgdata, 'PREF', NULL, xSGT_SaveInfoPrefsCB, xSGT_SaveProcPrefsCB);

    xSerial_svgame_register(xsgdata, XSG_MODE_SAVE);

    U32 progress = zSceneCalcProgress();
    char label[64];
    const char* area;

    if (globals.sceneCur->sceneID == 'PG12')
    {
        area = zSceneGetLevelName('HB01');
    }
    else
    {
        area = zSceneGetLevelName(globals.sceneCur->sceneID);
    }

    strncpy(label, area, sizeof(label));

    if (!xSGSetup(xsgdata, use_game, label, progress, 0, zSceneGetLevelIndex()))
    {
        teststat = false;
    }

    en_XSG_WHYFAIL whyFail = XSG_WHYERR_NONE;
    if (teststat)
    {
        S32 rc = xSGProcess(xsgdata);
        if (rc)
        {
            asstat = xSGAsyncStatus(xsgdata, 1, &whyFail, NULL);
        }

        xSGGameIsEmpty(xsgdata, use_game);
        xSGTgtHasGameDir(xsgdata, use_tgt);
        if (!rc)
        {
            teststat = false;
        }
        else
        {
            switch (asstat)
            {
            case XSG_ASTAT_SUCCESS:
                success = true;
                break;
            case XSG_ASTAT_FAILED:
                success = false;
                break;
            case XSG_ASTAT_NOOP:
            case XSG_ASTAT_INPROG:
                break;
            }
        }
    }

    if (teststat && !xSGWrapup(xsgdata))
    {
        teststat = false;
    }

    if (!xSGCheckMemoryCard(xsgdata, currentCard))
    {
        zSaveLoadSGDone(xsgdata);
        return 8;
    }

    if (!zSaveLoadSGDone(xsgdata))
    {
        teststat = false;
    }

    XSGAutoData* asg = xSGAutoSave_GetCache();
    if (success && teststat)
    {
        S32 idx = xSGTgtPhysSlotIdx(xsgdata, use_tgt);
        asg->SetCache(use_tgt, use_game, idx);
        globals.autoSaveFeature = 1;
        return 1;
    }
    asg->Discard();

    switch (whyFail)
    {
    case XSG_WHYERR_DAMAGE:
    case XSG_WHYERR_OTHER:
        return 7;
    case XSG_WHYERR_NOCARD:
    case XSG_WHYERR_CARDYANKED:
        return 8;
    default:
        return -1;
    }
}

S32 zSaveLoad_DoAutoSave()
{
    S32 success = 0;
    S32 teststat = 1;
    en_XSGASYNC_STATUS asstat = XSG_ASTAT_NOOP;
    st_XSAVEGAME_DATA* svinst;

    XSGAutoData* autodata = xSGAutoSave_GetCache();
    if (autodata == NULL)
    {
        return -1;
    }
    if (!autodata->IsValid())
    {
        return -1;
    }

    S32 use_tgt = CardtoTgt(autodata->LastTarget());
    S32 lastGame = autodata->LastGame();
    autodata->Discard();
    svinst = xSGInit(XSG_MODE_SAVE);
    xSGTgtSelect(svinst, use_tgt);
    xSGGameSet(svinst, lastGame);

    if (svinst == NULL)
    {
        teststat = 0;
    }

    zSceneSave(globals.sceneCur, 0);

    xSGAddSaveClient(svinst, 'ROOM', 0, xSGT_SaveInfoCB, xSGT_SaveProcCB);
    xSGAddSaveClient(svinst, 'PREF', 0, xSGT_SaveInfoPrefsCB, xSGT_SaveProcPrefsCB);

    xSerial_svgame_register(svinst, XSG_MODE_SAVE);
    U32 progress = zSceneCalcProgress();

    const char* area;
    if (globals.sceneCur->sceneID == 'PG12')
    {
        area = zSceneGetLevelName('HB01');
    }
    else
    {
        area = zSceneGetLevelName(globals.sceneCur->sceneID);
    }

    char label[64];
    strncpy(label, area, sizeof(label));
    if (!xSGSetup(svinst, lastGame, label, progress, 0, zSceneGetLevelIndex()))
    {
        teststat = 0;
    }

    if (teststat != 0)
    {
        S32 iprocess = xSGProcess(svinst);
        if (iprocess != 0)
        {
            asstat = xSGAsyncStatus(svinst, 1, 0, 0);
        }
        xSGGameIsEmpty(svinst, lastGame);
        xSGTgtHasGameDir(svinst, use_tgt);
        if (iprocess == 0)
        {
            teststat = false;
        }
        else
        {
            switch (asstat)
            {
            case XSG_ASTAT_INPROG:
                break;
            case XSG_ASTAT_SUCCESS:
                success = true;
                break;
            case XSG_ASTAT_FAILED:
                success = false;
                break;
            }
        }
    }

    if (teststat && xSGWrapup(svinst) == 0)
    {
        teststat = false;
    }

    if (xSGDone(svinst) == 0)
    {
        teststat = false;
    }

    if ((success) && (teststat))
    {
        if (autodata != NULL)
        {
            S32 idx = xSGTgtPhysSlotIdx(svinst, use_tgt);
            autodata->SetCache(use_tgt, lastGame, idx);
            globals.autoSaveFeature = 1;
        }
        return 1;
    }
    else
    {
        if (autodata != NULL)
        {
            autodata->Discard();
        }
        globals.autoSaveFeature = 0;
        return -1;
    }
}

void zSaveLoadAutoSaveUpdate()
{
    xBase* obj;
    s32 out1, out2;

    if (globals.autoSaveFeature == 0 || gGameMode == eGameMode_Pause)
    {
        return;
    }
    if (preAutoSaving == 0)
    {
        return;
    }

    S32 physicalSlot = xSGAutoSave_GetCache()->LastPhysicalSlot();
    if (physicalSlot >= 0)
    {
        autoSaveCard = physicalSlot;
        switch (CARDProbeEx(physicalSlot, &out1, &out2))
        {
        case 0:
        case -1:
            obj = zSceneFindObject(xStrHash("SAVING GAME ICON UI")); //"SAVING GAME ICON UI"
            if (obj != NULL)
            {
                zEntEvent(obj, eEventVisible);
            }
            break;
        default:
            obj = zSceneFindObject(xStrHash("SAVING GAME ICON UI")); //"SAVING GAME ICON UI"
            if (obj != NULL)
            {
                zEntEvent(obj, eEventInvisible);
            }

            obj = zSceneFindObject(xStrHash("MNU4 AUTO SAVE FAILED")); //"MNU4 AUTO SAVE FAILED"
            if (obj != NULL)
            {
                zEntEvent(obj, eEventVisible);
            }
            globals.autoSaveFeature = 0;
            zSaveLoadPreAutoSave(0);
            break;
        }
    }
}

void zSaveLoadPreAutoSave(bool onOff)
{
    preAutoSaving = onOff;
    if (onOff)
    {
        ResetButton::DisableReset();
    }
    else
    {
        ResetButton::EnableReset();
    }
}

S32 zSaveLoad_CardCheckFormatted(int, int);

S32 zSaveLoad_GameSelect(S32 mode)
{
    S32 done = 0;
    S32 i;
    st_XSAVEGAME_DATA* svinst;
    S32 use_tgt;

    badCard = 1;
    while (badCard != 0)
    {
        currentGame = -1;
        promptSel = -1;
        use_tgt = CardtoTgt(currentCard);

        if (mode == 1)
        {
            zSaveLoad_UIEvent(0, eEventEnable);
            zSaveLoad_UIEvent(0, eEventUIFocusOn);
            zSaveLoad_UIEvent(0x33, eEventUIFocusOn);
        }
        else
        {
            zSaveLoad_UIEvent(0x34, eEventUIFocusOn);
        }

        for (i = 0; i < 3; i++)
        {
            svinst = xSGInit(XSG_MODE_LOAD);
            xSGTgtSelect(svinst, use_tgt);
            if (xSGGameIsEmpty(svinst, i))
            {
                if (mode == 1)
                {
                    zSaveLoad_UIEvent(i + 6, eEventDisable);
                }
                strcpy(zSaveLoadGameTable[i].label, "Empty");
                strcpy(zSaveLoadGameTable[i].date, "Empty");
                zSaveLoadGameTable[i].progress = 0;
                zSaveLoadGameTable[i].size = 0;
                zSaveLoadGameTable[i].thumbIconIndex = -1;
            }
            else
            {
                strcpy(zSaveLoadGameTable[i].label, xSGGameLabel(svinst, i));
                if (strcmpi(zSaveLoadGameTable[i].label, zSceneGetLevelName('MNU3')) == 0)
                {
                    strcpy(zSaveLoadGameTable[i].label, zSceneGetLevelName('HB01'));
                }
                strcpy(zSaveLoadGameTable[i].date, xSGGameModDate(svinst, i));
                zSaveLoadGameTable[i].progress = xSGGameProgress(svinst, i);
                zSaveLoadGameTable[i].size = xSGGameSize(svinst, i);
                zSaveLoadGameTable[i].size = zSaveLoadGameTable[i].size + 0x1fff & 0xffffe000;
                zSaveLoadGameTable[i].size >>= 0xd;
                zSaveLoadGameTable[i].size += 3;

                zSendEventToThumbIcon(3);
                zSaveLoadGameTable[i].thumbIconIndex = xSGGameThumbIndex(svinst, i);
                if (strcmpi(zSaveLoadGameTable[i].label, "") == 0)
                {
                    zSaveLoadGameTable[i].thumbIconIndex = -1;
                }
            }
            xSGDone(svinst);
        }

        if (mode == 1)
        {
            for (S32 emptyCount = 6; emptyCount <= 8; emptyCount++)
            {
                if (zSaveLoad_slotIsEmpty(emptyCount - 6) == 0)
                {
                    zSaveLoad_UIEvent(emptyCount, eEventUISelect);
                    break;
                }
            }
        }
        else
        {
            zSaveLoad_UIEvent(0x15, eEventUIFocusOn);
            zSaveLoad_UIEvent(0x19, eEventUISelect);
        }
        badCard = 0;

        while (!done && promptSel == -1)
        {
            zSaveLoad_Tick();
            zSaveLoad_poll(3);
            if (promptSel == 4)
            {
                done = promptSel;
            }

            if (currentGame == -1)
            {
                continue;
            }

            done = zSaveLoad_CardCheck(currentCard, mode);
            if (done != 1)
            {
                currentGame = -1;
                badCard = 0;

                if (done == 9)
                {
                    zSaveLoad_CardWrongDeviceErrorPrompt(mode);
                    done = 9;
                }
                break;
            }

            done = zSaveLoad_CardCheckFormatted(currentCard, mode);
            if (done != 1)
            {
                switch (done)
                {
                case 5:
                    done = 0;
                    promptSel = -1;
                    continue;
                case 7:
                    zSaveLoad_CardDamagedErrorPrompt(mode);
                    promptSel = -1;
                    done = 7;
                    badCard = 0;
                    continue;
                case 2:
                    currentGame = -1;
                    badCard = 0;
                    continue;
                default:
                    continue;
                }
            }

            done = zSaveLoad_CardCheckValid(currentCard, mode);
            if (done != 1)
            {
                switch (done)
                {
                case 5:
                case 6:
                    done = 0;
                    promptSel = -1;
                    continue;
                case 2:
                    promptSel = -1;
                    currentGame = -1;
                    badCard = 0;
                    continue;
                default:
                    break;
                }
            }

            if (badCard == 0)
            {
                done = zSaveLoad_CardCheckGameSlot(currentCard, currentGame, mode);
                if (done != 1)
                {
                    switch (done)
                    {
                    case -1:
                    case 5:
                        done = 0;
                        promptSel = -1;
                        break;
                    case 2:
                        currentGame = -1;
                        break;
                    }
                }
            }
            else
            {
                done = 0;
                currentGame = -1;
            }
        }
    }

    if (mode == 1)
    {
        zSaveLoad_UIEvent(0, eEventEnable);
    }

    S32 index = 0x15;
    if (mode == 1)
    {
        index = 0;
    }
    zSaveLoad_UIEvent(index, eEventUIFocusOff_Unselect);
    return done;
}

void zSaveLoad_BuildName(char* name_txt, S32 idx)
{
    char desired[128];
    char current_name[128];

    BuildIt(desired, idx);

    S32 counter = 0;
    for (S32 i = 0; i < idx; i++)
    {
        BuildIt(current_name, i);
        if (strcmp(zSaveLoadGameTable[i].label, zSaveLoadGameTable[idx].label) == 0 &&
            strcmp(current_name, "Corrupt Save File\n\n") != 0)
        {
            counter++;
        }
    }

    if (counter > 0)
    {
        sprintf(name_txt, "%s (%d)", desired, counter);
        name_txt[0x3f] = NULL;
    }
    else
    {
        strcpy(name_txt, desired);
    }
}

S32 zSaveLoad_CardPick(S32 mode)
{
    S32 done = 0;
    S32 formatDone = 0x16;

    currentCard = -1;
    promptSel = -1;

    if (mode == 1)
    {
        formatDone = 1;
    }

    zSaveLoad_UIEvent(formatDone, eEventUIFocusOn);
    formatDone = 0x17;
    if (mode == 1)
    {
        formatDone = 4;
    }

    zSaveLoad_UIEvent(formatDone, eEventUISelect);

    while (!done && promptSel == -1)
    {
        if (currentCard != -1)
        {
            done = zSaveLoad_CardCheck(currentCard, mode);
            switch (done)
            {
            case 2:
                currentCard = -1;
                promptSel = -1;
                continue;
            case -1:
                done = 0;
                promptSel = -1;
                continue;
            case 9:
                zSaveLoad_CardWrongDeviceErrorPrompt(mode);
                done = -1;
                promptSel = -1;
                continue;
            }

            done = zSaveLoad_CardCheckFormatted(currentCard, mode);
            switch (done)
            {
            case 5:
                done = 0;
                if (promptSel == 3)
                {
                    promptSel = -1;
                    continue;
                }
                if (promptSel == 4)
                {
                    currentCard = -1;
                    return -1;
                }
                break;
            case 6:
                done = 0;
                promptSel = -1;
                continue;
            case 7:
                zSaveLoad_CardDamagedErrorPrompt(mode);
                done = -1;
                promptSel = -1;
                continue;
            case 2:
            case 4:
                currentCard = -1;
                promptSel = -1;
                continue;
            case -1:
                done = 0;
                promptSel = -1;
                continue;
            case 8:
            case 9:
            case 10:
            case 11:

            default:
                break;
            }

            done = zSaveLoad_CardCheckValid(currentCard, mode);
            switch (done)
            {
            case 5:
            case 6:
                done = 0;
                promptSel = -1;
                continue;
            case 2:
            case 4:
                promptSel = -1;
                currentGame = -1;
                break;
            case 0:
            case 1:
                break;
            case -1:
                done = 0;
                promptSel = -1;
                continue;
            case 3:
                break;
            }
        }

        zSaveLoad_Tick();
    }

    formatDone = 0x16;
    if (mode == 1)
    {
        formatDone = 1;
    }

    zSaveLoad_UIEvent(formatDone, eEventUIFocusOff_Unselect);

    if (done == 1)
    {
        return done;
    }
    if (promptSel != -1)
    {
        return promptSel;
    }
    return done;
}

S32 zSaveLoad_CardCheckSlotOverwrite(S32 cardNumber, S32 gameNumber)
{
    // TODO: Figure out what this number means.
    S32 iVar1 = zSaveLoad_CardCheckSlotOverwrite_Free(cardNumber, gameNumber);
    // NOTE (Square): I'm not sure that this is supposed to be a loop. It doesn't make
    // sense to just break at the end and the condition feels like it should be an early return
    // but this matches and I don't know how else to generate the `b` instruction at 0x18
    while (iVar1 != 1 || iVar1 == 10)
    {
        if (iVar1 == -1 || iVar1 == 10)
        {
            return iVar1;
        }

        if (IsValidName(zSaveLoadGameTable[gameNumber].label))
        {
            iVar1 = zSaveLoad_CardPromptOverwrite();
        }
        else
        {
            iVar1 = zSaveLoad_CardPromptOverwriteDamaged();
        }

        if (iVar1 == 5)
        {
            return iVar1;
        }

        if (iVar1 == 2 || iVar1 == 4)
        {
            return 2;
        }
        break;
    }

    return 1;
}

S32 zSaveLoad_CardCheckSlotEmpty(S32 cardNumber, S32 gameNumber)
{
    S32 i = zSaveLoad_CardCheckSlotEmpty_hasGame(cardNumber, gameNumber);
    while (i != 1)
    {
        i = zSaveLoad_CardCheckSlotEmpty_hasGame(cardNumber, gameNumber);
        if (i == -1 || i == 6)
        {
            return i;
        }

        i = zSaveLoad_CardPromptGameSlotEmpty();
        if (i == 2 || i == 4)
        {
            return 2;
        }
    }
    return 1;
}

S32 zSaveLoad_CardCheckGameSlot(S32 cardNumber, S32 gameNumber, S32 mode)
{
    if (mode == 1)
    {
        return zSaveLoad_CardCheckSlotEmpty(cardNumber, gameNumber);
    }
    else
    {
        return zSaveLoad_CardCheckSlotOverwrite(cardNumber, gameNumber);
    }
}

S32 zSaveLoad_CardCheckGames(S32 cardNumber, S32 mode)
{
    S32 result = zSaveLoad_CardCheckGamesSingle(cardNumber);
    while (result != 1)
    {
        badCard = 1;

        result = zSaveLoad_CardCheckGamesSingle(cardNumber);
        if (result == -1 || result == 6 || result == 5)
        {
            return result;
        }

        result = zSaveLoad_CardPromptGames(mode);
        if (result == 2 || result == 4)
        {
            return 2;
        }
    }
    return 1;
}

S32 zSaveLoad_CardCheckSpace(S32 cardNumber, S32 mode)
{
    S32 result = zSaveLoad_CardCheckSpaceSingle(cardNumber);
    while (result != 1)
    {
        badCard = 1;

        result = zSaveLoad_CardCheckSpaceSingle(cardNumber);
        if (result == -1 || result == 6 || result == 5)
        {
            return result;
        }

        result = zSaveLoad_CardPromptSpace(mode);
        if (result == 2 || result == 4)
        {
            return 2;
        }
    }
    return 1;
}

S32 zSaveLoad_CardCheckValid(S32 cardNumber, S32 mode)
{
    if (mode == 1)
    {
        return zSaveLoad_CardCheckGames(cardNumber, mode);
    }
    else
    {
        return zSaveLoad_CardCheckSpace(cardNumber, mode);
    }
}

S32 zSaveLoad_CardCheckFormatted(S32 cardNumber, S32 mode)
{
    S32 result;
    while (result = zSaveLoad_CardCheckFormattedSingle(cardNumber), result != 1)
    {
        badCard = 1;

        result = zSaveLoad_CardCheckFormattedSingle(cardNumber);
        if (result == -1)
        {
            return -1;
        }

        if (result == 7)
        {
            return 7;
        }

        result = zSaveLoad_CardPromptFormat(mode);
        if (result == 2 || result == 4)
        {
            return 2;
        }

        sAccessType = 3;
        S32 tmp = format(cardNumber, mode);
        sAccessType = 2;
        return tmp != 1 ? tmp : 11;
    }
    return 1;
}

S32 zSaveLoad_CardCheck(S32 cardNumber, S32 mode)
{
    S32 cardResult = zSaveLoad_CardCheckSingle(cardNumber);
    while (cardResult == 0)
    {
        badCard = 1;

        cardResult = zSaveLoad_CardPrompt(mode);
        if (cardResult == 2 || cardResult == 4)
        {
            return 2;
        }
        cardResult = zSaveLoad_CardCheckSingle(cardNumber);
    }
    return cardResult == 9 ? 9 : 1;
}

S32 zSaveLoad_CardCheckFormattedSingle(S32 cardNumber)
{
    S32 rc;
    st_XSAVEGAME_DATA* ldinst = xSGInit(XSG_MODE_LOAD);
    S32 tgtmax;

    rc = 0;
    switch (xSGTgtCount(ldinst, &tgtmax))
    {
    case 2:
        switch (xSGTgtIsFormat(ldinst, cardNumber, NULL))
        {
        case -1:
            rc = 7;
            break;
        case 1:
            rc = 1;
            break;
        case 0:
        default:
            rc = 0;
            break;
        }
        break;
    case 1:
        S32 idx = xSGTgtPhysSlotIdx(ldinst, 0);
        if (idx != cardNumber)
        {
            rc = -1;
        }
        else if (!(idx ^ cardNumber))
        {
            switch (xSGTgtIsFormat(ldinst, 0, NULL))
            {
            case -1:
                rc = 7;
                break;
            case 1:
                rc = 1;
                break;
            case 0:
            default:
                rc = 0;
                break;
            }
        }
        break;
    case 0:
        rc = -1;
        break;
    }

    xSGDone(ldinst);
    return rc;
}

S32 zSaveLoad_CardPromptOverwriteDamaged()
{
    zSaveLoad_UIEvent(0x15, eEventUIFocusOff_Unselect);
    zSaveLoad_UIEvent(0x23, eEventUIFocusOn_Select);

    promptSel = -1;
    while (promptSel == -1)
    {
        zSaveLoad_Tick();
        zSaveLoad_poll(5);
    }
    zSaveLoad_UIEvent(0x23, eEventUIFocusOff_Unselect);
    return promptSel;
}

S32 zSaveLoad_CardPromptOverwrite()
{
    zSaveLoad_UIEvent(0x15, eEventUIFocusOff_Unselect);
    zSaveLoad_UIEvent(0x22, eEventUIFocusOn_Select);

    promptSel = -1;
    while (promptSel == -1)
    {
        zSaveLoad_Tick();
        zSaveLoad_poll(5);
    }
    zSaveLoad_UIEvent(0x22, eEventUIFocusOff_Unselect);
    return promptSel;
}

S32 zSaveLoad_CardPromptGameSlotEmpty()
{
    zSaveLoad_UIEvent(0, eEventUIFocusOff_Unselect);
    zSaveLoad_UIEvent(0x13, eEventUIFocusOn_Select);

    promptSel = -1;
    while (promptSel == -1)
    {
        zSaveLoad_Tick();
    }
    zSaveLoad_UIEvent(0x13, eEventUIFocusOff_Unselect);
    return promptSel;
}

S32 zSaveLoad_CardPromptGames(S32 mode)
{
    S32 i = 0x15;
    if (mode == 1)
    {
        i = 0;
    }
    zSaveLoad_UIEvent(i, eEventUIFocusOff_Unselect);
    zSaveLoad_UIEvent(1, eEventUIFocusOff_Unselect);
    zSaveLoad_UIEvent(0x12, eEventUIFocusOn_Select);

    promptSel = -1;
    while (promptSel == -1)
    {
        zSaveLoad_Tick();
        zSaveLoad_poll(3);
    }
    zSaveLoad_UIEvent(0x12, eEventUIFocusOff_Unselect);
    return promptSel;
}

S32 zSaveLoad_CardPromptSpace(S32 mode)
{
    S32 i = 0x15;
    if (mode == 1)
    {
        i = 0;
    }
    zSaveLoad_UIEvent(i, eEventUIFocusOff_Unselect);
    zSaveLoad_UIEvent(0x16, eEventUIFocusOff_Unselect);
    zSaveLoad_UIEvent(0x25, eEventUIFocusOn_Select);

    promptSel = -1;
    while (promptSel == -1)
    {
        zSaveLoad_Tick();
        zSaveLoad_poll(3);
    }
    zSaveLoad_UIEvent(0x25, eEventUIFocusOff_Unselect);
    return promptSel;
}

S32 zSaveLoad_CardPromptFormat(S32 mode)
{
    S32 i = 0x15;
    if (mode == 1)
    {
        i = 0;
    }
    zSaveLoad_UIEvent(i, eEventUIFocusOff_Unselect);

    i = 0x16;
    if (mode == 1)
    {
        i = 1;
    }
    zSaveLoad_UIEvent(i, eEventUIFocusOff_Unselect);
    zSaveLoad_UIEvent(0x32, eEventUIFocusOn);

    promptSel = -1;
    while (promptSel == -1)
    {
        zSaveLoad_Tick();
        zSaveLoad_poll(3);
    }
    zSaveLoad_UIEvent(0x32, eEventUIFocusOff_Unselect);
    return promptSel;
}

S32 zSaveLoad_CardPrompt(S32 cardNumber)
{
    S32 i = 0x15;
    if (cardNumber == 1)
    {
        i = 0;
    }
    zSaveLoad_UIEvent(i, eEventEnable);

    i = 0x15;
    if (cardNumber == 1)
    {
        i = 0;
    }
    zSaveLoad_UIEvent(i, eEventUIFocusOff_Unselect);

    i = 0x24;
    if (cardNumber == 1)
    {
        i = 0x11;
    }
    zSaveLoad_UIEvent(i, eEventUIFocusOn_Select);

    i = 0x16;
    if (cardNumber == 1)
    {
        i = 1;
    }
    zSaveLoad_UIEvent(i, eEventUIFocusOff_Unselect);

    promptSel = -1;
    while (promptSel == -1)
    {
        zSaveLoad_Tick();
    }

    i = 0x24;
    if (cardNumber == 1)
    {
        i = 0x11;
    }
    zSaveLoad_UIEvent(i, eEventUIFocusOff_Unselect);

    if (cardNumber == 1)
    {
        zSaveLoad_UIEvent(0, eEventDisable);
    }
    return promptSel;
}

S32 format(S32 num, S32 mode)
{
    st_XSAVEGAME_DATA* data = zSaveLoadSGInit(XSG_MODE_LOAD);
    S32 tgtmax;

    S32 rc = 0;
    switch (xSGTgtCount(data, &tgtmax))
    {
    case 2:
        xSGTgtSelect(data, num);
        rc = xSGTgtFormatTgt(data, num, 0);
        zSaveLoadSGDone(data);
        if (rc == -1)
        {
            zSaveLoad_ErrorFormatCardYankedPrompt(mode);
            rc = 5;
        }
        else
        {
            if (rc == 0)
            {
                zSaveLoad_ErrorFormatPrompt(mode);
            }
            if (rc != 0)
            {
                rc = 1;
            }
        }
        break;
    case 1:
        S32 idx = xSGTgtPhysSlotIdx(data, 0);
        if (idx != num)
        {
            zSaveLoadSGDone(data);
            rc = 5;
        }
        else
        {
            if (idx ^ num)
            {
                zSaveLoadSGDone(data);
            }
            else
            {
                xSGTgtSelect(data, 0);
                rc = xSGTgtFormatTgt(data, num, 0);
                zSaveLoadSGDone(data);
                if (rc == 0)
                {
                    zSaveLoad_ErrorFormatPrompt(mode);
                }
            }
        }
        break;
    case 0:
        rc = 5;
        zSaveLoadSGDone(data);
        break;
    }
    return rc;
}

void zSaveLoad_Tick(bool)
{
    // REFERENCE BFBB
}
