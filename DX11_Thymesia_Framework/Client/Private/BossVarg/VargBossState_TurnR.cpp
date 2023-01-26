#include "stdafx.h"
#include "BossVarg/VargBossState_TurnR.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_TurnR);
CLONE_C(CVargBossState_TurnR, CComponent)

HRESULT CVargBossState_TurnR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_TurnR::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CVargBossState_TurnR::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TurnR90");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_TurnR::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_TurnR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_float fTurnValue = 1.57f / 0.85f;

	m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue * 2.f);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_TurnR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_TurnR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: TurnR -> OnStateStart" << endl;
#endif
#endif
	

}

void CVargBossState_TurnR::OnStateEnd()
{
	__super::OnStateEnd();

	
}



void CVargBossState_TurnR::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_TurnR::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_TurnR::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_TurnR::Free()
{
}

_bool CVargBossState_TurnR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (0.94f < ComputeAngleWithPlayer())
	{
		Rotation_TargetToLookDir();
		Get_Owner().lock()->Get_Component<CVargBossState_Idle>().lock()->Set_TurnCheck(false);
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
		return true;
	}

	return false;
}

