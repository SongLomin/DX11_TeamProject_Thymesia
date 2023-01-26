#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_WheelAtkLoop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MobWeapon.h"



GAMECLASS_C(CJokerState_WheelAtkLoop);
CLONE_C(CJokerState_WheelAtkLoop, CComponent)

HRESULT CJokerState_WheelAtkLoop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_WheelAtkLoop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_WheelAtkLoop::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_WheelAttackLoop");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_WheelAtkLoop::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_WheelAtkLoop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_WheelAtkLoop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_WheelAtkLoop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

	pWeapons.front().lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.2f);

	m_iCount += 1;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_WheelAtkLoop::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_WheelAtkLoop::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	if (m_iCount == 3)
	{
		m_iCount = 0;
		Get_OwnerCharacter().lock()->Change_State<CJokerState_WheelAtkEnd>(0.05f);
	}
	else
	{
		Get_OwnerCharacter().lock()->Change_State<CJokerState_WheelAtkLoop>(0.f);
	}
}

void CJokerState_WheelAtkLoop::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_WheelAtkLoop::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_WheelAtkLoop::Free()
{

}

_bool CJokerState_WheelAtkLoop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	
		

	return false;
}

