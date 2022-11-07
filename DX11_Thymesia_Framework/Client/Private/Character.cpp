#include "stdafx.h"
#include "Character.h"
//#include "BehaviorBase.h"
#include "Navigation.h"
#include "StateBase.h"
#include "Status.h"
#include "Collider.h"

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
	m_pStatus = Add_Component<CStatus>();
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

void CCharacter::Set_RigidColliderEnable(const _bool& In_bEnable)
{
	m_pRigidBodyColliderCom.lock()->Set_Enable(In_bEnable);
}

weak_ptr<CCollider> CCharacter::Get_RigidColliderComponent() const
{
	return m_pRigidBodyColliderCom;
}

weak_ptr<CStateBase> CCharacter::Get_PreState() const
{
	return m_pPreState;
}

weak_ptr<CStateBase> CCharacter::Get_CurState() const
{
	return m_pCurState;
}

void CCharacter::OnHit(weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	m_pCurState.lock()->OnHit(pOtherCollider, In_eHitType, In_fDamage);
}

void CCharacter::Call_WeaponFirstAttack(weak_ptr<CCollider> pOtherCollider)
{
	m_pCurState.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_FIRSTHIT);

}

void CCharacter::Call_WeaponAttack(weak_ptr<CCollider> pOtherCollider)
{


}

void CCharacter::OnLevelEnter()
{
	CNavigation::NAVIDESC tNaviDesc;
	tNaviDesc.m_iCurrentIndex = 0;

	m_pNaviMeshCom.lock()->Init_Navi("NaviMesh", tNaviDesc);
}

void CCharacter::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{

}

void CCharacter::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
}

void CCharacter::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
}

void CCharacter::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_EDITINIT == iArg)
	{
		m_isEdit = true;
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
