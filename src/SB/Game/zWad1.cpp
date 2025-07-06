#include "zWad1.h"
#include "xGlobals.h"
#include "zGlobals.h"

//                                                      zEnv

void zEnvEventCB(xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
}

void zEnvLoad(_zEnv* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

void zEnvSave(_zEnv* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void zEnvRender(xEnv* env)
{
    RpWorld* world = env->geom->world;
    S32 num = RpWorldGetNumMaterials(world);

    for (S32 i = 0; i < num; i++)
    {
        xSurface* sp = zSurfaceGetSurface(i);
        zSurfaceProps* pp = (zSurfaceProps*)sp->moprops;

        if (pp && pp->asset && pp->texanim_flags & 0x1)
        {
            RpMaterial* mp = RpWorldGetMaterial(world, i);

            if (mp)
            {
                xGroup* g = (xGroup*)pp->texanim[0].group_ptr;

                if (g)
                {
                    U32 texid = xGroupGetItem(g, pp->texanim[0].group_idx);
                    RwTexture* texptr = (RwTexture*)xSTFindAsset(texid, NULL);

                    if (texptr)
                    {
                        RpMaterialSetTexture(mp, texptr);
                    }
                }
            }
        }
    }

    xEnvRender(env, FALSE);
}

void zEnvStartingCamera(_zEnv*)
{
    return;
}

void zEnvSetup(_zEnv* env)
{
    // TODO
    // I don't think the struct is correct

    // This function seems to be correct in BFBB, our globals struct is fucked though.
    // Keep this the way it is, and fix the struct

    xBaseSetup(env);
    iEnvLightingBasics((iEnv*)globals.sceneCur->env->geom, (xEnvAsset*)env->easset);
}

// Again, Copied from Battle, Needs a good amount of work
void zEnvInit(_zEnv* env, xEnvAsset* easset)
{
    U32 bufsize;
    void* buf;

    xBaseInit(env, easset);

    env->easset = easset;
    //env->eventFunc = zEnvEventCB;

    if (env->linkCount)
    {
        env->link = (xLinkAsset*)(env->easset + 1);
    }

    globals.sceneCur->zen = env;
    globals.sceneCur->zen = env;

    xSTAssetCountByType('BSP ');

    buf = xSTFindAsset(easset->bspAssetID, &bufsize);

    if (buf)
    {
        xEnvLoadBsp(globals.sceneCur->env, buf, bufsize, 0);
    }

    zSurfaceRegisterMapper(easset->bspMapperID);

    if (easset->bspCollisionAssetID)
    {
        buf = xSTFindAsset(easset->bspCollisionAssetID, &bufsize);

        if (buf)
        {
            xEnvLoadBsp(globals.sceneCur->env, buf, bufsize, 1);
        }
    }

    if (easset->bspFXAssetID)
    {
        buf = xSTFindAsset(easset->bspFXAssetID, &bufsize);

        if (buf)
        {
            xEnvLoadBsp(globals.sceneCur->env, buf, bufsize, 2);
        }
    }

    if (easset->bspCameraAssetID)
    {
        buf = xSTFindAsset(easset->bspCameraAssetID, &bufsize);

        if (buf)
        {
            xEnvLoadBsp(globals.sceneCur->env, buf, bufsize, 3);
        }
    }

    if (globals.sceneCur->env && globals.sceneCur->env->geom)
    {
        zLightResetAll(globals.sceneCur->env);
        iLightInit(globals.sceneCur->env->geom->world);
    }
}

void zEnvInit(void* env, void* easset)
{
    zEnvInit((_zEnv*)env, (xEnvAsset*)easset);
}

//                                                      zEGenerator

void zEGenerator_TurnOn(zEGenerator* egen)
{
    zEGenAsset* zasset;
    xAnimPlay* aplay;
    xAnimSingle* asing;
    U16 itgt;
    U16 imp;
    S32 i;
    xLinkAsset* link;
    U32 dstAssetID;
    xBase* b;
    xVec3 destOffset;
    xEnt* ent;
    //S32 i;
    _tagLightningAdd add;
}

void zEGenerator_Render(zEGenerator* egen)
{
    xEntRender(egen);
}

void zEGenerator_Update(zEGenerator* egen, xScene* sc, F32 dt)
{
    xBase* b;
    zEnt* ent;
}

void zEGenerator_Move(zEGenerator* egen, xScene* xscn, F32 tmpFloat)
{
}

void zEGenerator_Reset(zEGenerator* egen, xScene* xscn)
{
    zEntReset((zEnt*)egen);
    zEGenAsset* zasset = (zEGenAsset*)egen->asset;
    egen->flags = (U16)zasset->flags;
    if (egen->flags & 1)
    {
        zEGenerator_TurnOn(egen);
    }
    else
    {
        zEGenerator_TurnOff(egen);
    }
    if (zasset->moreFlags & 0b100)
    {
        egen->chkby = XENT_COLLTYPE_NONE;
    }
    else
    {
        egen->chkby = XENT_COLLTYPE_PLYR | XENT_COLLTYPE_NPC;
    }
    egen->bupdate((xEnt*)egen, (xVec3*)&egen->model->Mat->pos);
}

void zEGenerator_Load(zEGenerator* ent, xSerial* s)
{
    zEntLoad(ent, s);
}

void zEGenerator_Save(zEGenerator* ent, xSerial* s)
{
    zEntSave(ent, s);
}

void zEGenerator_Setup(zEGenerator* egen, xScene* xscn)
{
    zEntSetup(egen);
    egen->num_dsts = 0;
    for (S32 i = 0; i < egen->linkCount; i++)
    {
        xLinkAsset* la = (xLinkAsset*)&egen->link[i];
        xBase* b = (xBase*)zSceneFindObject(la->dstAssetID);
        if (b != NULL && la->dstEvent == 0xc0)
        {
            if (b->baseType == eBaseTypeMovePoint)
            {
                egen->num_dsts++;
            }
            else if (b->baseFlags & 0x20)
            {
                egen->num_dsts++;
            }
        }
    }
}

void zEGenerator_Init(void* egen, void* asset)
{
}

//                                                      zDispatcher

extern st_ZDISPATCH_CONTEXT lbl_80254E00;
extern char lbl_80254E10[512];

extern S32 g_zdsp_init;
extern S32 warned_zDispatcher;
extern signed char init_zDispatcher;
extern S32 oldVibrationOption;
extern U32 oldSoundMode;
extern U32 oldMusicVolume;
extern U32 oldSFXVolume;

extern st_ZDISPATCH_DEPOT g_zdsp_depot;

extern iColor_tag _1143;

extern iColor_tag _1142;
extern F32 _1179;
extern F32 _1180;
extern F32 _1181;
extern F32 _1197;
extern F64 _1199;

extern U8 menu_fmv_played;
extern char zEventLogBuf[256][20];

S16 zDispatcher_GetCurrentWorld()
{
}

U32 zDispatcher_FindNextValidWorld(U32 sceneID)
{
    char sceneName[4];
    S32 worldIndex;
    S32 i;
    S32 j;
    S32 newIndex;
}

U32 zDispatcher_FindTaskPointer(U32 sceneID, S32 task)
{
    char sceneName[4];
    S32 i;
    S32 j;
}

void zDispatcher_FindWorldTask(U32 sceneID, S32& world, S32& task)
{
    char sceneName[4];
    S32 i;
    S32 j;
}

// WIP
// FIXME: This switch is a mess, good luck.

// PORTED DIRECTLY FROM BFBB
// NEEDS A LOT OF WORK
S32 ZDSP_elcb_event(xBase*, xBase* xb, U32 toEvent, const F32* toParam, xBase* toParamWidget)
{
    st_ZDISPATCH_DATA* dspdata = (st_ZDISPATCH_DATA*)xb;
    switch (toEvent)
    {
    case 10:
        ZDSP_instReset(dspdata, globals.sceneCur);
        break;
    case 0x129:
        zMusicNotifyEvent(toParam, toParamWidget);
        break;
    case 0x60:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_CFGSET, 0);
        break;
    case 0x61:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_CFGSET, 1);
        break;
    case 0x62:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_CFGSET, 2);
        break;
    case 0x63:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_CFGSET, 3);
        break;
    case 0x64:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_VIBESET, 1);
        break;
    case 0x65:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_VIBESET, 0);
        break;
    case 0x66:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SNDMOD_SET, 0);
        break;
    case 0x67:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SNDMOD_SET, 1);
        break;

    case 0x68:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SNDVOL_INCR);
        break;
    case 0x69:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SNDVOL_DECR);
        break;
    case 0x6a:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_MUSVOL_INCR);
        break;
    case 0x6b:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_MUSVOL_DECR);
        break;

    case 0x6c:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SFXVOL_INCR);
        break;
    case 0x6d:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SFXVOL_DECR);
        break;
    case 0xa9:
        zGameStateSwitchEvent(toEvent);
        break;
    case 0x9e:
        globals.currentActivePad = 0;
        zGameStateSwitchEvent(0x9e);
        break;
    case 0xa7:
        break;
    case 0xa8:
        zSaveLoad_DispatchCB(toEvent, toParam);
        break;

    case 0xc2:
        zEntPlayer_SNDPlay(ePlayerSnd_CheckPoint, _1179);
        F32 fVar2;
        if (*toParam != _1179)
        {
            fVar2 = _1180 * *toParam;
        }
        else
        {
            //fVar2 = globals.player.ent.frame->rot.angle;
        }
        zEntPlayer_StoreCheckPoint((xVec3*)toParamWidget, fVar2, globals.oldSkoolCamera.id);
        break;
    case 0x11e:
        zhud::show();
        break;
    case 0x11f:
        zhud::hide();
        break;
    case 0x120:
        iColor_tag black = _1142;
        iColor_tag clear = _1143;
        xScrFxFade(&clear, &black, *toParam, 0, 1);
        break;
    case 0x126:
        menu_fmv_played = 1;
        //zFMVPlay(zFMVFileGetName((eFMVFile)(U32)*toParam), 0x10001, _1181, 1, 0);
        break;
    case 0x130:
        zSceneEnableScreenAdj(1);
        zSceneSetOldScreenAdj();
        break;
    case 0x131:
        zSceneEnableScreenAdj(0);
        break;
    case 0x101:
        globals.player.g.PowerUp[(int)*toParam] = 1;
        break;

    case 0x1fd:
        xCameraFXShake(*toParam, toParam[1], toParam[2], toParam[3], _1179, NULL, NULL);
        break;
    case 0x210:
        xCMstart((xCreditsData*)toParamWidget, *toParam, xb);
        break;

    case 0x211:
        xCMstop();
        break;

    case 0x217:
        char events[512];
        U32 c;
        U32 len;
        U32 i;
        char log[512];

        for (i = 0; i < 512; i++)
        {
            events[i] = lbl_80254E10[i];
        }

        c = 0;
        for (i = 0; i < 8; i += len)
        {
            c += 1;
            len = strlen((char*)&ringlist[i]);
        }

        strcpy(lbl_80254E10, zEventLogBuf[c + 1]);
        for (i = c + 2; i < 0x13; i++)
        {
            strcat(log, zEventLogBuf[i]);
        }
        strncpy(events, log, 0x200);
        break;
    case 0x21b:
        zDispatcherStoreOptions();
        break;

    case 0x21c:
        zDispatcherRestoreOptions();
        break;
    }
    return 1;
}

