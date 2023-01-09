#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_WheelAtkStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"



GAMECLASS_C(CJokerState_WheelAtkStart);
CLONE_C(CJokerState_WheelAtkStart, CComponent)

HRESULT CJokerState_WheelAtkStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_WheelAtkStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_WheelAtkStart::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_WheelAttackStart");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_WheelAtkStart::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_WheelAtkStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_WheelAtkStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_WheelAtkStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_WheelAtkStart::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_WheelAtkStart::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_WheelAtkLoop>(0.05f);
}

void CJokerState_WheelAtkStart::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_WheelAtkStart::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_WheelAtkStart::Free()
{

}

_bool CJokerState_WheelAtkStart::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

