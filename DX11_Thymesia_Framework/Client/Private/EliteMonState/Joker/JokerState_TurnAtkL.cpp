#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_TurnAtkL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MobWeapon.h"

GAMECLASS_C(CJokerState_TurnAtkL);
CLONE_C(CJokerState_TurnAtkL, CComponent)

void CJokerState_TurnAtkL::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 38:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.6f, 1.f, 9.f, 0.7f);
		break;
	}
}

HRESULT CJokerState_TurnAtkL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_TurnAtkL::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CJokerState_TurnAtkL::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_TurnAttackL");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_TurnAtkL::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_TurnAtkL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_TurnAtkL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}

void CJokerState_TurnAtkL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
	pWeapons.front().lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.3f);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CJokerState_TurnAtkL::Call_NextKeyFrame, this, placeholders::_1);

#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CJokerState_TurnAtkL::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CJokerState_TurnAtkL::Call_NextKeyFrame, this, placeholders::_1);
}

void CJokerState_TurnAtkL::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_Idle>(0.05f);
}

void CJokerState_TurnAtkL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_TurnAtkL::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_TurnAtkL::Free()
{
}

_bool CJokerState_TurnAtkL::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

