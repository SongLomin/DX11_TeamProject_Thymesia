#include "stdafx.h"
#include "CorvusStates/CorvusState_Fall_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"
#include "PhysXController.h"

GAMECLASS_C(CCorvusState_Fall_Start);
CLONE_C(CCorvusState_Fall_Start, CComponent)

HRESULT CCorvusState_Fall_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Fall_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CCorvusState_Fall_Start::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Fall_Start");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Fall_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Fall_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Fall_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Fall_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Fall_Loop>();

}

void CCorvusState_Fall_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pPhysXControllerCom.lock()->Enable_Gravity(true);
	m_pPhysXControllerCom.lock()->Set_EnableSimulation(true);
	m_pPhysXControllerCom.lock()->Set_EnableColliderSimulation(true);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Stop -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_Fall_Start::OnStateEnd()
{
	__super::OnStateEnd();
}

void CCorvusState_Fall_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Fall_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Fall_Start::Free()
{
	
}

_bool CCorvusState_Fall_Start::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//피직즈(땅)이랑 충돌하면 피직스키고 idle로돌아가라

	if (Check_RequirementAttackState())
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_Climb_Fall_Attack>();
		return true;
	}

	PxControllerCollisionFlags Flags = Get_OwnerCharacter().lock()->Get_LastCollisionFlags();

	if ((Flags & PxControllerCollisionFlag::eCOLLISION_DOWN))
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_Fall_End>();
		return true;
	}



	if (Check_RequirementRunState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
		return true;
	}


	return false;
}

