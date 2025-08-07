#ifndef ZTALKBOX_H
#define ZTALKBOX_H

#include "zEntPlayerOOBState.h"
#include "zTextBox.h"

#include "xScene.h"
#include "zNMECommon.h"
#include "xBase.h"
#include "zFX.h"

// DIRECTLY PORTED FROM BFBB

struct xEntNPCAssetIN : xEntNPCAsset
{
    U32 navigation_mesh_id;
    U32 settings;
};

// // Lives here temporarily
// struct base : xEnt, xFactoryInst
// {
//     S16 bound_bone;
//     U16 sound_id_offset;
//     U16 global_parameters_size;
//     U16 local_parameters_size;
//     U32 type;
//     xModelAssetParam* global_parameters;
//     xModelAssetParam* local_parameters;
//     union
//     {
//         xMovePoint* movepoint;
//         U32 movepoint_asset_id;
//     };
//     xEntNPCAssetIN* npc_asset;
//     xModelAssetInfo* model_asset;
//     F32 shadow_strength;
//     F32 shadow_cache_fudge_factor;
//     xVec3 bound_offset;
// };

struct ztalkbox : xBase
{
    enum answer_enum
    {
        ANSWER_CONTINUE,
        ANSWER_YES,
        ANSWER_NO
    };

    struct _class_8
    {
        U8 visible : 1;
    };

    struct asset_type_1 : xDynAsset
    {
        struct _class_3
        {
            struct _class_4
            {
                U8 time : 8;
                U8 prompt : 8;
                U8 sound : 8;
                U8 event : 8;
            };
            _class_4 type;
            F32 delay;
            S32 which_event;
        };

        struct _class_11
        {
            U32 skip;
            U32 noskip;
            U32 quit;
            U32 noquit;
            U32 yesno;
        };

        U32 dialog_box;
        U32 prompt_box;
        U32 quit_box;
        U8 trap : 8;
        U8 pause : 8;
        U8 allow_quit : 8;
        U8 trigger_pads : 8;
        U8 page : 8;
        U8 show : 8;
        U8 hide : 8;
        U8 audio_effect : 8;
        U32 teleport;
        _class_3 auto_wait;
        _class_11 prompt;
    };

    struct _class_16
    {
        char* skip; // originally int8*
        char* noskip;
        char* quit;
        char* noquit;
        char* yesno;
    };

    struct callback
    {
    };

    _class_8 flag;
    asset_type_1* asset;
    ztextbox* dialog_box;
    ztextbox* prompt_box;
    ztextbox* quit_box;
    _class_16 prompt;
    U32 triggerPads;
    base* npc;

    void* permit(unsigned int, unsigned int);
    ztalkbox* get_active();
    void MasterLoveSlave(xBase* slave, S32 starting);
    void hide();
    void show();
    void stop_talk();
    void start_talk(U32 text_id, callback* cb, base* npc);
    void start_talk(char* s, callback* cb, base* npc);
    void set_text(U32 id);
    void set_text(char* s);
    void load(asset_type_1& a);
};

struct state_type
{
    state_enum type;

    void start();
};

#endif
