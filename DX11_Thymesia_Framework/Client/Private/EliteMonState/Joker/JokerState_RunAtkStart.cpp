#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_RunAtkStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"

GAMECLASS_C(CJokerState_RunAttackStart);
CLONE_C(CJokerState_RunAttackStart, CComponent)

void CJokerState_RunAttackStart::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 31:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.4f, 1.f, 9.f, 0.25f);
		break;
	}
}

HRESULT CJokerState_RunAttackStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_RunAttackStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CJokerState_RunAttackStart::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_RunAttackStart");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_RunAttackStart::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_RunAttackStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_RunAttackStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	TurnAttack(fTimeDelta);
	Check_AndChangeNextState();
}



void CJokerState_RunAttackStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CJokerState_RunAttackStart::Call_NextKeyFrame, this, placeholders::_1);

#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CJokerState_RunAttackStart::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CJokerState_RunAttackStart::Call_NextKeyFrame, this, placeholders::_1);
}

void CJokerState_RunAttackStart::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_RunAttackLoop>(0.05f);
}

void CJokerState_RunAttackStart::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_RunAttackStart::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_RunAttackStart::Free()
{
}

_bool CJokerState_RunAttackStart::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

