#include "stdafx.h"
#include "CorvusStates/CorvusState_ParryDeflectLeftup.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_ParryDeflectLeftup);
CLONE_C(CCorvusState_ParryDeflectLeftup, CComponent)

HRESULT CCorvusState_ParryDeflectLeftup::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_ParryDeflectLeftup::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_ParryDeflectLeftup::Start()
{
	__super::Start();

	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_ParryDeflect_LUp");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_ParryDeflectLeftup::Call_AnimationEnd, this);
}

void CCorvusState_ParryDeflectLeftup::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_ParryDeflectLeftup::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CCorvusState_ParryDeflectLeftup::OnDisable()
{

}

void CCorvusState_ParryDeflectLeftup::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Parry1>().lock())
	{
		m_bParry1ORParry2 = true;
	}
	else
	{
		m_bParry1ORParry2 = false;
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	//if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_ParryDeflectLeftup>().lock())
	//{
	//	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 5);
	//}
	//else
	//{
	//	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	//}
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif

	// GET_SINGLE(CGameManager)->Use_EffectGroup("ParryDeflect_LUp_Sparkle", m_pTransformCom, (_uint)TIMESCALE_LAYER::PLAYER);

	//m_pModelCom.lock()->Set_AnimationSpeed(3.f);
}

void CCorvusState_ParryDeflectLeftup::OnStateEnd()
{
	__super::OnStateEnd();

	//m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}

void CCorvusState_ParryDeflectLeftup::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_ParryDeflectLeftup::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	m_pModelCom.lock()->Set_AnimationSpeed(m_fDebugAnimationSpeed);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_ParryDeflectLeftup::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_ParryDeflectLeftup::Call_AnimationEnd, this);
}

_bool CCorvusState_ParryDeflectLeftup::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.17f)
	{
		if (Check_RequirementParryState())
		{
			if (m_bParry1ORParry2)
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_Parry2>();
			}
			else
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_Parry1>();
			}
		}

		if (Check_RequirementRunState())
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
		}

		return true;
	}

	return false;
}



