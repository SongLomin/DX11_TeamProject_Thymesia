#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_RunAtkEnd.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MobWeapon.h"

GAMECLASS_C(CJokerState_RunAtkEnd);
CLONE_C(CJokerState_RunAtkEnd, CComponent)

void CJokerState_RunAtkEnd::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 82:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.6f, 1.f, 9.f, 0.7f);
		break;
	}
}

HRESULT CJokerState_RunAtkEnd::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_RunAtkEnd::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CJokerState_RunAtkEnd::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_RunAttackEnd");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_RunAtkEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_RunAtkEnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_RunAtkEnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (m_bAttackLookAtLimit)
		TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_RunAtkEnd::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	m_bAttackLookAtLimit = true;
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CJokerState_RunAtkEnd::Call_NextKeyFrame, this, placeholders::_1);
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CJokerState_RunAtkEnd::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CJokerState_RunAtkEnd::Call_NextKeyFrame, this, placeholders::_1);
}

void CJokerState_RunAtkEnd::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_Idle>(0.05f);
}

void CJokerState_RunAtkEnd::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_RunAtkEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_RunAtkEnd::Free()
{
}

_bool CJokerState_RunAtkEnd::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.2f &&
		m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() <= 0.4f)
	{
		weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
		list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
		pWeapons.front().lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
		m_bAttackLookAtLimit = false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.4f &&
		m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() <= 0.42f)
	{
		weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
		list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
		pWeapons.front().lock()->Set_WeaponDesc(HIT_TYPE::DOWN_HIT, 1.5f);
		m_bAttackLookAtLimit = true;
	}
	else
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	return false;
}

