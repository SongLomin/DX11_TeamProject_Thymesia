#include "stdafx.h"
#include "CorvusStates/CorvusState_Die.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "UI_Landing.h"
#include "GameObject.h"
#include "PhysXCharacterController.h"


GAMECLASS_C(CCorvusState_Die);
CLONE_C(CCorvusState_Die, CComponent)

HRESULT CCorvusState_Die::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Die::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_Die::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Dead_1");
	
}

void CCorvusState_Die::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fGrayScaleValue = max(0.f, m_fGrayScaleValue - fTimeDelta * 0.2f);
	GAMEINSTANCE->Set_GrayScale(m_fGrayScaleValue);

	if (m_bAnimPlay)
	{
		m_pModelCom.lock()->Play_Animation(fTimeDelta);
	}
	else if(m_bDissolve)
	{
		Get_OwnerPlayer()->Set_PassIndex(7);
		m_fDissolveTime -= fTimeDelta;

		_float fDissolveAmount = SMath::Lerp(1.f, -0.1f, m_fDissolveTime / 4.f);
		Get_OwnerPlayer()->Set_DissolveAmount(fDissolveAmount);
		if (m_fDissolveTime < -0.1f)
			m_bDissolve = false;
	}
	else
	{
		_float4 vOutPos;
		Get_OwnerPlayer()->Set_PassIndex(0);
		if (FAILED(GET_SINGLE(CGameManager)->Respawn_LastCheckPoint(&vOutPos)))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_JoggingStartEnd>();
			GAMEINSTANCE->Set_GrayScale(1.f);

		}
		else
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_JoggingStartEnd>();

			PxControllerFilters Filters;
			m_pPhysXControllerCom.lock()->Set_Position
			(
				XMLoadFloat4(&vOutPos),
				0.f,
				Filters
			);
			GAMEINSTANCE->Set_GrayScale(1.f);
		}
	}

}

void CCorvusState_Die::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Die::OnDisable()
{

}

void CCorvusState_Die::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_fDissolveTime = 4.f;
	m_bDissolve = true;
	m_bAnimPlay = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	GET_SINGLE(CGameManager)->Release_Focus();

	weak_ptr<CUI_Landing> pLanding = GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front();

	pLanding.lock()->Call_Landing(CUI_Landing::LANDING_DEAD);


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif


}

void CCorvusState_Die::OnStateEnd()
{
	__super::OnStateEnd();


}

void CCorvusState_Die::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	// Do Nothing
}

//void CCorvusState_Die::Call_AnimationEnd(_uint iEndAnimIndex)
//{
//	if (!Get_Enable())
//		return;
//
//	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
//
//}

void CCorvusState_Die::Free()
{
	
}

_bool CCorvusState_Die::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.99f)
	{
		m_bAnimPlay = false;
	}

	return false;
}


