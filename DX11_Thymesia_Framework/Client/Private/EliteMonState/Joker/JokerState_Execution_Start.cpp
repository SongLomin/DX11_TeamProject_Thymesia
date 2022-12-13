#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_Execution_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MonsterHPBar_Base.h"
#include "Status_Monster.h"

GAMECLASS_C(CJokerState_TakeExecution_Start);
CLONE_C(CJokerState_TakeExecution_Start, CComponent)

HRESULT CJokerState_TakeExecution_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_TakeExecution_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_TakeExecution_Start::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_TakeExecution_Start");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_TakeExecution_Start::Call_AnimationEnd, this);
}

void CJokerState_TakeExecution_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_TakeExecution_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_TakeExecution_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	m_pOwner.lock()->Get_Component<CStatus_Monster>().lock()->CallBack_UI_Disable();

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif

}

void CJokerState_TakeExecution_Start::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_TakeExecution_Start::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_Execution_Loop>(0.05f);
}

void CJokerState_TakeExecution_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_TakeExecution_Start::Call_AnimationEnd, this);
}

void CJokerState_TakeExecution_Start::Free()
{

}

_bool CJokerState_TakeExecution_Start::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

void CJokerState_TakeExecution_Start::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CEliteStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

