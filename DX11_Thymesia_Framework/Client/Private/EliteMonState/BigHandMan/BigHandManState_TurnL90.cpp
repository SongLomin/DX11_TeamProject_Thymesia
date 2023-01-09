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



GAMECLASS_C(CBigHandManState_TurnL90);
CLONE_C(CBigHandManState_TurnL90, CComponent)

HRESULT CBigHandManState_TurnL90::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBigHandManState_TurnL90::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBigHandManState_TurnL90::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV2Villager_M.ao|LV2Villager01_M_TurnL90");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBigHandManState_TurnL90::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_TurnL90::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_float fTurnValue = 1.57f / 0.85f;

	m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue * -1.5f);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBigHandManState_TurnL90::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBigHandManState_TurnL90::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);



	

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BigHandManState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CBigHandManState_TurnL90::OnStateEnd()
{
	__super::OnStateEnd();


}

void CBigHandManState_TurnL90::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
}

void CBigHandManState_TurnL90::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBigHandManState_TurnL90::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_TurnL90::Free()
{

}

_bool CBigHandManState_TurnL90::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	if (ComputeAngleWithPlayer() > 0.98f)
	{
		Rotation_TargetToLookDir();
		Get_Owner().lock()->Get_Component<CBigHandManState_Idle>().lock()->Set_TurnCheck(false);
		Get_OwnerCharacter().lock()->Change_State<CBigHandManState_Idle>(0.05f);
		return true;
	}



	return false;
}

