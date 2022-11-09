#include "stdafx.h"
#include "CorvusStates/CorvusState_SprintStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_SprintStart);
CLONE_C(CCorvusState_SprintStart, CComponent)

HRESULT CCorvusState_SprintStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_SprintStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iAnimIndex = 10;
	return S_OK;
}

void CCorvusState_SprintStart::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_SprintStart::Call_AnimationEnd, this);
}

void CCorvusState_SprintStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);
}

void CCorvusState_SprintStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_SprintStart::OnDisable()
{

}

void CCorvusState_SprintStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "LuxiyaState: RunStart -> OnStateStart" << endl;
#endif

	m_iDustEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup("Dust", m_pTransformCom);
}

void CCorvusState_SprintStart::OnStateEnd()
{
	__super::OnStateEnd();

	GET_SINGLE(CGameManager)->UnUse_EffectGroup("Dust", m_iDustEffectIndex);
}

void CCorvusState_SprintStart::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Sprint>();

}

void CCorvusState_SprintStart::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_SprintStart::Call_AnimationEnd, this);
}

_bool CCorvusState_SprintStart::Check_AndChangeNextState()
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
	
	if (!KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
	{
		if (!KEY_INPUT(KEY::W, KEY_STATE::HOLD)
			&& !KEY_INPUT(KEY::A, KEY_STATE::HOLD)
			&& !KEY_INPUT(KEY::S, KEY_STATE::HOLD)
			&& !KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_JoggingStartEnd>();
			return true;
		}
	}


	if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_AVoid>();
		return true;
	}
	

	return false;
}


