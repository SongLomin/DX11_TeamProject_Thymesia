#include "stdafx.h"
#include "CorvusStates/CorvusState_ParryDeflectRight.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_ParryDeflectRight);
CLONE_C(CCorvusState_ParryDeflectRight, CComponent)

HRESULT CCorvusState_ParryDeflectRight::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_ParryDeflectRight::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_ParryDeflectRight::Start()
{
	__super::Start();

	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_ParryDeflect_R");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_ParryDeflectRight::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_ParryDeflectRight::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_ParryDeflectRight::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}



void CCorvusState_ParryDeflectRight::OnDisable()
{

}

void CCorvusState_ParryDeflectRight::OnStateStart(const _float& In_fAnimationBlendTime)
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

	m_pOwnerFromPlayer.lock()->Set_RimLightDesc(12.f, { 1.f, 0.92f, 0.5f }, 1.f);

	//if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_ParryDeflectRight>().lock())
	//{
	//	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 5);
	//}
	//else
	//{
	//	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	//}
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

	
}

void CCorvusState_ParryDeflectRight::OnStateEnd()
{
	__super::OnStateEnd();

	
}

void CCorvusState_ParryDeflectRight::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_ParryDeflectRight::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_ParryDeflectRight::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_ParryDeflectRight::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_ParryDeflectRight::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CPlayerStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

_bool CCorvusState_ParryDeflectRight::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 7)
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


