#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_TurnAtkR.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MobWeapon.h"



GAMECLASS_C(CJokerState_TurnAtkR);
CLONE_C(CJokerState_TurnAtkR, CComponent)

HRESULT CJokerState_TurnAtkR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_TurnAtkR::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_TurnAtkR::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_TurnAttackR");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_TurnAtkR::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_TurnAtkR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_TurnAtkR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_TurnAtkR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();

	pWeapons.front().lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.2f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_TurnAtkR::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_TurnAtkR::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_Idle>(0.05f);
}

void CJokerState_TurnAtkR::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_TurnAtkR::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_TurnAtkR::Free()
{

}

_bool CJokerState_TurnAtkR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

