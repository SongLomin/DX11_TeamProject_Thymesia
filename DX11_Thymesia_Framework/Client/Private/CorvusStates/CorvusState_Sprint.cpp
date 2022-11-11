#include "stdafx.h"
#include "CorvusStates/CorvusState_Sprint.h"
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"


GAMECLASS_C(CCorvusState_Sprint);
CLONE_C(CCorvusState_Sprint, CComponent)



HRESULT CCorvusState_Sprint::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Sprint::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	
	return S_OK;
}



void CCorvusState_Sprint::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD1_Sprint");
	m_pTransform = m_pOwner.lock()->Get_Component<CTransform>();
}

void CCorvusState_Sprint::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Turn_Transform(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	m_pTransform.lock()->Go_Straight(m_fCurrentSpeed * fTimeDelta * 1.75f, m_pNaviCom);
}

void CCorvusState_Sprint::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Sprint::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_fCurrentSpeed = m_fMaxSpeed / 2.f;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_iDustEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup("Dust", m_pTransformCom);

#ifdef _DEBUG
	cout << "LuxiyaState: Run -> OnStateStart" << endl;

#endif
}

void CCorvusState_Sprint::OnStateEnd()
{
	__super::OnStateEnd();

	GET_SINGLE(CGameManager)->UnUse_EffectGroup("Dust", m_iDustEffectIndex);
}

void CCorvusState_Sprint::Free()
{
}

_bool CCorvusState_Sprint::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//if (Check_RequirementUltimateState())
	//{
	//	Rotation_NearToLookDir();
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_UltimateSkill>();
	//	return true;
	//}
	//
	//if (Check_RequirementAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_NearToLookDir();
	//
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_Attack>();
	//	Get_OwnerPlayer()->Get_Component<CLuxiyaState_Attack>().lock()->Play_AttackWithIndex(0);
	//	return true;
	//}
	//
	//if (Check_RequirementDashState())
	//{
	//	Rotation_InputToLookDir();
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_Dash>();
	//	return true;
	//}

	if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
	{
		if (!KEY_INPUT(KEY::W, KEY_STATE::HOLD)
			&& !KEY_INPUT(KEY::A, KEY_STATE::HOLD)
			&& !KEY_INPUT(KEY::S, KEY_STATE::HOLD)
			&& !KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
			return true;
		}
	}

	if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_AVoid>();
		return true;
	}


	if (Check_RequirementAttackState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_SprintAttack>();
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

