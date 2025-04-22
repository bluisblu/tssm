#ifndef ZWADNME_H
#define ZWADNME_H

#include <types.h>

#include "xNME.h"
#include "zBossCam.h"
#include "xPlayer.h"

struct base : xEnt, xFactoryInst
{
    S16 bound_bone;
    U16 sound_id_offset;
    U16 global_parameters_size;
    U16 local_parameters_size;
    U32 type;
    xModelAssetParam* global_parameters;
    xModelAssetParam* local_parameters;
    union
    {
        xMovePoint* movepoint;
        U32 movepoint_asset_id;
    };
    xEntNPCAssetIN* npc_asset;
    xModelAssetInfo* model_asset;
    F32 shadow_strength;
    F32 shadow_cache_fudge_factor;
    xVec3 bound_offset;
};

struct zNMENPCWrapper : base
{
    void (*fun_setup)(xEnt*);
    void (*fun_reset)(xEnt*);
    S32 colFreq;
    U8 flg_colCheck;
    U8 flg_penCheck;
    U16 flg_unusedCollFlags;
    struct
    {
        S32 aflg_basenme : 8;
        S32 flg_upward : 8;
        S32 flg_xtrarend : 1;
        S32 flg_inUpdate : 1;
        S32 flg_newtime : 1;
        S32 flg_postproc : 1;
        S32 flg_profProc : 1;
        S32 flg_hudrend : 1;
        S32 flg_moreUnusedSpace : 10;
    };

    void InitFromNPCMgr(xEntAsset* asset);
    void Init(xEntAsset* asset);
    void PostInit();
    void PostSetup();
    void Load();
    void NewTime();
    void RenderExtra();
    void RenderHud();
    void Move();
    S32 ColFreqReset();
    void init(xEntAsset* asset);
    void SelfSetup();
    void setup();
    void Reset();
    void Setup();
    void reset();
    void update_npc(F32 dt);
    void Process();
    void update_bound();
    void change_bounds();
    U8 system_event();
    void render_npc();
    void Render();
    void debug_render();
    void render_extra();
    void damage();
    void destroy();
    void Destroy();
    void scene_setup();
    void add_states();
    void add_transitions();
    S32 SysEvent();
    void BUpdate();
    void UpdateWrapper(F32 dt);
    void NMEWrap_Init(xEntAsset* asset);
    xAnimTable* CreateAnimTable();
    void SelfDestroy();
    void CollideReview();
};

struct zNMECommon : zNMENPCWrapper
{
    zNMEAsset* nmeass;
    S32 siz_nmeass;
    xPsyche* psy_self;
    NMECfgCommon* cfg_common;
    NMEShadParms* cfg_shadparm;
    S32 flg_vuln;
    S32 flg_move;
    S32 flg_misc;
    S32 flg_able;
    F32 spd_throttle;
    NMERuntime runtimeData;
    zNMEDriver* drv_data;
    zNMENavNet* navnet;
    union
    {
        F32 tmr_common[3];
        F32 tmr_invuln;
    };
    F32 tmr_scary;
    F32 tmr_lastAlert;
    zNMECommon* npc_duplodude;
    zNMESoundTable* snd_table;
    iSndHandle sndID[10];
    zShrapnelAsset* shrapnelAsset;

    void Destroy();
    void Render();
    void SelfDestroy();
    void ProcessInvis();
    void PostProcess();
    S32 IsAlive();
    S32 IsHealthy();
    S32 IsPresent();
    en_nmesimp SimpleStatus();
    F32 HealthRatio();
    void Boo();
    void DuploOwner(zNMECommon* duper);
    void SpeakBegin();
    void SpeakEnd();
    void SpeakStart();
    void SpeakStop();
    S32 CanRope();
    void CollideReview();
    zMovePoint* MvptFirstFollow();
    zMovePoint* MvptFirstTarget();
    xEntFrame* Gimme_xEntFrame();
    zNMEDriver* Gimme_DriveData();
    xEntShadow* Gimme_ShadowData();
    zNMENavNet* Gimme_NavNet();
    zNMEArena* Gimme_NMEArena();
    xShadowSimpleCache* Gimme_SimpShadowCache();
    void Reset();
    void PoofOfBubbles();
    S32 LaunchProjectile(en_haztyp haztyp, F32 spd_proj, F32 dst_minRange, xVec3* pos_launch,
                         F32 tym_predictMax, F32 hyt_offset, F32 rad_min, F32 rad_max);
    void GiveReward();
    void DeathFXKick();
    void DieTheGoodDeath();
    F32 ThrottleAdjust(F32 dt, F32 spd_want, F32 accel);
    xAnimTable* CreateAnimTable(xAnimTable* table);
    xAnimTransition* AnimMakeSmackTran(U32 animID);
    S32 AnimStart(U32 animID, S32 forceRestart);
    S32 MatMoveToPos(xVec3* pos_tobe, F32 dt, F32 spd, S32 doTurn, F32 spd_turnrate);
    F32 MatQRotTowards(F32 dt, xVec3* dir_face, F32 spd_turnrate);
    xModelInstance* ModelAtomicFind(S32 index, S32 idx_prev, xModelInstance* mdl_prev);
    S32 HaveLOSToPos(xVec3* pos, F32 dist, xScene* xscn, xBase* tgt, xCollis* colCallers);
    S32 ReviewCollBounce(xEntCollis* npccol, xVec3* vel, F32 fac_elastic);
    S32 ReviewCollSurfs(xEntCollis* npccol, zNMECommon* npc);
    void RunCollision(F32 dt);
    en_nmesimp Common_SimpStat();
    void ApplyGlowDamp();
    void ShadStufUpdate();
    void FullShadAcquire();
    void ShadowPrep();
    S32 WannaPoolShadow();
    void TellMeVisOnOff(xEnt* ent);
    S32 Respawn(xVec3* pos, zMovePoint* mvptFirst, zMovePoint* mvptSpawnRef);
    void Damage(en_npcdmg dmgtype, xBase* who, xVec3* vec_hit, S32 amt_dmgCaller,
                en_plyrpup pup_fromCaller);
    void ConvertHitEvent(NMESysEvent* sysmail);
    S32 DfltSysEventMail(NMESysEvent* sysmail);
    S32 SysEvent(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget,
                 U32 toParamWidgetID, S32* handled);
    S32 NMEMessage(NMEMsg* mail);
    S32 TypeHandleMail();
    void CreateConfig();
    void Setup();
    void Common_Init();
    void Process(F32 dt);
    void BUpdate(xVec3* pos);
    void Init(xEntAsset* entasset);
};

