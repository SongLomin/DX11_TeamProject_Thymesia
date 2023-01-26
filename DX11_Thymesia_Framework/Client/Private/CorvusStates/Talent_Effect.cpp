#include "stdafx.h"
#include "CorvusStates/Talent_Effect.h"
#include "GameManager.h"
#include "ClientComponent.h"
#include "Component.h"
#include "Player.h"



GAMECLASS_C(CTalent_Effect)
CLONE_C(CTalent_Effect, CComponent)

HRESULT CTalent_Effect::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}
HRESULT CTalent_Effect::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;

}
void CTalent_Effect::Start()
{
	__super::Start();
}

_flag CTalent_Effect::Check_Requirement(weak_ptr<CPlayer> pPlayer)
{
	return 0;
}




void CTalent_Effect::Free()
{
}
