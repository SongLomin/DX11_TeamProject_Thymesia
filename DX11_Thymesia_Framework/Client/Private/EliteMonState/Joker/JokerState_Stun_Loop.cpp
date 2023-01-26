#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_Stun_Loop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"



GAMECLASS_C(CJokerState_Stun_Loop);
CLONE_C(CJokerState_Stun_Loop, CComponent)

HRESULT CJokerState_Stun_Loop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_Stun_Loop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_Stun_Loop::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_Stun_Loop");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_Stun_Loop::Call_AnimationEnd, this, placeholders::_1);*/
}

void CJokerState_Stun_Loop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_Stun_Loop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_Stun_Loop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_Stun_Loop::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_Stun_Loop::Free()
{

}

_bool CJokerState_Stun_Loop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	if (Get_OwnerMonster()->Get_EliteExecutionStartOnOff())
	{
		Get_OwnerCharacter().lock()->Change_State<CJokerState_TakeExecution_Start>(0.05f);
		return true;
	}

	return false;
}

