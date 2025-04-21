#ifndef XNME_H
#define XNME_H

#include "xEnt.h"
#include "xModel.h"
#include "xBehaviour.h"
#include "xFrag.h"
#include "xPar.h"
#include "xMail.h"
#include "xSound.h"

enum en_allow
{
    ALLOW_NEVER,
    ALLOW_NORMAL,
    ALLOW_ALWAYS,
    ALLOW_NOMORE
};

enum en_nmesimp
{
    NME_SIMP_HAPPY,
    NME_SIMP_DEAD,
    NME_SIMP_IGNORANT,
    NME_SIMP_WARYDETECT,
    NME_SIMP_WARY,
    NME_SIMP_WORRIED,
    NME_SIMP_CONFUSED,
    NME_SIMP_BIGBULLY,
    NPC_STAT_NOMORE
};

enum en_haztyp
{
    HAZ_TYP_UNKNOWN,
    HAZ_TYP_EXPLODE,
    HAZ_TYP_IMPACT,
    HAZ_TYP_IMPACTWET,
    HAZ_TYP_IMPACTBOOM,
    HAZ_TYP_DAZED,
    HAZ_TYP_SPAWNBALL_XX,
    HAZ_TYP_SPAWNBALL_GG,
    HAZ_TYP_SPAWNBALL_TR,
    HAZ_TYP_SPAWNBALL_PT,
    HAZ_TYP_SPAWNGROW,
    HAZ_TYP_BUCKOBOOM_V1,
    HAZ_TYP_TIKIBOOM,
    HAZ_TYP_SLAMWAVE_V1,
    HAZ_TYP_SLAMWAVE_V2,
    HAZ_TYP_SLAMWAVE_V3,
    HAZ_TYP_FLING_V1,
    HAZ_TYP_FLING_V2,
    HAZ_TYP_FLING_V3,
    HAZ_TYP_FLING_FROGFISH,
    HAZ_TYP_SPITPUDDLE_V1,
    HAZ_TYP_SPITPUDDLE_V2,
    HAZ_TYP_SPITPUDDLE_V3,
    HAZ_TYP_ICECREAMSPLAT,
    HAZ_TYP_SPITSTEAM,
    HAZ_TYP_SPITSPLASH,
    HAZ_TYP_POPPER_V1,
    HAZ_TYP_POPPER_V2,
    HAZ_TYP_POPPER_V3,
    HAZ_TYP_MERVBOMB_V1,
    HAZ_TYP_MERVBOMB_V2,
    HAZ_TYP_MERVBOMB_V3,
    HAZ_TYP_MERVLET_V1,
    HAZ_TYP_MERVLET_V2,
    HAZ_TYP_MERVLET_V3,
    HAZ_TYP_ZAPBALL_V1,
    HAZ_TYP_ZAPBALL_V2,
    HAZ_TYP_ZAPBOOM_V1,
    HAZ_TYP_ZAPBOOM_V2,
    HAZ_TYP_TURBALL_V1,
    HAZ_TYP_TURBALL_V2,
    HAZ_TYP_TURBALL_V3,
    HAZ_TYP_TURBOOM,
    HAZ_TYP_GOLFBALL,
    HAZ_TYP_GOLFBOOM,
    HAZ_TYP_GOLFJUNK_A,
    HAZ_TYP_GOLFJUNK_B,
    HAZ_TYP_GOLFJUNK_C,
    HAZ_TYP_GOLFJUNK_D,
    HAZ_TYP_GOLFJUNK_E,
    HAZ_TYP_DENNIS_KNIFE,
    HAZ_TYP_DENNIS_KNIFEBOOM,
    HAZ_TYP_TUNEBORN,
    HAZ_TYP_TUNEBANG,
    HAZ_TYP_TUNEBOOM,
    HAZ_TYP_TUNEFLYT,
    HAZ_TYP_SWIMWAKE,
    HAZ_TYP_BOWWAVE,
    HAZ_TYP_SPLASH_RING,
    HAZ_TYP_SPLISH_DROP,
    HAZ_TYP_VISSPLASH_WAVE,
    HAZ_TYP_VISSPLASH_DROP,
    HAZ_TYP_VISSPLASH_WATER,
    HAZ_TYP_VISSPLASH_CHOC,
    HAZ_TYP_VISSPLASH_LAVA,
    HAZ_TYP_VISSPLASH_BILE,
    HAZ_TYP_VISSPLASH_OIL,
    HAZ_TYP_VISSPLASH_SAUCE,
    HAZ_TYP_VISSPLASH_TOXIC,
    HAZ_TYP_REFLECT_V1,
    HAZ_TYP_PLYRBANG_V1,
    HAZ_TYP_POWERUP_EXPLOSION_V1,
    HAZ_TYP_NOMORE,
    HAZ_TYP_FORCE = 0x7fffffff
};

