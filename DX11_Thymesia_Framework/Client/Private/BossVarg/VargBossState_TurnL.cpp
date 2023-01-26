#include "stdafx.h"
#include "BossVarg/VargBossState_TurnL.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_TurnL);
CLONE_C(CVargBossState_TurnL, CComponent)

HRESULT CVargBossState_TurnL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_TurnL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_TurnL::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TurnL90");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_TurnL::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_TurnL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_TurnL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	_float fTurnValue = 1.57f / 0.85f;

	m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue * -2.5f);

	Check_AndChangeNextState();
}



void CVargBossState_TurnL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: TurnL -> OnStateStart" << endl;
#endif
#endif
}

void CVargBossState_TurnL::OnStateEnd()
{
	__super::OnStateEnd();
}



void CVargBossState_TurnL::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_TurnL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_TurnL::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_TurnL::Free()
{

}

_bool CVargBossState_TurnL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (ComputeAngleWithPlayer() > 0.94f)
	{
		Rotation_TargetToLookDir();
		Get_Owner().lock()->Get_Component<CVargBossState_Idle>().lock()->Set_TurnCheck(false);
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
		return true;
	}

	return false;
}