struct zNMEBoss : zNMECommon
{
};

struct zNMEDennis;

struct zNMESBBat : zNMECommon
{
    zNMEDennis* npc_bossman;
    void Reset();
    void Process(F32 dt);
    void Setup();
    void Init(xEntAsset* asset);
    void SelfSetup();
    U32 AnimPick(S32 gid);
    xAnimTable* CreateAnimTable(xAnimTable* table);
};

struct zNMEDennis : zNMEBoss
{
    S32 pts_health;
    NMECfgDennis* cfg_dennis;
    F32 tmr_nextLeap;
    S32 cnt_attacksTilEvade;
    zMovePoint* nav_evade;
    zNMESBBat* npc_sbbat;
    union
    {
        en_denstage stageCurr;
        S32 stageCurrNum;
    };
    union
    {
        xModelBlur moveBlur;
        xModelBlur moveBlurShort;
    };
    xModelBlur moveBlurLong;
    zStreamedSoundList oneLiners;
    DennisLetMeKnow tellMeWhen;
    S32 flg_firstTimestep;
    zBossCam_Binary cam_boss;
    en_dbcmode mod_camCurr;
    en_dbcmode mod_camWant;
    xGroup* grp_mvpt;

    zMovePoint* PickEvade(zMovePoint* nav_from);
    S32 DenMailSys(NMESysEvent* sysmail);
    S32 DenMailDamage(NMEDamageInfo* dmgmail);
    S32 TypeHandleMail(NMEMsg* mail);
    void BossCamSet(en_dbcmode mod_desired, U8 forceCut);
    en_vis VisionTarget();
    void RenderHud();
    void PostProcess();
    void NewTime(F32 dt);
    void BUpdate(xVec3* pos);
    void Process(F32 dt);
    S32 IsAlive();
    void Destroy();
    void Reset();
    void Setup();
    void Init(xEntAsset* asset);
    xAnimTable* CreateAnimTable(xAnimTable* table);
    U32 AnimPick(S32 gid, en_npcgspot gspot);
    zMovePoint* MvptFirstFollow();
    void SelfSetup();
    S32 DfltVulnFlags();
    void CreateConfig();
};

struct zNMEDenToo : zNMEBoss
{
    S32 pts_health;
    DenTooMeter denTooMeter;
    NMECfgDenToo* cfg_dentoo;
    F32 tmr_nextLeap;
    S32 cnt_attacksTilEvade;
    zMovePoint* nav_evade;
    union
    {
        en_dentstage stageCurr;
        S32 stageCurrNum;
    };
    xModelBlur blur_jumpShort;
    xModelBlur blur_jumpLong;
    zStreamedSoundList oneLiners;
    xGroup* grp_mvpt;
    S32 flg_firstTimestep;
    zBossCam_Binary cam_boss;
    en_dtbcmode mod_camCurr;
    en_dtbcmode mod_camWant;
    F32 tmr_bobCycle;

    zMovePoint* PickEvade(zMovePoint* nav_from);
    S32 DenMailSys(NMESysEvent* sysmail);
    S32 DenMailDamage(NMEDamageInfo* dmgmail);
    S32 TypeHandleMail(NMEMsg* mail);
    void BossCamRepel();
    void BossCamSet(en_dtbcmode mod_desired, U8 forceCut);
    void RespondToHittingPlayer();
    en_vis VisionTarget();
    void RenderHud();
    void BUpdate(xVec3* pos);
    void PostProcess();
    void Process(F32 dt);
    S32 IsAlive();
    void Destroy();
    void Reset();
    void Setup();
    void Init(xEntAsset* asset);
    xAnimTable* CreateAnimTable(xAnimTable* table);
    U32 AnimPick(S32 gid, en_npcgspot gspot);
    zMovePoint* MvptFirstFollow();
    void SelfSetup();
    S32 DfltVulnFlags();
    void CreateConfig();
    S32 WannaPoolShadow();
};

struct zNMEGoalDenDamage : zNMEGoalCommon
{
    S32 GoalHandleMail(NMEMsg* mail);
    S32 ChkExistence(zNMEDennis* npc, F32 dt, S32& nextgoal, en_trantype& trantype);
    S32 Process(en_trantype* trantype, F32 dt, void* ctxt);
    S32 Exit(float, void*);
    S32 Enter();
};

struct zNMEGoalDentDamage : zNMEGoalCommon
{
    S32 GoalHandleMail(NMEMsg* mail);
    S32 ChkExistence(zNMEDenToo* npc, F32 dt, S32& nextgoal, en_trantype& trantype);
    S32 Process(en_trantype* trantype, F32 dt, void* ctxt);
    S32 Exit(float, void*);
    S32 Enter();
};

#endif
