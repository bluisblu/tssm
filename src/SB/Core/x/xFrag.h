#ifndef XFRAG_H
#define XFRAG_H

#include "xMath.h"
#include "xEnt.h"
#include "xPar.h"

enum zFragType
{
    eFragInactive,
    eFragGroup,
    eFragShrapnel,
    eFragParticle,
    eFragProjectile,
    eFragLightning,
    eFragSound,
    eFragShockwave,
    eFragExplosion,
    eFragDistortion,
    eFragFire,
    eFragCount,
    eFragForceSize = 0x7fffffff
};

enum zFragLocType
{
    eFragLocBone,
    eFragLocBoneUpdated,
    eFragLocBoneLocal,
    eFragLocBoneLocalUpdated,
    eFragLocTag,
    eFragLocTagUpdated,
    eFragLocCount,
    eFragLocForceSize = 0x7fffffff
};

struct zFragAsset
{
    zFragType type;
    U32 id;
    U32 parentID[2];
    F32 lifetime;
    F32 delay;
};

struct zFragBone
{
    S32 index;
    xVec3 offset;
};

struct zFragLocInfo
{
    union
    {
        zFragBone bone;
        xModelTag tag;
    };
};

struct zFragLocation
{
    zFragLocType type;
    zFragLocInfo info;
    F32 rand_radius;
};

struct presence_volume
{
    F32 knockback;
    xVec3 center;
    F32 radius;
    F32 damage_player;
    F32 damage_npc;
    F32 damage_other;
};

struct trail_emitter
{
    S32 type;
    xVec3 loc;
    F32 emitted;
    S32 glare_id;
    F32 streak_delay;
    F32 streaks_emitted;
    F32 spark_delay;
    F32 sparks_emitted;
    presence_volume* presence;
    iSndHandle tracking_sound;
};

struct zFrag;

struct zFragGroup
{
    zFrag* list[21];
};

struct zShrapnelAsset;

struct zFragShrapnelAsset : zFragAsset
{
    U32 shrapnelID;
    zShrapnelAsset* shrapAsset;
};

struct zFragShrapnel
{
    zFragShrapnelAsset* fasset;
};

struct zFragParticleAsset : zFragAsset
{
    zFragLocation source;
    zFragLocation vel;
    xParEmitterCustomSettings emit;
    U32 flags;
    U32 parEmitterID;
    zParEmitter* parEmitter;
};

struct zFragParticle
{
    zFragParticleAsset* fasset;
};

struct xCurveAsset
{
};

struct zFragProjectileAsset : zFragAsset
{
    U32 modelInfoID;
    RpAtomic* modelFile;
    zFragLocation launch;
    zFragLocation vel;
    zFragLocation velPlusMinus;
    zFragLocation rot;
    zFragLocation rotPlusMinus;
    F32 bounce;
    S32 maxBounces;
    U32 flags;
    U32 childID;
    zShrapnelAsset* child;
    F32 minScale;
    F32 maxScale;
    U32 scaleCurveID;
    xCurveAsset* scaleCurve;
    F32 gravity;
};

struct zFragProjectile
{
    zFragProjectileAsset* fasset;
    xModelInstance* model;
    xParabola path;
    F32 angVel;
    F32 t;
    F32 tColl;
    S32 numBounces;
    F32 scale;
    F32 alpha;
    F32 bounceC1;
    F32 bounceC2;
    xVec3 N;
    xVec3 axis;
};

struct zFragLightningAsset : zFragAsset
{
    zFragLocation start;
    zFragLocation end;
    U32 startParentID;
    U32 endParentID;
};

struct zLightning;

struct zFragLightning
{
    zFragLightningAsset* fasset;
    xModelInstance* startParent;
    xModelInstance* endParent;
    zLightning* lightning;
};

struct zFragSoundAsset : zFragAsset
{
    U32 assetID;
    zFragLocation source;
};

struct zFragSound
{
    zFragSoundAsset* fasset;
    iSndHandle soundID;
};

