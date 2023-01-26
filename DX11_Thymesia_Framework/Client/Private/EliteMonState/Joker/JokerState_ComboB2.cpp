#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_ComboB2.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MobWeapon.h"

GAMECLASS_C(CJokerState_ComboB2);
CLONE_C(CJokerState_ComboB2, CComponent)

void CJokerState_ComboB2::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 54:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.6f, 1.f, 9.f, 0.7f);
		break;
	}
}

HRESULT CJokerState_ComboB2::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_ComboB2::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CJokerState_ComboB2::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_ComboB02");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_ComboB2::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_ComboB2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_ComboB2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (m_bAttackLookAtLimit)
		TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_ComboB2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
	pWeapons.front().lock()->Set_WeaponDesc(HIT_TYPE::DOWN_HIT, 1.4f);
	m_bAttackLookAtLimit = false;
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CJokerState_ComboB2::Call_NextKeyFrame, this, placeholders::_1);

#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CJokerState_ComboB2::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CJokerState_ComboB2::Call_NextKeyFrame, this, placeholders::_1);
}

void CJokerState_ComboB2::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_Idle>(0.05f);
}

void CJokerState_ComboB2::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_ComboB2::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_ComboB2::Free()
{
}

_bool CJokerState_ComboB2::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.3f)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	return false;
}

