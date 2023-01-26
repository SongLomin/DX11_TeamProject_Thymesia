#include "stdafx.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"



GAMECLASS_C(CBigHandManState_Stun_Start);
CLONE_C(CBigHandManState_Stun_Start, CComponent)

HRESULT CBigHandManState_Stun_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBigHandManState_Stun_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBigHandManState_Stun_Start::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV2Villager_M.ao|LV2Villager01_M_Stun_Start");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBigHandManState_Stun_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_Stun_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBigHandManState_Stun_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBigHandManState_Stun_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	Get_OwnerMonster()->Set_RimLightDesc(6.f, { 0.5f,1.f,0.9f }, 1.f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BigHandManState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CBigHandManState_Stun_Start::OnStateEnd()
{
	__super::OnStateEnd();


}

void CBigHandManState_Stun_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBigHandManState_Stun_Loop>(0.05f);
}

void CBigHandManState_Stun_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBigHandManState_Stun_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_Stun_Start::Free()
{

}

_bool CBigHandManState_Stun_Start::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	if (Get_OwnerMonster()->Get_EliteExecutionStartOnOff())
	{
		Get_OwnerCharacter().lock()->Change_State<CBigHandManState_VS_TakeExecution_01>(0.05f);
		return true;
	}

	return false;
}

