#ifndef XENT_H
#define XENT_H

#include <types.h>
#include "xEnv.h"
#include "xCollis.h"
#include "xGrid.h"
#include "xFX.h"
#include "xShadow.h"

struct xEntAsset : xBaseAsset
{
    U8 flags;
    U8 subtype;
    U8 pflags;
    U8 moreFlags;
    U32 surfaceID;
    xVec3 ang;
    xVec3 pos;
    xVec3 scale;
    F32 redMult;
    F32 greenMult;
    F32 blueMult;
    F32 seeThru;
    F32 seeThruSpeed;
    U32 modelInfoID;
    U32 animListID;
};

struct xModelInstance;

struct xAnimPlay
{
    xAnimPlay* Next;
    U16 NumSingle;
    U16 BoneCount;
    xAnimSingle* Single;
    void* Object;
    xAnimTable* Table;
    xMemPool* Pool;
    xModelInstance* ModelInst;
    void (*BeforeAnimMatrices)(xAnimPlay*, xQuat*, xVec3*, S32);
};

struct xScene
{
    U32 sceneID;
    U16 flags;
    U16 num_trigs;
    U16 num_stats;
    U16 num_dyns;
    U16 num_npcs;
    U16 num_act_ents;
    F32 gravity;
    F32 drag;
    F32 friction;
    U16 num_ents_allocd;
    U16 num_trigs_allocd;
    U16 num_stats_allocd;
    U16 num_dyns_allocd;
    U16 num_npcs_allocd;
    xEnt** trigs;
    xEnt** stats;
    xEnt** dyns;
    xEnt** npcs;
    xEnt** act_ents;
    xEnv* env;
    xMemPool mempool;
    xBase* (*resolvID)(U32);
    unsigned char* (*base2Name)(xBase*);
    unsigned char* (*id2Name)(U32);
};

struct xEntFrame
{
    xMat4x3 mat;
    xMat4x3 oldmat;
    xVec3 oldvel;
    xRot oldrot;
    xRot drot;
    xRot rot;
    xVec3 dvel;
    xVec3 vel;
    U32 mode;
    xVec3 dpos;
};

struct xEntCollis
{
    U8 chk;
    U8 pen;
    U8 env_sidx;
    U8 env_eidx;
    U8 npc_sidx;
    U8 npc_eidx;
    U8 dyn_sidx;
    U8 dyn_eidx;
    U8 stat_sidx;
    U8 stat_eidx;
    U8 idx;
    xCollis colls[18];
    void (*post)(xEnt*, xScene*, F32, xEntCollis*);
    U32 (*depenq)(xEnt*, xEnt*, xScene*, F32, xCollis*);
};

struct xEntShadow
{
    xVec3 pos;
    xVec3 vec;
    RpAtomic* shadowModel;
    F32 dst_cast;
    F32 radius[2];
    struct
    {
        S32 flg_castOnOneDFF : 1;
        S32 flg_castOnAllDFF : 1;
        S32 flg_disableEnvCast : 1;
        S32 flg_shadowUnused : 29;
    };
};

struct xEnt : xBase
{
    xEntAsset* asset;
    U16 idx;
    U8 flags;
    U8 miscflags;
    U8 subType;
    U8 pflags;
    U16 moreFlags;
    struct
    {
        U8 _isCulled : 2;
        U8 collisionEventReceived : 2;
    };
    U8 driving_count;
    U8 num_ffx;
    U8 collType;
    U8 collLev;
    U8 chkby;
    U8 penby;
    void (*visUpdate)(xEnt*);
    xModelInstance* model;
    xModelInstance* collModel;
    xModelInstance* camcollModel;
    void (*update)(xEnt*, xScene*, F32);
    void (*endUpdate)(xEnt*, xScene*, F32);
    void (*bupdate)(xEnt*, xVec3*);
    void (*move)(xEnt*, xScene*, F32, xEntFrame*);
    void (*render)(xEnt*);
    xEntFrame* frame;
    xEntCollis* collis;
    xGridBound gridb;
    xBound bound;
    void (*transl)(xEnt*, xVec3*, xMat4x3*);
    xFFX* ffx;
    xEnt* driver;
    xEnt* driven;
    S32 driveMode;
    xShadowSimpleCache* simpShadow;
    xEntShadow* entShadow;
    anim_coll_data* anim_coll;
    void* user_data;
};

struct xEntNPCAsset
{
    S32 npcFlags;
    S32 npcModel;
    S32 npcProps;
    U32 movepoint;
    U32 taskWidgetPrime;
    U32 taskWidgetSecond;
};

struct xEntNPCAssetIN : xEntNPCAsset
{
    U32 navigation_mesh_id;
    U32 settings;
};

#endif
