#include "stdafx.h"
#include "BossVarg/VargBossState_WalkR.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "PhysXController.h"

GAMECLASS_C(CVargBossState_WalkR);
CLONE_C(CVargBossState_WalkR, CComponent)

HRESULT CVargBossState_WalkR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_WalkR::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_WalkR::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_WalkR");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_WalkR::Call_AnimationEnd, this, placeholders::_1);*/
}

void CVargBossState_WalkR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);



	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	//m_pTransformCom.lock()->Add_PositionWithRotation(XMVectorSet(m_fCurrentSpeed * fTimeDelta, 0.f, 0.f, 1.f), m_pNaviCom);

	PxControllerFilters Filters;

	m_pPhysXControllerCom.lock()->MoveWithRotation({ m_fCurrentSpeed * fTimeDelta, 0.f, 0.f }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}


void CVargBossState_WalkR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bFirstLookAt)
		Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}



void CVargBossState_WalkR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: WalkR -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_WalkR::OnStateEnd()
{
	__super::OnStateEnd();


}


//
//void CVargBossState_WalkR::Call_AnimationEnd(_uint iEndAnimIndex)
//{
//	if (!Get_Enable())
//		return;
//
//
//	Get_OwnerCharacter().lock()->Change_State<CVargBossState_WalkR>(0.05f);
//}

//void CVargBossState_WalkR::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_WalkR::Call_AnimationEnd, this, placeholders::_1);
//}

void CVargBossState_WalkR::Free()
{

}

_bool CVargBossState_WalkR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		m_bFirstLookAt = false;
		Get_Owner().lock()->Get_Component<CVargBossState_Idle>().lock()->Set_BackReset(true);
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidR>(0.05f);
		return true;
	}


	return false;
}

