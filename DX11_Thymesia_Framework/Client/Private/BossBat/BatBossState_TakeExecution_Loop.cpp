#include "stdafx.h"
#include "BossBat/BatBossState_TakeExecution_Loop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "MonsterHPBar_Boss.h"
#include "GameManager.h"
#include "UI_Landing.h"

GAMECLASS_C(CBatBossState_TakeExecution_Loop);
CLONE_C(CBatBossState_TakeExecution_Loop, CComponent)

HRESULT CBatBossState_TakeExecution_Loop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_TakeExecution_Loop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_TakeExecution_Loop::Start()
{
	__super::Start();

	m_fDissolveTime = 4.5f;

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_TakeExecution_Loop");

}

void CBatBossState_TakeExecution_Loop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	Get_OwnerMonster()->Set_PassIndex(7);
	m_fDissolveTime -= fTimeDelta;

	_float fDissolveAmount = SMath::Lerp(1.f, -0.1f, min(1.f, m_fDissolveTime / 4.f));
	Get_OwnerMonster()->Set_DissolveAmount(fDissolveAmount);
}


void CBatBossState_TakeExecution_Loop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (0.f > m_fDissolveTime)
	{
		m_pOwner.lock()->Set_Enable(false);
	}

	Check_AndChangeNextState();
}



void CBatBossState_TakeExecution_Loop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BatState: Start -> OnStateStart" << endl;
#endif
#endif

	Weak_StaticCast<CBossMonster>(m_pOwner).lock()->Get_HPBar().lock()->Set_Enable(false);

	GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front().lock()->Call_Landing(CUI_Landing::LANDING_KILL_BOSS, 2.f);

	GET_SINGLE(CGameManager)->Activate_Section(1000, EVENT_TYPE::ON_ENTER_SECTION);
}	


void CBatBossState_TakeExecution_Loop::OnStateEnd()
{
	__super::OnStateEnd();


}



void CBatBossState_TakeExecution_Loop::Free()
{

}

_bool CBatBossState_TakeExecution_Loop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	return false;
}