S32 ZDSP_doCommand(st_ZDISPATCH_DATA* dspdata, st_ZDISPATCH_CONTEXT* cmdCtxt)
{
    S32 iv;
    en_DISPATCH_COMMAND cmd = cmdCtxt->cmd;
    void* indata = cmdCtxt->indata;
    void* result = cmdCtxt->result;

    if (init_zDispatcher == 0)
    {
        warned_zDispatcher = 0;
        init_zDispatcher = 1;
    }
    if (warned_zDispatcher == 0)
    {
        warned_zDispatcher = 1;
    }

    switch (cmd)
    {
    case ZDSP_CMD_CTRL_CFGGET:
        *(S32*)result = 0;
        break;
    case ZDSP_CMD_CTRL_CFGSET:
        if (result != NULL)
        {
            *(S32*)result = 0;
        }
        break;
    case ZDSP_CMD_CTRL_VIBEGET:
        *(S32*)result = zVarEntryCB_VibrationOn(NULL);
        break;
    case ZDSP_CMD_CTRL_VIBESET:
        if (globals.option_vibration != *(S32*)indata)
        {
            globals.option_vibration = *(S32*)indata;
            xPadRumbleEnable(globals.currentActivePad, globals.option_vibration);
            zRumbleStart(globals.currentActivePad, SDR_Test);
        }
        break;
    case ZDSP_CMD_SNDMOD_GET:
        *(S32*)result = zVarEntryCB_SndMode(NULL);
        break;
    case ZDSP_CMD_SNDMOD_SET:
        if (*(S32*)indata == 0)
        {
            iSndStereo(0);
        }
        else if (*(S32*)indata == 1)
        {
            iSndStereo(1);
        }

        if (result != NULL)
        {
            *(S32*)result = zVarEntryCB_SndMode(NULL);
        }
        break;
    case ZDSP_CMD_MUSVOL_GET:
        *(S32*)result = zVarEntryCB_SndMusicVol(NULL);
        break;
    case ZDSP_CMD_MUSVOL_SET:
        WRAP_xsnd_setMusicVolume(*(S32*)indata);
        if (result != NULL)
        {
            *(S32*)result = zVarEntryCB_SndMusicVol(NULL);
        }
        break;
    case ZDSP_CMD_MUSVOL_INCR:
        iv = zVarEntryCB_SndMusicVol(NULL);
        if (indata != NULL)
        {
            iv += *(S32*)indata;
        }
        else
        {
            iv += 1;
        }
        WRAP_xsnd_setMusicVolume(iv);
        if (result != NULL)
        {
            *(S32*)result = zVarEntryCB_SndMusicVol(NULL);
        }
        break;
    case ZDSP_CMD_MUSVOL_DECR:
        iv = zVarEntryCB_SndMusicVol(NULL);
        if (indata != NULL)
        {
            iv -= *(S32*)indata;
        }
        else
        {
            iv -= 1;
        }
        WRAP_xsnd_setMusicVolume(iv);
        if (result != NULL)
        {
            *(S32*)result = zVarEntryCB_SndMusicVol(NULL);
        }
        break;
    case ZDSP_CMD_SFXVOL_GET:
        *(S32*)result = zVarEntryCB_SndFXVol(NULL);
        break;
    case ZDSP_CMD_SFXVOL_SET:
        WRAP_xsnd_setSFXVolume(*(S32*)indata);
        if (result != NULL)
        {
            *(S32*)result = zVarEntryCB_SndFXVol(NULL);
        }
        break;
    case ZDSP_CMD_SFXVOL_INCR:
        iv = zVarEntryCB_SndFXVol(NULL);
        if (indata != NULL)
        {
            iv += *(S32*)indata;
        }
        else
        {
            iv += 1;
        }
        WRAP_xsnd_setSFXVolume(iv);
        if (result != NULL)
        {
            *(S32*)result = zVarEntryCB_SndFXVol(NULL);
        }
        break;
    case ZDSP_CMD_SFXVOL_DECR:
        iv = zVarEntryCB_SndFXVol(NULL);
        if (indata != NULL)
        {
            iv -= *(S32*)indata;
        }
        else
        {
            iv -= 1;
        }
        WRAP_xsnd_setSFXVolume(iv);
        if (result != NULL)
        {
            *(S32*)result = zVarEntryCB_SndFXVol(NULL);
        }
        break;
    case ZDSP_CMD_GSTATE_GET:
        *(S32*)result = zGameStateGet();
        break;
    case ZDSP_CMD_GSTATE_SET:
        zGameStateSwitch(*(S32*)indata);
        if (result != NULL)
        {
            *(S32*)result = zGameStateGet();
        }
        break;
    case ZDSP_CMD_GMODE_GET:
        *(S32*)result = zGameModeGet();
        break;
    case ZDSP_CMD_GMODE_SET:
        zGameModeSwitch(*(eGameMode*)indata);
        if (result != NULL)
        {
            *(S32*)result = zGameModeGet();
        }
        break;
    case ZDSP_CMD_SNDVOL_GET:
    case ZDSP_CMD_SNDVOL_SET:
    case ZDSP_CMD_SNDVOL_INCR:
    case ZDSP_CMD_SNDVOL_DECR:
    case ZDSP_CHECKPNT_SET:
        break;
    }
    return 1;
}

