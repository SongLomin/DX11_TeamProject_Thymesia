#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_Stun_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"



GAMECLASS_C(CJokerState_Stun_Start);
CLONE_C(CJokerState_Stun_Start, CComponent)

HRESULT CJokerState_Stun_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_Stun_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_Stun_Start::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_Stun_Start");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_Stun_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_Stun_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_Stun_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_Stun_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	Get_OwnerMonster()->Set_RimLightDesc(6.f, { 0.5f,1.f,0.9f }, 1.f);
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_Stun_Start::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_Stun_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_Stun_Loop>(0.05f);
}

void CJokerState_Stun_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_Stun_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_Stun_Start::Free()
{

}

_bool CJokerState_Stun_Start::Check_AndChangeNextState()
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

