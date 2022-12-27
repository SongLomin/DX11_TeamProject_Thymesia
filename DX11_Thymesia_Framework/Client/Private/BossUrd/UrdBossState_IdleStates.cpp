#include "stdafx.h"
#include "BossUrd/UrdBossState_IdleStates.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CUrdBossState_IdleStates)

HRESULT CUrdBossState_IdleStates::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_IdleStates::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_IdleStates::Start()
{
	__super::Start();


}

void CUrdBossState_IdleStates::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}


void CUrdBossState_IdleStates::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

void CUrdBossState_IdleStates::Free()
{

}



