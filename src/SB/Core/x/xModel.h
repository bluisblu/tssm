#ifndef XMODEL_H
#define XMODEL_H

#include <types.h>
#include <rpworld.h>
#include <rwcore.h>
#include <rpworld.h>
#include "xMath.h"
#include "xSound.h"

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

struct xMovePointAsset : xBaseAsset
{
    xVec3 pos;
    U16 wt;
    U8 on;
    U8 bezIndex;
    U8 flg_props;
    U8 pad;
    U16 numPoints;
    F32 delay;
    F32 zoneRadius;
    F32 arenaRadius;
};

struct xMovePoint : xBase
{
    xMovePointAsset* asset;
    xVec3* pos;
    xMovePoint** nodes;
    xMovePoint* prev;
    U32 node_wt_sum;
    U8 on;
    U8 pad[3];
    F32 delay;
    xSpline3* spl;
};

struct xAnimPhysicsData
{
    xVec3* tranTable;
    F32* yawTable;
    S32 tranCount;
};

struct xAnimFile
{
    xAnimFile* Next;
    unsigned char* Name;
    U32 ID;
    U32 FileFlags;
    F32 Duration;
    F32 TimeOffset;
    U16 BoneCount;
    U8 NumAnims[2];
    void** RawData;
    xAnimPhysicsData* PhysicsData;
};

struct xAnimSingle;
struct xAnimActiveEffect;

struct xAnimEffect
{
    xAnimEffect* Next;
    U16 Flags;
    U16 Probability;
    F32 StartTime;
    F32 EndTime;
    U32 (*Callback)(U32, xAnimActiveEffect*, xAnimSingle*, void*);
};

struct xAnimActiveEffect
{
    xAnimEffect* Effect;
    union
    {
        U32 Handle;
        iSndHandle SndHandle;
    };
};

struct xAnimState;

struct xAnimTransition
{
    xAnimTransition* Next;
    xAnimState* Dest;
    U32 (*Conditional)(xAnimTransition*, xAnimSingle*, void*);
    U32 (*Callback)(xAnimTransition*, xAnimSingle*, void*);
    U32 Flags;
    U32 UserFlags;
    F32 SrcTime;
    F32 DestTime;
    U16 Priority;
    U16 QueuePriority;
    F32 BlendRecip;
    U16* BlendOffset;
};

struct xAnimTransitionList
{
    xAnimTransitionList* Next;
    xAnimTransition* T;
};

struct xAnimMultiFileBase
{
    U32 Count;
};

struct xAnimMultiFileEntry
{
    U32 ID;
    xAnimFile* File;
};

struct xAnimMultiFile : xAnimMultiFileBase
{
    xAnimMultiFileEntry Files[1];
};

struct xAnimPlay;

struct xAnimState
{
    xAnimState* Next;
    unsigned char* Name;
    U32 ID;
    U32 Flags;
    U32 UserFlags;
    F32 Speed;
    xAnimFile* Data;
    xAnimEffect* Effects;
    xAnimTransitionList* Default;
    xAnimTransitionList* List;
    F32* BoneBlend;
    F32* TimeSnap;
    F32 FadeRecip;
    U16* FadeOffset;
    void* CallbackData;
    xAnimMultiFile* MultiFile;
    void (*BeforeEnter)(xAnimPlay*, xAnimState*, void*);
    void (*StateCallback)(xAnimState*, xAnimSingle*, void*);
    void (*BeforeAnimMatrices)(xAnimPlay*, xQuat*, xVec3*, S32);
};

struct xAnimSingle
{
    U32 SingleFlags;
    xAnimState* State;
    F32 Time;
    F32 CurrentSpeed;
    F32 BilinearLerp[2];
    xAnimEffect* Effect;
    U32 ActiveCount;
    F32 LastTime;
    xAnimActiveEffect* ActiveList;
    xAnimPlay* Play;
    xAnimTransition* Sync;
    xAnimTransition* Tran;
    xAnimSingle* Blend;
    F32 BlendFactor;
    xVec3 PhysDisp;
    F32 YawDisp;
    U32 pad[1];
};

struct xAnimTable
{
    signed char* Name;
    xAnimTransition* TransitionList;
    xAnimState* StateList;
    U32 AnimIndex;
    U32 MorphIndex;
    U32 UserFlags;
};

struct xModelAssetParam
{
    U32 HashID;
    U8 WordLength;
    U8 String[3];
};

struct xModelAssetInfo
{
    U32 Magic;
    U32 NumModelInst;
    U32 AnimTableID;
    U32 CombatID;
    U32 BrainID;
};

struct xModelInstance;

struct xModelPool
{
    xModelPool* Next;
    U32 NumMatrices;
    xModelInstance* List;
};

struct xModelPipe
{
    U32 Flags;
    U8 Layer;
    U8 AlphaDiscard;
    U16 PipePad;
};

struct xEnt;

struct xSurface : xBase
{
    U32 idx;
    U32 type;
    union
    {
        U32 mat_idx;
        xEnt* ent;
        void* obj;
    };
    F32 friction;
    U8 state;
    U8 pad[3];
    void* moprops;
};

struct xModelBucket
{
    RpAtomic* Data;
    RpAtomic* OriginalData;
    union
    {
        xModelInstance* List;
        xModelBucket** BackRef;
    };
    S32 ClipFlags;
    xModelPipe Pipe;
};

struct xLightKitLight
{
    U32 type;
    RwRGBAReal color;
    F32 matrix[16];
    F32 radius;
    F32 angle;
    RpLight* platLight;
};

struct xLightKit
{
    U32 tagID;
    U32 groupID;
    U32 lightCount;
    xLightKitLight* lightList;
};

struct xModelInstance
{
    xModelInstance* Next;
    xModelInstance* Parent;
    xModelPool* Pool;
    xAnimPlay* Anim;
    RpAtomic* Data;
    xModelPipe Pipe;
    U8 InFrustum;
    U8 TrueClip;
    S8 sortBias;
    U8 modelpad;
    F32 RedMultiplier;
    F32 GreenMultiplier;
    F32 BlueMultiplier;
    F32 Alpha;
    F32 FadeStart;
    F32 FadeEnd;
    xSurface* Surf;
    xModelBucket** Bucket;
    xModelInstance* BucketNext;
    xLightKit* LightKit;
    void* Object;
    U16 Flags;
    U8 BoneCount;
    U8 BoneIndex;
    U8* BoneRemap;
    RwMatrixTag* Mat;
    xVec3 Scale;
    xBox animBound;
    xBox combinedAnimBound;
    U32 modelID;
    U32 shadowID;
    RpAtomic* shadowmapAtomic;
    _class_20 anim_coll;
};

struct xModelTag
{
    xVec3 v;
    U32 matidx;
    F32 wt[4];
};

struct activity_data_1
{
};

struct xModelBlur
{
    activity_data_1* activity;

    void render_enable(U8 enabled);
    void render_all();
    void update_all(F32 dt);
    void reset_all();
    void scene_exit();
    void scene_enter();
};

#endif
