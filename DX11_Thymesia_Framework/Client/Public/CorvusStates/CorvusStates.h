#pragma once

#include "GameManager.h"



//플레이어 기본 스테이트 
#include "CorvusStates/CorvusState_Jogging.h"
#include "CorvusStates/CorvusState_JoggingStart.h"
#include "CorvusStates/CorvusState_JoggingStartEnd.h"
#include "CorvusStates/CorvusState_Idle.h"
#include "CorvusStates/CorvusState_Stop.h"
#include "CorvusStates/CorvusState_Hit.h"
#include "CorvusStates/CorvusState_Sprint.h"
#include "CorvusStates/CorvusState_SprintStart.h"
#include "CorvusStates/CorvusState_Run.h"
#include "CorvusStates/CorvusState_RunB.h"
#include "CorvusStates/CorvusState_RunL.h"
#include "CorvusStates/CorvusState_RunR.h"
#include "CorvusStates/CorvusState_AVoid.h"
#include "CorvusStates/CorvusState_AVoidB.h"
#include "CorvusStates/CorvusState_AVoidL.h"
#include "CorvusStates/CorvusState_AVoidR.h"
#include "CorvusStates/CorvusState_ShortAvoid.h"
#include "CorvusStates/CorvusState_AVoidSlash.h"
#include "CorvusStates/CorvusState_AVoidThrust.h"
#include "CorvusStates/CorvusState_SprintAttack.h"
#include "CorvusStates/CorvusState_LAttack1.h"
#include "CorvusStates/CorvusState_LAttack2.h"
#include "CorvusStates/CorvusState_LAttack3.h"
#include "CorvusStates/CorvusState_LAttack4.h"
#include "CorvusStates/CorvusState_LAttack5.h"
#include "CorvusStates/CorvusState_Parry1.h"
#include "CorvusStates/CorvusState_Parry2.h"
#include "CorvusStates/CorvusState_BasicHealing.h"
#include "CorvusStates/CorvusState_ClawAttackTab.h"
#include "CorvusStates/CorvusState_ClawAttackHold.h"
#include "CorvusStates/CorvusState_ClawAttackAway.h"
#include "CorvusStates/CorvusState_ClawAttackTab2.h"
#include "CorvusStates/CorvusState_ClawAttackHoldLoop.h"
#include "CorvusStates/CorvusState_HurtXXL.h"
#include "CorvusStates/CorvusState_HurtL.h"
#include "CorvusStates/CorvusState_HurtR.h"
#include "CorvusStates/CorvusState_Die.h"
#include "CorvusStates/CorvusState_ParryDeflectLeft.h"
#include "CorvusStates/CorvusState_ParryDeflectLeftup.h"
#include "CorvusStates/CorvusState_ParryDeflectRight.h"
#include "CorvusStates/CorvusState_ParryDeflectRightup.h"
#include "CorvusStates/CorvusState_PS_Axe.h"
#include "CorvusStates/CorvusState_PS_CaneSword.h"
#include "CorvusStates/CorvusState_PS_Knife.h"
#include "CorvusStates/CorvusState_PS_Magician.h"
#include "CorvusStates/CorvusState_PS_UrdSword.h"
#include "CorvusStates/CorvusState_PS_VargSword.h"
#include "CorvusStates/CorvusState_Climb_L_Down.h"
#include "CorvusStates/CorvusState_Climb_L_Down_End.h"
#include "CorvusStates/CorvusState_Climb_L_Idle.h"
#include "CorvusStates/CorvusState_Climb_L_UP.h"
#include "CorvusStates/CorvusState_Climb_L_UP_End.h"
#include "CorvusStates/CorvusState_Climb_R_Down.h"
#include "CorvusStates/CorvusState_Climb_R_Down_End.h"
#include "CorvusStates/CorvusState_Climb_R_Idle.h"
#include "CorvusStates/CorvusState_Climb_R_UP.h"
#include "CorvusStates/CorvusState_Climb_R_UP_End.h"
#include "CorvusStates/CorvusState_Climb_Start.h"
#include "CorvusStates/CorvusState_Climb_R_Up_Down_End.h"
#include "CorvusStates/CorvusState_Climb_L_Up_Down_End.h"




#include "CorvusStates/CorvusState_Fall_End.h"
#include "CorvusStates/CorvusState_Fall_Loop.h"
#include "CorvusStates/CorvusState_Fall_Start.h"
#include "CorvusStates/CorvusState_Climb_Fall_Attack.h"
#include "CorvusStates/CorvusState_CheckPointEnd.h"
#include "CorvusStates/CorvusState_CheckPointLoop.h"
#include "CorvusStates/CorvusState_CheckPointStart.h"



//플레이어 기본 스테이트

//플레이어 처형 스테이트
#include "CorvusStates/StateExecution/Varg_Execution.h"
#include "CorvusStates/StateExecution/CorvusState_NorMob_Execution.h"
#include "CorvusStates/StateExecution/CorvusState_RaidAttack1Hurt.h"
//플레이어 처형 스테이트
// 
// 
//#include "CorvusStates/CorvusState_Dash.h"
//#include "CorvusStates/CorvusState_LobbyStand.h"
//#include "CorvusStates/CorvusState_HitDown.h"
//#include "CorvusStates/CorvusState_Born.h"
//#include "CorvusStates/CorvusState_RedSkill.h"
//#include "CorvusStates/CorvusState_BlueSkill.h"
//#include "CorvusStates/CorvusState_YellowSkill.h"
//#include "CorvusStates/CorvusState_QTE.h"
//#include "CorvusStates/CorvusState_UltimateSkill.h"
//#include "CorvusStates/CorvusState_Win.h"
//#include "CorvusStates/CorvusState_StandUp.h"

#include "CCorvus.h"