enum en_npcdmg
{
    DMGTYP_UNDECIDED,
    DMGTYP_ABOVE,
    DMGTYP_BELOW,
    DMGTYP_SIDE,
    DMGTYP_INSTAKILL,
    DMGTYP_WOUNDEVENT,
    DMGTYP_KILLEVENT,
    DMGTYP_NMEATTACK,
    DMGTYP_HITBYTOSS,
    DMGTYP_NMEKNOCKED,
    DMGTYP_ROPE,
    DMGTYP_CRUISEBUBBLE,
    DMGTYP_FRIENDLYFIRE,
    DMGTYP_REFLECTED,
    DMGTYP_BOULDER,
    DMGTYP_BUBBOWL,
    DMGTYP_THUNDER_TIKI_EXPLOSION,
    DMGTYP_DAMAGE_SURFACE,
    DMGTYP_BUNGEED,
    DMGTYP_SURFACE,
    DMGTYP_CARTWHEEL,
    DMGTYP_CARSMASH,
    DMGTYP_SLIDE,
    DMGTYP_EXPLOSION,
    DMGTYP_BBOWLZAP,
    DMGTYP_NOMORE,
    DMGTYP_FORCEINT = 0x7fffffff
};

enum en_plyrpup
{
    ePowerupLevelDisabled,
    ePowerupLevelNormal,
    ePowerupLevelLevelMax,
    ePowerupLevelHealthMax,
    ePowerupLevelCount
};

enum en_denstage
{
    DEN_STAGE_UNKNOWN,
    DEN_STAGE_INTRO,
    DEN_STAGE_A,
    DEN_STAGE_B,
    DEN_STAGE_C,
    DEN_STAGE_OUTRO,
    DEN_STAGE_NOMORE
};

enum en_dbcmode
{
    DEN_CAMMOD_UNKNOWN,
    DEN_CAMMOD_NORMAL,
    DEN_CAMMOD_BINARY,
    DEN_CAMMOD_NOMORE
};

enum en_vis
{
    VIS_PREPARE_TO_DIE,
    VIS_HORROR_FLIK,
    VIS_OH_DEAR_GAHD,
    VIS_GET_BACK_HERE,
    VIS_I_SEEE_YOUUU,
    VIS_GREY_AND_FOGGY,
    VIS_LIKE_A_BIRDIE,
    VIS_EARTHWORM_JIM,
    VIS_NOBODY_AROUND,
    VIS_NOMORE
};

enum en_dtbcmode
{
    DENT_CAMMOD_UNKNOWN,
    DENT_CAMMOD_NORMAL,
    DENT_CAMMOD_BINARY,
    DENT_CAMMOD_NOMORE
};

enum en_dentstage
{
    DENT_STAGE_UNKNOWN,
    DENT_STAGE_INTRO,
    DENT_STAGE_A,
    DENT_STAGE_B,
    DENT_STAGE_C,
    DENT_STAGE_OUTRO,
    DENT_STAGE_NOMORE
};

struct xGroupAsset : xBaseAsset
{
    U16 itemCount;
    U16 groupFlags;
};

struct xGroup : xBase // Move after filling in more headers
{
    xGroupAsset* asset;
    xBase** item;
    U32 last_index;
    S32 flg_group;
};

