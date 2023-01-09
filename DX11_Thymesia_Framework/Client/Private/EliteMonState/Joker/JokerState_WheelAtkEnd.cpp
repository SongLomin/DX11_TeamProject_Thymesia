#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_WheelAtkEnd.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MobWeapon.h"



GAMECLASS_C(CJokerState_WheelAtkEnd);
CLONE_C(CJokerState_WheelAtkEnd, CComponent)

HRESULT CJokerState_WheelAtkEnd::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_WheelAtkEnd::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_WheelAtkEnd::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_WheelAttackEnd");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_WheelAtkEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_WheelAtkEnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_WheelAtkEnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_bAttackLookAtLimit)
	TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_WheelAtkEnd::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

	pWeapons.front().lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.2f);

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_WheelAtkEnd::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_WheelAtkEnd::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_Idle>(0.05f);
}

void CJokerState_WheelAtkEnd::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_WheelAtkEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_WheelAtkEnd::Free()
{

}

_bool CJokerState_WheelAtkEnd::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;
	

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.3f)
	{
		m_bAttackLookAtLimit = false;
	}

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
	}


	return false;
}

