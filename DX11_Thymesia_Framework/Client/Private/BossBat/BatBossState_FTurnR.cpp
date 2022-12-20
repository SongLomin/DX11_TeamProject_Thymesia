#include "stdafx.h"
#include "BossBat/BatBossState_FTurnR.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_FTurnR);
CLONE_C(CBatBossState_FTurnR, CComponent)

HRESULT CBatBossState_FTurnR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_FTurnR::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_FTurnR::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|FTURNR");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_FTurnR::Call_AnimationEnd, this);
}

void CBatBossState_FTurnR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));

	PxControllerFilters Filters = Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);


	if (m_bTurnCheck)
	{
		_float fTurnValue = 3.14f / 2.5f;

		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue * 2.f);
	}
	
	
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_FTurnR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_FTurnR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pPhysXControllerCom.lock()->Enable_Gravity(true);

	m_bTurnCheck = true;

	Rotation_TargetToLookDir();

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG 
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif

}	


void CBatBossState_FTurnR::OnStateEnd()
{
	__super::OnStateEnd();



}



void CBatBossState_FTurnR::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_FTurnR::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_FTurnR::Call_AnimationEnd, this);
}

void CBatBossState_FTurnR::Free()
{

}



_bool CBatBossState_FTurnR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 150)
	{
		
		if (ComputeAngleWithPlayer() > 0.97f && m_bTurnCheck)
		{
			Rotation_TargetToLookDir();
			m_bTurnCheck = false;

		}
	}

	return false;
}

