#include "stdafx.h"
#include "BossBat/BatBossState_TurnR.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_TurnR);
CLONE_C(CBatBossState_TurnR, CComponent)

HRESULT CBatBossState_TurnR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_TurnR::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_TurnR::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_TurnR90");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_TurnR::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_TurnR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_float fTurnValue = 1.57f / 3.f;

	m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue * 2.f);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_TurnR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CBatBossState_TurnR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif

}	

void CBatBossState_TurnR::OnStateEnd()
{
	__super::OnStateEnd();
}



void CBatBossState_TurnR::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_TurnR::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_TurnR::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_TurnR::Free()
{

}

_bool CBatBossState_TurnR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (ComputeAngleWithPlayer() > 0.97f)
	{
		Rotation_TargetToLookDir();
		Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_TurnCheck(false);
		Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
		return true;
	}

	return false;
}

