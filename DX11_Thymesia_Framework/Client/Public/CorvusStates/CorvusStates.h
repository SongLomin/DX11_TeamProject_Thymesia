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
#include "CorvusStates/CorvusState_AVoid.h"
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
#include "CorvusStates/CorvusState_HurtXXL.h"
#include "CorvusStates/CorvusState_HurtL.h"
#include "CorvusStates/CorvusState_HurtR.h"
#include "CorvusStates/CorvusState_Die.h"
#include "CorvusStates/CorvusState_ParryDeflectLeft.h"
#include "CorvusStates/CorvusState_ParryDeflectLeftup.h"
#include "CorvusStates/CorvusState_ParryDeflectRight.h"
#include "CorvusStates/CorvusState_ParryDeflectRightup.h"
#include "CorvusStates/CorvusState_ClawAttackTab.h"

//플레이어 기본 스테이트

//플레이어 처형 스테이트
#include "CorvusStates/StateExecution/Varg_Execution.h"
#include "CorvusStates/StateExecution/CorvusState_NorMob_Execution.h"
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