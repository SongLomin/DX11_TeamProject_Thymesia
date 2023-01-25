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

	m_pTransformCom       = m_pOwner.lock()->Get_Component<CTransform>();
	m_pNaviCom            = m_pOwner.lock()->Get_Component<CNavigation>();
	m_pModelCom           = m_pOwner.lock()->Get_Component<CModel>();
	m_pPhysXControllerCom = m_pOwner.lock()->Get_Component<CPhysXCharacterController>();
	
	// TODO : _uint i 이거 뭔 용도?
	//_uint i = _uint(m_pOwner.lock()->Get_ComponentsByType<CStatus>().size());
	m_pStatusCom          = m_pOwner.lock()->Get_ComponentByType<CStatus>();


	Set_Enable(false);
	return S_OK;
}

void CStateBase::Start()
{
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

_float3 CStateBase::Get_ShakingOffset()
{
	_vector vWorldPos = XMVector3TransformNormal(XMLoadFloat3(&m_vShakingOffSet), m_pOwner.lock()->Get_Transform()->Get_WorldMatrix());
	XMStoreFloat3(&m_vShakingOffSet, vWorldPos);
	return m_vShakingOffSet;
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

void CStateBase::Check_AndChangeHitState(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
}

_bool CStateBase::Get_NearGameObjectInDistance(weak_ptr<CGameObject>& Out_pGameObject, list<weak_ptr<CGameObject>> In_pGameObjects, const _float In_fDistance)
{
	//yes
	//?? 방법이두가지 맵을쓰는방법 리스트에서 쏘트를쓰는바업
	// 맵을쓰는방법
	_vector vMyPos = m_pTransformCom.lock()->Get_Position();
	vMyPos.m128_f32[1] = 0.f;

	map<_float, weak_ptr<CGameObject>> SortedMap;
	_vector vOtherPos;
	_float fDistance;
	for (auto& elem : In_pGameObjects)
	{

		vOtherPos = elem.lock()->Get_Component<CTransform>().lock()->Get_Position();
		vOtherPos.m128_f32[1] = 0.f;

		fDistance = XMVectorGetX(XMVector3Length(vMyPos - vOtherPos));

		if (In_fDistance < fDistance)
		{
			continue;
		}
		//맵은자동정렬 
		SortedMap.emplace(fDistance, elem);
	}

	if (SortedMap.empty())
		return false;

	Out_pGameObject = (*SortedMap.begin()).second;

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

void CStateBase::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	HitEffectSound();
}

void CStateBase::OnDestroy()
{
	UNUSE_START(CStateBase);
}

void CStateBase::OnEnable(void* _Arg)
{
}

void CStateBase::OnDisable()
{
}

void CStateBase::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

}

void CStateBase::OnEventMessage(weak_ptr<CBase> pArg)
{

}

void CStateBase::HitEffectSound()
{

}

void CStateBase::Free()
{

}
