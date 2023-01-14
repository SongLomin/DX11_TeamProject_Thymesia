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

	if (m_bAnimPlay)
	{
		m_pModelCom.lock()->Play_Animation(fTimeDelta);
	}
	else
	{
		_float4 vOutPos;

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