void zDispatcher_Load(st_ZDISPATCH_DATA* dspdata, xSerial* s)
{
    xBaseLoad(dspdata, s);
}

void zDispatcher_Save(st_ZDISPATCH_DATA* dspdata, xSerial* s)
{
    xBaseSave(dspdata, s);
}

void zDispatcher_InitDep(st_ZDISPATCH_DATA* dspdata, zScene* zscn)
{
}

void zDispatcher_Init(st_ZDISPATCH_DATA* dspdata, xBaseAsset* bass)
{
    xBaseInit(dspdata, bass);
    dspdata->rawass = bass;
    //dspdata->eventFunc = ZDSP_elcb_event;
    if (dspdata->linkCount != 0)
    {
        dspdata->link = (xLinkAsset*)(dspdata->rawass + 1);
    }
    else
    {
        dspdata->link = NULL;
    }
}

st_ZDISPATCH_DATA* zDispatcher_getInst(st_ZDISPATCH_DATA* pool, S32 idx)
{
    return &pool[idx];
}

st_ZDISPATCH_DATA* zDispatcher_memPool(S32 cnt)
{
    st_ZDISPATCH_DATA* pool;
    st_ZDISPATCH_DEPOT* depot = &g_zdsp_depot;
    if (cnt < 1)
    {
        return NULL;
    }
    else
    {
        pool = (st_ZDISPATCH_DATA*)xMemAlloc(gActiveHeap, cnt * sizeof(st_ZDISPATCH_DATA), 0);
        memset(pool, 0, cnt * sizeof(st_ZDISPATCH_DATA));
        depot->raw_pool = pool;
        depot->raw_cnt = cnt;
        return pool;
    }
}

