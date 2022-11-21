#include "stdafx.h"
#include "StateBase.h"
#include "GameObject.h"
#include "Client_Components.h"
#include "Character.h"

_uint CStateBase::g_iStateIndex;

GAMECLASS_C(CStateBase);


HRESULT CStateBase::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CStateBase::Initialize(void* pArg)
{
	USE_START(CStateBase);

	m_pOwnerFromCharacter = Weak_Cast<CCharacter>(m_pOwner);
	m_iStateIndex = g_iStateIndex++;

	Set_Enable(false);
	return S_OK;
}

void CStateBase::Start()
{
	m_pTransformCom = m_pOwner.lock()->Get_Component<CTransform>();
	m_pNaviCom = m_pOwner.lock()->Get_Component<CNavigation>();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_pPhysXControllerCom = m_pOwner.lock()->Get_Component<CPhysXController>();

	_uint i = m_pOwner.lock()->Get_ComponentsByType<CStatus>().size();

	m_pStatusCom = m_pOwner.lock()->Get_ComponentByType<CStatus>();
}

void CStateBase::Tick(_float fTimeDelta)
{
}

void CStateBase::LateTick(_float fTimeDelta)
{
}


void CStateBase::Set_Animation(_uint In_iNumIndex, _float In_fFixedPlayRatio)
{
	m_iAnimIndex = In_iNumIndex;

	if(In_fFixedPlayRatio > 0.f)
		m_fFixedPlayRatio = In_fFixedPlayRatio;
}

weak_ptr<CCharacter> CStateBase::Get_OwnerCharacter() const
{
	return m_pOwnerFromCharacter;
}


_bool CStateBase::Check_Requirement()
{
	if (m_pOwnerFromCharacter.lock()->Is_Edit())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() < m_fFixedPlayRatio)
	{
		return false;
	}

	return true;
}

void CStateBase::OnStateStart(const _float& In_fAnimationBlendTime)
{
	CallBack_StateStart();

}

void CStateBase::OnStateEnd()
{
	CallBack_StateEnd();
}

void CStateBase::OnHit(weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{

}

void CStateBase::OnDestroy()
{
	UNUSE_START(CStateBase);
}

void CStateBase::OnEnable(void* _Arg)
{
	//m_pBehavior = m_pOwner.lock()->Get_ComponentByType<CBehaviorBase>();
	m_pNaviCom = m_pOwner.lock()->Get_Component<CNavigation>();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
}

void CStateBase::OnDisable()
{
}

void CStateBase::Free()
{

}



