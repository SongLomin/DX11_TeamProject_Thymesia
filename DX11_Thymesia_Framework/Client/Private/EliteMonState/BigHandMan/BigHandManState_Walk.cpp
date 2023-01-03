#include "stdafx.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"



GAMECLASS_C(CBigHandManState_Walk);
CLONE_C(CBigHandManState_Walk, CComponent)

HRESULT CBigHandManState_Walk::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBigHandManState_Walk::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBigHandManState_Walk::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV2Villager_M.ao|LV2Villager01_M_Walk");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBigHandManState_Walk::Call_AnimationEnd, this);
}

void CBigHandManState_Walk::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBigHandManState_Walk::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBigHandManState_Walk::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BigHandManState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CBigHandManState_Walk::OnStateEnd()
{
	__super::OnStateEnd();


}

void CBigHandManState_Walk::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	
}

void CBigHandManState_Walk::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBigHandManState_Walk::Call_AnimationEnd, this);
}

void CBigHandManState_Walk::Free()
{

}

_bool CBigHandManState_Walk::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	return false;
}

