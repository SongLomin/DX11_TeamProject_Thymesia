#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_Execution_End.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "GameManager.h"
#include "Inventory.h"



GAMECLASS_C(CJokerState_Execution_End);
CLONE_C(CJokerState_Execution_End, CComponent)

HRESULT CJokerState_Execution_End::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_Execution_End::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_Execution_End::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_TakeExecution_End");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_Execution_End::Call_AnimationEnd, this, placeholders::_1);*/
}

void CJokerState_Execution_End::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_Execution_End::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_Execution_End::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_Execution_End::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_Execution_End::Free()
{

}

_bool CJokerState_Execution_End::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

