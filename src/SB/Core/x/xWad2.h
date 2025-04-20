#include <types.h>
#include <rpworld.h>
#include "xWad5.h"

struct xLinkAsset
{
    U16 srcEvent;
    U16 dstEvent;
    U32 dstAssetID;
    F32 param[4];
    U32 paramWidgetAssetID;
    U32 chkAssetID;
};

struct xBase
{
    U32 id;
    U8 baseType;
    U8 linkCount;
    U16 baseFlags;
    xLinkAsset* link;
    void (*eventFunc)(xBase*, xBase*, U32, F32*, xBase*, U32);
};

struct xBaseAsset
{
    U32 id;
    U8 baseType;
    U8 linkCount;
    U16 baseFlags;
};

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
