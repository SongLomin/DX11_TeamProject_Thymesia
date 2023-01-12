#include "stdafx.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"
#include "Status_Monster.h"



GAMECLASS_C(CBigHandManState_VS_TakeExecution_01);
CLONE_C(CBigHandManState_VS_TakeExecution_01, CComponent)

HRESULT CBigHandManState_VS_TakeExecution_01::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBigHandManState_VS_TakeExecution_01::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBigHandManState_VS_TakeExecution_01::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV2Villager_M.ao|LV2Villager01_M_VS_TakeExecution_01");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBigHandManState_VS_TakeExecution_01::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_VS_TakeExecution_01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();



	if (!m_bAnimationStop)
	{
		pCurrentPlayer.lock()->Get_Transform()->LookAt2D(Get_OwnerMonster()->Get_Transform()->Get_Position());

		m_pModelCom.lock()->Play_Animation(fTimeDelta);
	}
	
}


void CBigHandManState_VS_TakeExecution_01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBigHandManState_VS_TakeExecution_01::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Get_OwnerMonster()->Release_Monster();

	Rotation_TargetToLookDir();


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BigHandManState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CBigHandManState_VS_TakeExecution_01::OnStateEnd()
{
	__super::OnStateEnd();


}

void CBigHandManState_VS_TakeExecution_01::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
}

void CBigHandManState_VS_TakeExecution_01::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBigHandManState_VS_TakeExecution_01::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_VS_TakeExecution_01::Free()
{

}

_bool CBigHandManState_VS_TakeExecution_01::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;
	//58보다크면 흠


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 58)
	{
		m_bAnimationStop = true;
		m_pOwner.lock()->Get_Component<CStatus_Monster>().lock()->CallBack_UI_Disable();
		
	}

	return false;
}

