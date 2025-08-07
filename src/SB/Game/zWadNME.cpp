#include "zWadNME.h"

void zNMECritBasic::SelfSetup()
{
    xBehaveMgr* bmgr;
    xPsyche* psy;
    bmgr = xBehaveMgr_GetSelf();
    bmgr->Subscribe(this, 0);
    psy = psy_self;
    psy->BrainBegin();
    psy->AddGoal('NGC\0', NULL);
    psy->AddGoal('NGC\1', NULL);
    psy->AddGoal('NGC\4', NULL);
    psy->AddGoal('NGC\2', NULL);
    psy->AddGoal('NGC\3', NULL);
    psy->BrainEnd();
    //*(undefined4 *)(this_01 + 0x44) = 0x4e474300;
}

void zNMECommon::DieTheGoodDeath()
{
}

S32 zNMEGoalDenDead::Exit(F32 dt, void* ctxt)
{
}

S32 zNMEGoalDenDamage::Exit(F32 dt, void* ctxt)
{
    zNMEDennis& npc = *(zNMEDennis*)this->psyche->clt_owner;
    npc.tmr_nextLeap = -1.0f;
    return 0;
}

zNMEGoalDenEvade::PathDirect(zNMEDennis*, F32 dt)
{
}

// Return type not in dwarf, Could be wrong
void zNMEDennis::MeterRender()
{
}

void zNMEDennis::RenderHud()
{
    zNMEDennis::MeterRender();
}

S32 zNMEDennis::DfltVulnFlags()
{
    return 0xc07b0007; // HACKED TOGETHER FOR TESTING PURPOSES
}
