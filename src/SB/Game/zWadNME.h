#ifndef ZWadNME_h
#define ZWadNME_h

#include <types.h>

// change to zNMEGoalDenDamage : zNMEGoalCommon
// temp for testing
struct zNMEGoalDenDamage
{
    // S32 GoalHandleMail(NMEMsg* mail);
    //S32 ChkExistence(zNMEDennis* npc, float32 dt, int32& nextgoal, en_trantype& trantype);
    // S32 Process(en_trantype* trantype, float32 dt, void* ctxt);
    S32 Exit(float, void*);
    S32 Enter();
};

// change to zNMEGoalCommon : xGoal
// temp for testing
struct zNMEGoalCommon
{
    U32 anid_played;
    struct
    {
        S32 flg_npcgauto : 8;
        S32 flg_npcgable : 16;
        S32 bul_entered : 1;
        S32 bul_resumed : 1;
        S32 bul_unused : 6;
    };
    struct
    {
        S32 flg_info : 16;
        S32 flg_user : 16;
    };

    S32 CollReview();
    S32 GoalHandleMail();
    void Clear();
    S32 Enter();
    S32 Resume();
    //S32 Process(en_trantype* trantyp, F32 dt, void* ctxt);
    // erroring cus of en_trantype
};

struct zNMEGoalDentDamage : zNMEGoalCommon
{
    //S32 GoalHandleMail(NMEMsg* mail);
    //S32 ChkExistence(zNMEDenToo* npc, float32 dt, int32& nextgoal, en_trantype& trantype);
    //S32 Process(en_trantype* trantype, float32 dt, void* ctxt);
    S32 Exit(float, void*);
    S32 Enter();
};

#endif