void zDispatcher_sceneFinish()
{
    st_ZDISPATCH_DEPOT* depot = &g_zdsp_depot;

    g_zdsp_init--;
    if (g_zdsp_init == 0)
    {
        memset(depot, 0, sizeof(st_ZDISPATCH_DEPOT));
    }
}

void zDispatcher_scenePrepare()
{
    st_ZDISPATCH_DEPOT* depot = &g_zdsp_depot;
    if (g_zdsp_init++ == 0)
    {
        memset(depot, 0, sizeof(st_ZDISPATCH_DEPOT));
    }
}

void zDispatcher_Shutdown()
{
}

void zDispatcher_Startup()
{
}

void zDispatcherParseINI(xIniFile* ini)
{
    S32 i;
    char itemName[16];
    char value[256];
    char* value_;
    S32 j;
    char* ltok;
    char* tok;
}

//                                                      z_disco_floor

// REQUIRES A LOT OF STUFF TO BE PORTED
// TOO LAZY TO DO IT RN

//                                                      zDestructible

//                                                      zCutsceneMgr

static S32 gCutsceneSkipOK;
static S32 donpcfx;
static U8 ents_hidden;
static S32 s_atomicNumber;

void zCutsceneMgrUpdate(xBase* to, xScene* sc, F32 dt)
{
    zCutsceneMgr* t = (zCutsceneMgr*)to;
    if (t->csn->ShutDownWait)
    {
        if (donpcfx)
        {
            zNPCFXCutsceneDone(sc, dt, t);
        }
        donpcfx = 0;
        if (xCutscene_Destroy(t->csn))
        {
            zCutsceneMgrKillFX(t);
            t->csn = NULL;
            globals.cmgr = NULL;
            xCameraSetFOV(&globals.oldSkoolCamera, t->oldfov);
        }
    }
    else
    {
        if (t->csn->Ready)
        {
            if (!donpcfx)
            {
                donpcfx = zNPCFXCutscenePrep(sc, dt, t);
            }
            if (donpcfx)
            {
                zNPCFXCutscene(sc, dt, t);
            }

            zCutsceneMgrUpdateFX(t);
            if (t->stop || !xCutscene_Update(t->csn, dt))
            {
                if (!t->stop)
                {
                    zEntEvent(to, to, 0x13);
                }

                t->csn->PlaybackSpeed = 0.0f;
                //zEntEvent(&globals.player.ent, 9);
                //zEntEvent(&globals.player.ent, 3);
                zEntEvent(to, to, 0xbf);
                iCameraSetNearFarClip(0.0f, 0.0f);

                if ((globals.sceneCur->pendingPortal))
                {
                    //zEntEvent(&globals.player.ent, 8);
                    //zEntEvent(&globals.player.ent, 4);
                    xSndStopAll(0xffff);
                    return;
                }

                if (donpcfx)
                {
                    zNPCFXCutsceneDone(sc, dt, t);
                }
                donpcfx = 0;

                if (xCutscene_Destroy(t->csn))
                {
                    zCutsceneMgrKillFX(t);
                    t->csn = NULL;
                    globals.cmgr = NULL;
                    xCameraSetFOV(&globals.oldSkoolCamera, t->oldfov);
                }

                zMusicUnpause(0);
            }
            else
            {
                if (gCutsceneSkipOK && t->csn->Time > gSkipTimeCutscene &&
                    globals.pad0->pressed & 0x50000)
                {
                    zEntEvent(to, to, 0x13);
                }
            }
        }
        else
        {
            iCSLoadStep(t->csn);
            if (t->csn->Ready)
            {
                zCutsceneMgrPlayStart(t);
                zEntEvent(to, to, 0x18);
                //zEntEvent(&globals.player.ent, 8);
                //zEntEvent(&globals.player.ent, 4);
                donpcfx = zNPCFXCutscenePrep(sc, dt, t);
            }
        }
    }
    check_hide_entities();
}

