#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_Dead.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MonsterHPBar_Elite.h"


GAMECLASS_C(CJokerState_Dead);
CLONE_C(CJokerState_Dead, CComponent)

HRESULT CJokerState_Dead::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_Dead::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_Dead::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_Dead");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_Dead::Call_AnimationEnd, this);
}

void CJokerState_Dead::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_Dead::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_Dead::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "JokerState: Idle -> OnStateStart" << endl;
#endif // _DEBUG
#endif // _DEBUG_COUT_

	Weak_StaticCast<CMonster>(m_pOwner).lock()->Get_HPBar().lock()->Set_Enable(false);
}

void CJokerState_Dead::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_Dead::Call_AnimationEnd()
{
}

void CJokerState_Dead::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_Dead::Call_AnimationEnd, this);
}

void CJokerState_Dead::Free()
{

}

_bool CJokerState_Dead::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