struct OriginalBackupData
{
    struct
    {
        S32 flg_enableWander : 1;
        S32 flg_enablePatrol : 1;
        S32 flg_enableDetect : 1;
        S32 flg_activeOn : 1;
        S32 flg_takeNoDamage : 1;
        S32 flg_unused : 27;
    };
    union
    {
        en_allow overrideDetect;
        S32 alignmeproperly;
    };
    union
    {
        en_allow overrideAttack;
        S32 alignmeproperlyToo;
    };
};

struct Restore
{
    U8 chkby;
    U8 penby;
    U8 padpad[2];
    union
    {
        en_npcgol gid_compare;
        S32 gid_COMPARE;
    };
};

struct NMERuntime
{
    struct
    {
        S32 flg_enableWander : 1;
        S32 flg_enablePatrol : 1;
        S32 flg_enableDetect : 1;
        S32 flg_activeOn : 1;
        S32 flg_takeNoDamage : 1;
        S32 flg_unused : 27;
    };
    union
    {
        en_allow overrideDetect;
        S32 alignmeproperly;
    };
    union
    {
        en_allow overrideAttack;
        S32 alignmeproperlyToo;
    };
    OriginalBackupData orig;
    Restore restore;
};

struct zNMEDriver
{
    xEnt* ent_driver;
    F32 tym_mount;
    xMat4x3 mat_parLast;
    xMat4x3 mat_ownerLast;
    struct
    {
        S32 flg_inContact : 1;
        S32 flg_matchOrient : 1;
        S32 flg_unused : 30;
    };

    void WheelTurnsYou_C(xMat4x3* mat_owner);
    void ReviewCollide(xEntCollis* npccol);
};

struct zNMECommon;

struct zMovePoint : xMovePoint
{
};

struct zNMENavNet
{
    zMovePoint* nav_past;
    zMovePoint* nav_curr;
    zMovePoint* nav_dest;
    zMovePoint* nav_lead;
    xSpline3* spl_mvptspline;
    F32 len_mvptspline;
    F32 dst_curspline;
    zNMECommon* nme_owner;

    S32 MvptCycle();
};

struct zNMESoundTable
{
    iSndGroupHandle sndGroupHandle[10];
    unsigned char* assetNames[10];
    F32 timer[10];
    F32 time[10];
    S32 flags[10];
};

struct xDynAsset : xBaseAsset
{
    U32 type;
    U16 version;
    U16 handle;
};

struct zNMEAsset : xDynAsset
{
    xEntAsset ent_asset;
};

struct Damage_1
{
    F32 tym_invuln;
};

struct Physics
{
    F32 acc_grav;
    F32 spd_maxFall;
};

struct Movement_1
{
    F32 spd_move;
    F32 acc_move;
    F32 dst_deviant;
    F32 spd_turnrate;
};

struct NMECfgCommon
{
    Damage_1 damage;
    Physics physics;
    Movement_1 movement;
};

struct SimpShadParm
{
    F32 rad_shadow;
    unsigned char* nam_shadowTexture;
    RwRaster* rast_shadow;
};

struct FullShadParm
{
    S32 tobeDetermined;
};

struct ShadowRadii
{
    F32 rad_noShadow;
    F32 rad_complexShadow;
};

struct NMEShadParms
{
    SimpShadParm simpshad;
    FullShadParm fullshad;
    ShadowRadii shadrad;
};

struct zNMEArena
{
    S32 flg_arena;
    xVec3 pos_arena;
    F32 rad_arena;
    zMovePoint* nav_arena;
    zMovePoint* nav_refer_dest;
    zMovePoint* nav_refer_curr;

    zMovePoint* NextBestNav(zMovePoint* nav_from);
    S32 Cycle(zNMECommon* npc, S32 peek);
};

struct NMESysEvent
{
    S32 doLinkEvents;
    S32 handled;
    xBase* from;
    xBase* to;
    union
    {
        U32 toEvent;
        en_xEventTags toEvent_asEnum;
    };
    F32 toParam[4];
    xBase* toParamWidget;
    U32 toParamWidgetID;
};