void zCutsceneMgrFinishExit(xBase* to)
{
    zCutsceneMgr* t = (zCutsceneMgr*)to;

    while (1)
    {
        iFileAsyncService();
        xSndUpdate();
        if (t->csn->Waiting == 0)
            break;
        iVSync();
    }
    if (donpcfx != 0)
    {
        zNPCFXCutsceneDone(globals.sceneCur, 0.0f, t);
    }
    donpcfx = 0;
    if (xCutscene_Destroy(t->csn) == 0)
    {
        return;
    }

    //zEntEvent(&globals.player.ent, 9);
    //zEntEvent(&globals.player.ent, 3);
    zCutsceneMgrKillFX(t);
    t->csn = NULL;
    globals.cmgr = NULL;
}

void zCutsceneMgrFinishLoad(xBase* to)
{
    zCutsceneMgr* t = (zCutsceneMgr*)to;

    while (true)
    {
        iFileAsyncService();
        xSndUpdate();
        iCSLoadStep(t->csn);
        iTRCDisk::CheckDVDAndResetState();
        if (t->csn->Ready)
        {
            break;
        }
        iVSync();
    }
    zCutsceneMgrPlayStart(t);
    zEntEvent(to, to, 0x18);
    //zEntEvent(&globals.player.ent, 8);
    //zEntEvent(&globals.player.ent, 4);
    return;
}

