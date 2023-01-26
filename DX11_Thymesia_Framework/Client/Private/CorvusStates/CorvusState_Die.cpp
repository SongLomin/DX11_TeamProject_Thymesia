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
#include "UI_FadeMask.h"
#include "Camera_Target.h"


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
	m_fDissolveTime = max(m_fDissolveTime -fTimeDelta,0.f);
	_float fDissolveAmount = SMath::Lerp(1.f, -0.1f, m_fDissolveTime / 4.f);
	Get_OwnerPlayer()->Set_DissolveAmount(fDissolveAmount);

	if (m_bFadeOutTrigger && 3.5f > m_fDissolveTime)
	{
		m_bFadeOutTrigger = false;

		weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Add_GameObject<CUI_FadeMask>(m_pOwner.lock()->Get_CreatedLevel());
		pFadeMask.lock()->Set_Fade_Delay(0.f,1.f,2.f,1.5f,EASING_TYPE::LINEAR);
	}


	if (m_bAnimPlay)
	{
		m_pModelCom.lock()->Play_Animation(fTimeDelta);
	}
	else
	{
		_float4 vOutPos;
		Get_OwnerPlayer()->Set_PassIndex(0);
		if (FAILED(GET_SINGLE(CGameManager)->Respawn_LastCheckPoint(&vOutPos)))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_JoggingStartEnd>();

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
			Get_OwnerPlayer()->Get_Transform()->Set_Position(XMLoadFloat4(&vOutPos));
			GET_SINGLE(CGameManager)->Get_CameraTarget().lock()->OnEventMessage((_uint)EVENT_TYPE::ON_RESPAWN);
		}
		Get_OwnerPlayer()->OnEventMessage((_uint)EVENT_TYPE::ON_RESPAWN);
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

	m_fDissolveTime = 4.5f;
	m_bDissolve = true;
	m_bAnimPlay = true;
	m_fGrayScaleValue = 1.f;
	m_bFadeOutTrigger = true;

	Get_OwnerPlayer()->Set_PassIndex(7);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	GET_SINGLE(CGameManager)->Release_Focus();

	weak_ptr<CUI_Landing> pLanding = GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front();

	pLanding.lock()->Call_Landing(CUI_Landing::LANDING_DEAD);

	m_bAnimPlay = true;

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif


}

void CCorvusState_Die::OnStateEnd()
{
	__super::OnStateEnd();

	weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Add_GameObject<CUI_FadeMask>(m_CreatedLevel);
	pFadeMask.lock()->Set_Fade(1.f, 0.f, 2.f, EASING_TYPE::LINEAR);
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


