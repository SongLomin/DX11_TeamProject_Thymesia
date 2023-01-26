#include "stdafx.h"
#include "CorvusStates/CorvusState_Jogging.h"
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "PhysXController.h"

GAMECLASS_C(CCorvusState_Jogging);
CLONE_C(CCorvusState_Jogging, CComponent)



HRESULT CCorvusState_Jogging::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Jogging::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	
	return S_OK;
}

void CCorvusState_Jogging::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_WalkF");
	m_pTransform = m_pOwner.lock()->Get_Component<CTransform>();
}

void CCorvusState_Jogging::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Turn_Transform(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	PxControllerFilters Filters;

	m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);

	//m_pTransform.lock()->Go_Straight(m_fCurrentSpeed * fTimeDelta * 0.5f, m_pNaviCom);
}

void CCorvusState_Jogging::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Jogging::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_fCurrentSpeed = m_fMaxSpeed / 2.f;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Run -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_Jogging::OnStateEnd()
{
	__super::OnStateEnd();

}

void CCorvusState_Jogging::Free()
{
}

_bool CCorvusState_Jogging::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//if (Check_RequirementUltimateState())
	//{
	//	Rotation_NearToLookDir();
	//	Get_OwnerPlayer()->Change_State<CNorMonState_UltimateSkill>();
	//	return true;
	//}
	//
	//if (Check_RequirementAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_NearToLookDir();
	//
	//	Get_OwnerPlayer()->Change_State<CNorMonState_Attack>();
	//	Get_OwnerPlayer()->Get_Component<CNorMonState_Attack>().lock()->Play_AttackWithIndex(0);
	//	return true;
	//}
	//
	//if (Check_RequirementDashState())
	//{
	//	Rotation_InputToLookDir();
	//	Get_OwnerPlayer()->Change_State<CNorMonState_Dash>();
	//	return true;
	//}

	
	if (!KEY_INPUT(KEY::W, KEY_STATE::HOLD)
		&& !KEY_INPUT(KEY::A, KEY_STATE::HOLD)
		&& !KEY_INPUT(KEY::S, KEY_STATE::HOLD)
		&& !KEY_INPUT(KEY::D, KEY_STATE::HOLD))
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_JoggingStartEnd>();
		return true;
	}

	if (Check_RequirementParryState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_Parry1>();
		return true;
	}
	

	return false;
}