void zCutsceneMgrLoad(zCutsceneMgr*, xSerial* s)
{
}

void zCutsceneMgrSave(zCutsceneMgr*, xSerial* s)
{
}

//                                                      zConfig

U8 zConfig_isPlayerPatrick(ePlayerEnum playerEnum)
{
    U8 isPatrick = 0;
    if ((playerEnum >= 10) && (playerEnum <= 0x15))
    {
        isPatrick = 1;
    }
    return isPatrick;
}

U8 zConfig_isPlayerSpongebob(ePlayerEnum playerEnum)
{
    U8 isSpongebob = 0;
    if ((playerEnum >= 0) && (playerEnum <= 10))
    {
        isSpongebob = 1;
    }
    return isSpongebob;
}

//                                                      zConditional

S32 zConditionalEventCB(xBase*, xBase*, U32, const F32*, xBase*);

void zConditionalLoad(_zConditional* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

void zConditionalSave(_zConditional* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void zConditionalInit(void* b, void* asset)
{
    _zConditional* cond = (_zConditional*)b;

    xBaseInit((xBase*)cond, (xBaseAsset*)asset);
    //cond->eventFunc = (xBaseEventCB)zConditionalEventCB;
    //cond->asset = asset;

    if (cond->linkCount != 0)
    {
        cond->link = (xLinkAsset*)(cond->asset + 1);
    }
    else
    {
        cond->link = NULL;
    }
    zVarInit(zVarEntryTable);
}

//                                                      zCollGeom

static volatile U32 sNumTables;
static U32 sTableCount[3];
static zCollGeomTable* sTableList[3];

void zCollGeom_CamDisable(xEnt* ent)
{
    ent->camcollModel = NULL;
}

void zCollGeom_CamEnable(xEnt* ent)
{
    if (!ent->camcollModel)
    {
        ent->camcollModel = ent->model;
    }
}

void zCollGeom_Init()
{
    sNumTables = xSTAssetCountByType('COLL');

    if (sNumTables)
    {
        U32 tmpsize, i, k;
        void* data;

        for (k = 0; k < sNumTables; k++)
        {
            data = xSTFindAssetByType('COLL', k, &tmpsize);

            if (!data)
            {
                break;
            }

            sTableCount[k] = *(U32*)data;

            if (!sTableCount[k])
            {
                sTableList[k] = NULL;
                break;
            }

            sTableList[k] = (zCollGeomTable*)((U32*)data + 1);

            for (i = 0; i < sTableCount[k]; i++)
            {
                if ((U32)sTableList[k][i].baseModel)
                {
                    sTableList[k][i].baseModel =
                        (RpAtomic*)xSTFindAsset((U32)sTableList[k][i].baseModel, NULL);
                }

                if ((U32)sTableList[k][i].colModel[0])
                {
                    sTableList[k][i].colModel[0] =
                        (RpAtomic*)xSTFindAsset((U32)sTableList[k][i].colModel[0], NULL);
                }

                if (!sTableList[k][i].colModel[0])
                {
                    if (sTableList[k][i].camcolModel)
                    {
                        sTableList[k][i].camcolModel =
                            (RpAtomic*)xSTFindAsset((U32)sTableList[k][i].camcolModel, NULL);
                    }
                }
            }
        }
    }
}

// PORTED FROM BATTLE, GOOD REFERENCE
U32 zCollGeom_EntSetup(xEnt* ent)
{
    U32 i, j;
    S32 auto_cam_coll;

    auto_cam_coll = (ent->baseType == eBaseTypeStatic);

    ent->collModel = NULL;
    ent->camcollModel = NULL;

    if (ent->model)
    {
        for (j = 0; j < sNumTables; j++)
        {
            for (i = 0; i < sTableCount[j]; i++)
            {
                if (sTableList[j][i].baseModel == ent->model->Data)
                {
                    RpAtomic* imodel;

                    if (imodel = sTableList[j][i].colModel[0])
                    {
                        xModelInstance* model = xModelInstanceAlloc(imodel, ent, 0x2000, 0, NULL);

                        ent->collModel = model;
                        ent->collModel->Mat = ent->model->Mat;

                        while (imodel = iModelFile_RWMultiAtomic(imodel))
                        {
                            xModelInstanceAttach(xModelInstanceAlloc(imodel, ent, 0x2000, 0, NULL),
                                                 model);
                        }
                    }
                    else if (imodel = sTableList[j][i].camcolModel)
                    {
                        xModelInstance* model = xModelInstanceAlloc(imodel, ent, 0x2000, 0, NULL);

                        ent->camcollModel = model;
                        ent->camcollModel->Mat = ent->model->Mat;

                        while (imodel = iModelFile_RWMultiAtomic(imodel))
                        {
                            xModelInstanceAttach(xModelInstanceAlloc(imodel, ent, 0x2000, 0, NULL),
                                                 model);
                        }
                    }
                    else
                    {
                        auto_cam_coll = 0;
                    }
                }
            }
        }
    }

    if (auto_cam_coll && !ent->camcollModel)
    {
        ent->camcollModel = ent->model;
    }

    if (ent->collModel)
    {
        return 1;
    }

    ent->collModel = ent->model;
    return 0;
}

//                                                      zCar

// Refrain from doing these functions until the zCar struct is fixed.

#define Car 1
#define Idle01 2
#define Drive 3
#define DriveSlippy 4
#define Boosting 5
#define Reversing 6
#define Crash 7
#define SpinLeft 8
#define SpinRight 9
#define Success 10
#define Failed 11
#define Death 12
#define Jump 13
#define JumpLandSquash 14

#define ANIM_COUNT 78
extern U32 g_hash_caranim[ANIM_COUNT];
const char* g_strz_caranim[ANIM_COUNT];

// Needs the functions calls updated
// NewState and DefaultBeforeEnter do not have correct amount of params yet

xAnimTable* CreateAnimTable()
{
    S32 ourAnims[32] = {
        // clang-format off
        Car,
        Idle01,      
        Drive,
        DriveSlippy,
        Boosting,
        Reversing,
        Crash,
        SpinLeft,
        SpinRight,
        Success,
        Failed,
        Death,
        Jump,
        JumpLandSquash,

        // clang-format on
    };

    xAnimTable* table = xAnimTableNew("stop0", 0);

    xAnimTableNewState(table, "Car", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL, NULL,
                       xAnimDefaultBeforeEnter, NULL, NULL);
}

U32 zCar::JumpEndedCB(xAnimTransition* unk0, xAnimSingle* anim, void* unk3)
{
    zCar* pCar = zEntPlayerCarGetCar();
}

U32 zCar::AnimDefaultCB(xAnimTransition* unk0, xAnimSingle* anim, void*)
{
    zCar* pCar = zEntPlayerCarGetCar();
}
