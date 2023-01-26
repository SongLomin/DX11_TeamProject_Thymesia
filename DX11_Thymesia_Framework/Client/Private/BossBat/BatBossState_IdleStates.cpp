#include "stdafx.h"
#include "BossBat/BatBossState_IdleStates.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_IdleStates)

HRESULT CBatBossState_IdleStates::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_IdleStates::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_IdleStates::Start()
{
	__super::Start();


}

void CBatBossState_IdleStates::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}


void CBatBossState_IdleStates::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

void CBatBossState_IdleStates::Free()
{

}