struct NMEBlastInfo
{
    xVec3 pos_blast;
    F32 rad_blast;
    F32 spd_expand;
};

struct NMEChatInfo
{
    xVec3 pos_chat;
    F32 tym_chat;
};

struct NMESpawnInfo
{
    xVec3 pos_spawn;
    zMovePoint* nav_firstMovepoint;
    zMovePoint* nav_spawnReference;
    S32 spawnSuccess;
};

struct NMETargetInfo
{
    xBase* bas_tgt;
    xVec3 pos_tgt;
};

struct NMEDamageInfo
{
    en_npcdmg dmg_type;
    xBase* dmg_from;
    xVec3 vec_dmghit;
    S32 amt_damage;
    union
    {
        S32 fac_powerup;
        en_plyrpup pup_player;
    };
};

struct NMEStunInfo
{
    F32 tym_stuntime;
    en_carystat carrystate;
    S32 allowStun;
};

struct NMEScriptInfo
{
    U32 aid_playanim;
};

struct NMEMountInfo
{
    xEnt* ent_toMount;
    xCollis* col_forMount;
};

struct NMEDestInfo
{
    xVec3 pos_there;
    zMovePoint* nav_there;
};

struct NMEAreaInfo
{
    zNMECommon* npc_origin;
    xVec3 pos_origin;
};

struct NMEMsg
{
    en_npcmsg msgid;
    U32 sendto;
    U32 from;
    en_msgdata infotype;
    union
    {
        NMESysEvent sysevent;
        NMEBlastInfo blastarea;
        NMEChatInfo chatter;
        NMESpawnInfo spawning;
        NMETargetInfo target;
        NMEDamageInfo dmgdata;
        NMEStunInfo stundata;
        NMEScriptInfo scriptdata;
        NMEMountInfo mountdata;
        NMEDestInfo destdata;
        NMEAreaInfo areadata;
    };
    void* attached;
    NMEMsg* next;
    F32 tmr_delay;
};

struct Health_3
{
    S32 pts_healthMax;
};

struct Ranges_2
{
    F32 rad_tooclose[2];
    F32 rad_lob;
    F32 rad_lobgrey;
    F32 rad_aware;
    F32 rad_awaregrey;
};

struct Attack_3
{
    F32 spd_proj;
    S32 num_salvo;
    F32 tym_shoot;
    F32 dst_spreadHorz;
};

struct Battle_6
{
    F32 tym_reload;
    F32 tym_leapDelay;
    S32 num_atakPerLeap;
};

struct NMECfgDennis
{
    Health_3 health;
    Ranges_2 ranges;
    Attack_3 attack;
    Battle_6 battle;
};

struct DennisLetMeKnow : xPSYNote
{
};

// DenTooMeter : BarMeter FIX LATER
struct DenTooMeter
{
    U8 lastPaused;
    //TextureScroller denTooTexureScroller; FIX LATER

    void update();
    void setup();
    void* ct();
};

struct Health_8
{
    S32 pts_healthMax;
};

struct Ranges_5
{
    F32 rad_tooclose;
    F32 rad_lob;
    F32 rad_lobgrey;
    F32 rad_aware;
    F32 rad_awaregrey;
};

struct Attack_0
{
};

struct ThrowSide
{
    F32 spd_proj;
    S32 num_salvo;
    F32 dst_spreadHorz;
};

struct ThrowToes
{
    F32 spd_proj;
    S32 num_salvo;
    F32 dst_nearTgt;
    F32 dst_farTgt;
    F32 hyt_elevate;
};

struct Battle_2
{
    F32 tym_reload;
    F32 tym_leapDelay;
    S32 num_atakPerLeap;
};

struct NMECfgDenToo
{
    Health_8 health;
    Ranges_5 ranges;
    Attack_0 attack;
    ThrowSide throwSide;
    ThrowToes throwToes;
    Battle_2 battle;
};

#endif