struct zFragShockwaveAsset : zFragAsset
{
    U32 modelInfoID;
    F32 birthRadius;
    F32 deathRadius;
    F32 birthVelocity;
    F32 deathVelocity;
    F32 birthSpin;
    F32 deathSpin;
    F32 birthColor[4];
    F32 deathColor[4];
};

struct zFragShockwave
{
    zFragShockwaveAsset* fasset;
    F32 currSize;
    F32 currVelocity;
    F32 deltVelocity;
    F32 currSpin;
    F32 deltSpin;
    F32 currColor[4];
    F32 deltColor[4];
};

struct zFragExplosionAsset : zFragAsset
{
    U32 type;
    zFragLocation loc;
    U32 flags;
};

struct zFragExplosion
{
    zFragExplosionAsset* fasset;
    trail_emitter trail;
    xVec3 initPos;
};

struct zFragDistortionAsset : zFragAsset
{
    U32 type;
    zFragLocation loc;
    U32 flags;
    F32 radius;
    F32 duration;
    F32 intensity;
    F32 freq;
    F32 repeat_delay;
};

struct zFragDistortion
{
    zFragDistortionAsset* fasset;
    xVec3 loc;
    S32 type;
    F32 repeat_time;
};

struct zFragFireAsset : zFragAsset
{
    zFragLocation loc;
    U32 flags;
    F32 radius;
    F32 scale;
    F32 fuel;
    F32 heat;
    F32 damage;
    F32 knockback;
};

struct zFragFire
{
    zFragFireAsset* fasset;
};

struct zFragInfo
{
    union
    {
        zFragGroup group;
        zFragShrapnel shrapnel;
        zFragParticle particle;
        zFragProjectile projectile;
        zFragLightning lightning;
        zFragSound sound;
        zFragShockwave shockwave;
        zFragExplosion explosion;
        zFragDistortion distortion;
        zFragFire fire;
    };
};

struct zFrag
{
    zFragType type;
    zFragInfo info;
    F32 delay;
    F32 alivetime;
    F32 lifetime;
    void (*update)(zFrag*, F32);
    void (*destroy)(zFrag*);
    xModelInstance* parent[2];
    zFrag* prev;
    zFrag* next;
};

struct _class_9
{
    xVec3 endPoint[2];
    xVec3 endPointB;
    xVec3 direction;
    F32 length;
    F32 scale;
    F32 width;
    F32 endParam[2];
    F32 endVel[2];
    F32 paramSpan[2];
};

struct xColor_tag // Will need moved after we fill other files
{
    U8 r;
    U8 g;
    U8 b;
    U8 a;
    RwRGBA rgba;
};

struct zLightning
{
    U32 type;
    U32 flags;
    _class_9 func;
    U32 numStrips;
    F32 alphaRamp;
    xColor_tag color;
    F32 time_left;
    F32 time_total;
    zLightning* nextBranch;
    zLightning* prevBranch;
    F32 parentSegment;
    F32 lastParentSegment;
    F32 segmentsPerMeter;
    F32 branchSpeed;
    S32 damage;
    F32 knockBackSpeed;
    RwRaster* mainTexture;
    RwRaster* branchTexture;
    F32 sparkTimer;
    xVec3 collisionPoint;
    F32 genTime;
    U8 randomizeBranchEndPoint;
    U8 collisionEnabled;
    iSndHandle sndHandle;
    xEnt* followStart;
    xVec3 followStartOffset;
    xEnt* followEnd;
    xVec3 followEndOffset;
    void (*renderCB)(zLightning*, RxObjSpace3DVertex*, RxObjSpace3DVertex*, U32);
    void* context;
    F32 weightParam[12];
};

struct zShrapnelAsset
{
    S32 fassetCount;
    U32 shrapnelID;
    void (*initCB)(zShrapnelAsset*, xModelInstance*, xVec3*, void (*)(zFrag*, zFragAsset*));
};

#endif
