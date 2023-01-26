#include "stdafx.h"
#include "Attack_Area.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"
#include "Monster.h"

GAMECLASS_C(CAttackArea);
CLONE_C(CAttackArea, CGameObject);

HIT_TYPE CAttackArea::Get_HitType() const
{
	return (HIT_TYPE)m_tWeaponDesc.iHitType;
}

ATTACK_OPTION CAttackArea::Get_OptionType() const
{
	return (ATTACK_OPTION)m_tWeaponDesc.iOptionType;
}

HRESULT CAttackArea::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAttackArea::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	ZeroMemory(&m_tWeaponDesc, sizeof(ATTACKAREA_DESC));

	m_pHitColliderComs.push_back(Add_Component<CCollider>());

	return S_OK;
}

HRESULT CAttackArea::Start()
{
	return S_OK;
}

void CAttackArea::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);

}

void CAttackArea::LateTick(_float fTimeDelta)
{
	

	__super::LateTick(fTimeDelta);


	
}

HRESULT CAttackArea::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	return S_OK;
}

void CAttackArea::Init_AttackArea(const ATTACKAREA_DESC& In_WeaponDesc, weak_ptr<CTransform> In_ParentTransformCom)
{
	//m_pModelCom.lock()->Init_Model(In_pModelCom.lock()->Get_ModelKey());
	m_pParentTransformCom = In_ParentTransformCom;
	
	weak_ptr<CCharacter> pParentFromCharacter = Weak_Cast<CCharacter>(In_ParentTransformCom.lock()->Get_Owner());

	if (!pParentFromCharacter.lock())
	{
		// 부모 객체가 캐릭터가 아니거나 삭제된 객체임.
		DEBUG_ASSERT;
	}

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));


	ColliderDesc.vScale = _float3(1.f, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);

	ColliderDesc.iLayer = pParentFromCharacter.lock()->Get_AttackCollisionLayer();

	m_pHitColliderComs.front().lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);

	m_pParentCharacter = pParentFromCharacter;

	m_tWeaponDesc = In_WeaponDesc;

	m_pHitColliderComs.front().lock()->Set_ColliderScale(XMVectorSet(m_tWeaponDesc.fWeaponScale, m_tWeaponDesc.fWeaponScale, m_tWeaponDesc.fWeaponScale, 0.f));
	
	Update_TransformWithParent();

	m_fCurrentFreq = m_tWeaponDesc.fHitFreq;

	Disable_Weapon();
}

void CAttackArea::Enable_Weapon(const _float& In_fLifeTime)
{
	for (auto& elem : m_pHitColliderComs)
	{
		if (elem.lock()->Set_Enable(true))
		{
			m_fLifeTime = In_fLifeTime;
			//m_bSyncTransform = In_bSyncTransform;
			m_bFirstAttack = true;
		}
	}
}

void CAttackArea::Disable_Weapon()
{
	for (auto& elem : m_pHitColliderComs)
	{
		if (elem.lock()->Set_Enable(false))
		{
			m_iHitColliderIndexs.clear();
		}
	}

	m_pTransformCom.lock()->Set_WorldMatrix(XMMatrixIdentity());
}


weak_ptr<CCharacter> CAttackArea::Get_ParentObject()
{
	return m_pParentCharacter;
}

void CAttackArea::Update_TransformWithParent()
{
	if (!m_pParentTransformCom.lock())
	{
		DEBUG_ASSERT;
	}

	m_pTransformCom.lock()->Set_WorldMatrix(m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());

	m_pTransformCom.lock()->Go_Right(m_tWeaponDesc.vWeaponOffset.x);
	m_pTransformCom.lock()->Go_Up(m_tWeaponDesc.vWeaponOffset.y);
	m_pTransformCom.lock()->Go_Straight(m_tWeaponDesc.vWeaponOffset.z);

	m_pHitColliderComs.front().lock()->Update(m_pTransformCom.lock()->Get_UnScaledWorldMatrix());
}

_bool CAttackArea::Check_AllDisableCollider()
{
	_bool bDisable = true;

	for (auto& elem : m_pHitColliderComs)
	{
		if (elem.lock()->Get_Enable())
		{
			bDisable = false;
		}
	}

	return bDisable;
}

void CAttackArea::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	_uint iOtherColliderIndex = pOtherCollider.lock()->Get_ColliderIndex();
	list<_uint>::iterator iter = find(m_iHitColliderIndexs.begin(), m_iHitColliderIndexs.end(), iOtherColliderIndex);

	//이미 충돌했던적이 있음.
	if (m_iHitColliderIndexs.end() != iter)
		return;

	m_iHitColliderIndexs.push_back(iOtherColliderIndex);

	weak_ptr<CCharacter> pCharacter = Weak_Cast<CCharacter>(pOtherCollider.lock()->Get_Owner());
	if(pCharacter.lock())
		pCharacter.lock()->OnHit(pOtherCollider, pMyCollider, (HIT_TYPE)m_tWeaponDesc.iHitType, m_tWeaponDesc.fDamage);

	if (m_bFirstAttack)
	{
		if (m_pParentCharacter.lock())
		{
			m_pParentCharacter.lock()->Call_WeaponFirstAttack(pMyCollider, pOtherCollider);
		}
		
		m_bFirstAttack = false;
	}
	else
	{
		if (m_pParentCharacter.lock())
		{
			m_pParentCharacter.lock()->Call_WeaponAttack(pMyCollider, pOtherCollider);
		}
	}
}

void CAttackArea::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{


}

void CAttackArea::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CAttackArea::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	for (auto& elem : m_pHitColliderComs)
	{
		elem.lock()->Set_Enable(true);
	}
}

void CAttackArea::OnDisable()
{
	__super::OnDisable();
	for (auto& elem : m_pHitColliderComs)
	{
		elem.lock()->Set_Enable(false);
	}

}

void CAttackArea::Free()
{
}