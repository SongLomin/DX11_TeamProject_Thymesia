#include "stdafx.h"
#include "BossVarg/VargBossState_WalkB.h"
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

GAMECLASS_C(CVargBossState_WalkB);
CLONE_C(CVargBossState_WalkB, CComponent)

HRESULT CVargBossState_WalkB::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_WalkB::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_WalkB::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_WalkB");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_WalkB::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_WalkB::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);
	_vector vDirLook = Get_CurMonToStartMonDir();

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->Move(vDirLook * m_fCurrentSpeed * fTimeDelta, 0.f, fTimeDelta, Filters);
}


void CVargBossState_WalkB::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_bFirstLookAt)
	Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}



void CVargBossState_WalkB::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bFirstLookAt = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: WalkB -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_WalkB::OnStateEnd()
{
	__super::OnStateEnd();


}



void CVargBossState_WalkB::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidB>(0.05f);
}



void CVargBossState_WalkB::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_WalkB::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_WalkB::Free()
{

}

_bool CVargBossState_WalkB::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.1f)
	{
		m_bFirstLookAt = false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		
		Get_Owner().lock()->Get_Component<CVargBossState_Idle>().lock()->Set_BackReset(true);
		Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidB>(0.05f);
		return true;
	}
	



	return false;
}

