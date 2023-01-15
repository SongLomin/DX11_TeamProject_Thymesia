#include "stdafx.h"
#include "Character.h"
//#include "BehaviorBase.h"
#include "Client_Components.h"

GAMECLASS_C(CCharacter)
CLONE_C(CCharacter, CGameObject)

HRESULT CCharacter::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	
	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pNaviMeshCom = Add_Component<CNavigation>();
	m_pPhysXControllerCom = Add_Component<CPhysXCharacterController>();

	
	m_pRequirementChecker.emplace(hash<string>()("RootMotion"), CRequirementChecker::Create());

	return S_OK;
}

HRESULT CCharacter::Start()
{
	__super::Start();

	if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
	{
		if (m_pHitColliderCom.lock())
			m_pHitColliderCom.lock()->Set_Enable(false);
	}

	return S_OK;
}

_uint CCharacter::Get_CurrentStateIndex() const
{

	return m_pCurState.lock()->Get_StateIndex();
}

_uint CCharacter::Get_PreStateIndex() const
{
	return m_pPreState.lock()->Get_StateIndex();
}

void CCharacter::OnStealMonsterSkill(MONSTERTYPE Monstertype)
{
	
}

void CCharacter::Set_RimLightDesc(const _float In_fLightTime, const _float3 In_vColor, const _float In_fPower, const EASING_TYPE In_eEasingType)
{
	m_fRimLightTime = m_fRimLightTimeAcc = In_fLightTime;
	m_vRimLightColor = In_vColor;
	m_fRimLightPower = In_fPower;
	m_eRimLightEasingType = In_eEasingType;
}



void CCharacter::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pCurState.lock())
		m_pCurState.lock()->Tick(fTimeDelta);

	if (m_pHitColliderCom.lock())
		m_pHitColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

	if (0.f < m_fRimLightTimeAcc)
	{
		m_fRimLightTimeAcc -= fTimeDelta;

		m_fRimLightPower = max(0.f, m_fRimLightPower * m_fRimLightTimeAcc / m_fRimLightTime);

	}
	
}

void CCharacter::Thread_PreLateTick(_float fTimeDelta)
{
	__super::Thread_PreLateTick(fTimeDelta);
#ifdef _USE_THREAD_
	if (m_pCurState.lock())
		m_pCurState.lock()->LateTick(fTimeDelta);
#endif // _USE_THREAD_
}

void CCharacter::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifndef _USE_THREAD_
	if (m_pCurState.lock())
		m_pCurState.lock()->LateTick(fTimeDelta);
#endif // _USE_THREAD_

	Move_RootMotion(fTimeDelta);

#ifdef _USE_GRAVITY_
	PxControllerFilters Filters;

	if (m_pPhysXControllerCom.lock()->Get_Controller())
	{
		m_LastCollisionFlags = m_pPhysXControllerCom.lock()->MoveGravity(fTimeDelta, Filters);

		if (m_LastCollisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_pPhysXControllerCom.lock()->Reset_Gravity();
		}

#ifdef _LIFEGUARD_FOR_FALL_
		_vector vPos = m_pPhysXControllerCom.lock()->Get_Position();
		if (vPos.m128_f32[1] < -50.f)
		{
			vPos.m128_f32[1] = 50.f;
			m_pPhysXControllerCom.lock()->Set_Position(vPos, fTimeDelta, Filters);
		}
#endif
	}
#endif

	

	
}

void CCharacter::Before_Render(_float fTimeDelta)
{
	if (LEVEL::LEVEL_EDIT != m_CreatedLevel)
		m_pPhysXControllerCom.lock()->Synchronize_Transform(m_pTransformCom);

	__super::Before_Render(fTimeDelta);
}

void CCharacter::Move_RootMotion(_float fTimeDelta)
{
	if (!m_pRequirementChecker[hash<string>()("RootMotion")]->Check_Requirments())
	{
		return;
	}

	Move_RootMotion_Internal();
}

weak_ptr<CStateBase> CCharacter::Get_PreState() const
{
	return m_pPreState;
}

weak_ptr<CStateBase> CCharacter::Get_CurState() const
{
	return m_pCurState;
}

weak_ptr<CPhysXController> CCharacter::Get_PhysXController() const
{
	return m_pPhysXControllerCom;
}




void CCharacter::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	m_pCurState.lock()->OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CCharacter::Call_WeaponFirstAttack(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	//m_pCurState.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_FIRSTHIT);

}

void CCharacter::Call_WeaponAttack(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{


}

void CCharacter::OnLevelEnter()
{
	CNavigation::NAVIDESC tNaviDesc;
	tNaviDesc.m_iCurrentIndex = 0;

	//m_pNaviMeshCom.lock()->Init_Navi("NaviMesh", tNaviDesc);
}

void CCharacter::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	m_pCurState.lock()->OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CCharacter::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	m_pCurState.lock()->OnCollisionStay(pMyCollider, pOtherCollider);
}

void CCharacter::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	m_pCurState.lock()->OnCollisionExit(pMyCollider, pOtherCollider);
}

void CCharacter::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_EDITINIT == iArg)
	{
		m_isEdit = true;
		m_pPhysXControllerCom.lock()->Set_Enable(false);
	}
}

void CCharacter::Free()
{
}

_bool CCharacter::Change_State(weak_ptr<CStateBase> pNextState, const _float& In_fAnimationBlendTime)
{

	if (!pNextState.lock().get())
	{
		DEBUG_ASSERT;
		return false;
	}

	if (m_pCurState.lock().get())
	{
		m_pCurState.lock()->OnStateEnd();
		m_pCurState.lock()->Set_Enable(false);
	}

	m_pPreState = m_pCurState;
	m_pCurState = pNextState;

	m_pCurState.lock()->OnStateStart(In_fAnimationBlendTime);
	m_pCurState.lock()->Set_Enable(true);
	return true;
}
