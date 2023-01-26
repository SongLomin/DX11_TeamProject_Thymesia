#include "stdafx.h"
#include "CorvusStates/Talent_Sword.h"
#include "GameManager.h"
#include "GameInstance.h"
#include "CorvusStates/Talent.h"
#include "CorvusStates/Talent_Effect_NorSwordLV1.h"
#include "CorvusStates/Talent_Effect_AvoidSlashLV1.h"
#include "CorvusStates/Talent_Effect_AvoidSlashLV2.h"
#include "CorvusStates/Talent_Effect_AvoidThrustLV1.h"
#include "CorvusStates/Talent_Effect_AvoidThrustLV2.h"
#include "Player.h"

GAMECLASS_C(CTalent_Sword)
CLONE_C(CTalent_Sword, CGameObject)

HRESULT CTalent_Sword::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CTalent_Sword::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}



void CTalent_Sword::Free()
{

}

