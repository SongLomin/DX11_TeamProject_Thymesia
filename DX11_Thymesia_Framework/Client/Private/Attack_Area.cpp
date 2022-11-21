#include "stdafx.h"
#include "Attack_Area.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"

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

	m_pShaderCom = Add_Component<CShader>();
	m_pRendererCom = Add_Component<CRenderer>();
	m_pHitColliderCom = Add_Component<CCollider>();

	ZeroMemory(&m_tWeaponDesc, sizeof(WEAPON_DESC));

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements);

	

	return S_OK;
}

HRESULT CAttackArea::Start()
{
	return S_OK;
}

void CAttackArea::Tick(_float fTimeDelta)
{
	if (!m_pHitColliderCom.lock()->Get_Enable())
		return;

	if (m_fLifeTime > 0.f)
	{
		m_fLifeTime -= fTimeDelta;
	}

	else
	{
		Disable_Weapon();
		return;
	}

	if (m_tWeaponDesc.fHitFreq > 0.f)
	{
		if (m_fCurrentFreq > 0.f)
		{
			m_fCurrentFreq -= fTimeDelta;
		}

		else
		{
			// 갱신 시간이 되면 갱신한다.

			m_fCurrentFreq = m_tWeaponDesc.fHitFreq;

			Disable_Weapon();
			Enable_Weapon(m_fLifeTime, m_bSyncTransform);
		}
	}

	__super::Tick(fTimeDelta);

}

void CAttackArea::LateTick(_float fTimeDelta)
{
	if (!m_pHitColliderCom.lock()->Get_Enable())
		return;

	//부모 게임 오브젝트가 없음.
	if (!m_pParentTransformCom.lock())
	{
		DEBUG_ASSERT;
	}

	__super::LateTick(fTimeDelta);


	if (m_bSyncTransform)
	{
		Update_TransformWithParent();
		
	}
}

HRESULT CAttackArea::Render()
{
	//SetUp_ShaderResource();

	__super::Render();

	/*_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{

		}

		m_pShaderCom.lock()->Begin(0);

		m_pModelCom.lock()->Render_Mesh(i);
	}*/

#ifdef _DEBUG
	m_pHitColliderCom.lock()->Render();

#endif // _DEBUG



	return S_OK;
}

void CAttackArea::Init_Weapon(const WEAPON_DESC& In_WeaponDesc, weak_ptr<CTransform> In_ParentTransformCom)
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

	m_pHitColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);

	m_pParentCharacter = pParentFromCharacter;

	m_tWeaponDesc = In_WeaponDesc;

	m_pHitColliderCom.lock()->Set_ColliderScale(XMVectorSet(m_tWeaponDesc.fWeaponScale, m_tWeaponDesc.fWeaponScale, m_tWeaponDesc.fWeaponScale, 0.f));
	
	Update_TransformWithParent();

	m_fCurrentFreq = m_tWeaponDesc.fHitFreq;

	Disable_Weapon();
}

void CAttackArea::Enable_Weapon(const _float& In_fLifeTime, const _bool& In_bSyncTransform)
{
	if (m_pHitColliderCom.lock()->Set_Enable(true))
	{
		m_fLifeTime = In_fLifeTime;
		m_bSyncTransform = In_bSyncTransform;
		m_bFirstAttack = true;
	}
}

void CAttackArea::Disable_Weapon()
{
	if (m_pHitColliderCom.lock()->Set_Enable(false))
	{
		m_iHitColliderIndexs.clear();
	}
}

void CAttackArea::Set_WeaponScale(const _float& In_fWeaponScale)
{
	m_pHitColliderCom.lock()->Set_ColliderScale(XMVectorSet(In_fWeaponScale, In_fWeaponScale, In_fWeaponScale, 0.f));
}


weak_ptr<CCharacter> CAttackArea::Get_ParentObject()
{
	return m_pParentCharacter;
}

void CAttackArea::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");
	//m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

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

	m_pHitColliderCom.lock()->Update(m_pTransformCom.lock()->Get_UnScaledWorldMatrix());
}

void CAttackArea::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
	_uint iOtherColliderIndex = pOtherCollider.lock()->Get_ColliderIndex();
	list<_uint>::iterator iter = find(m_iHitColliderIndexs.begin(), m_iHitColliderIndexs.end(), iOtherColliderIndex);

	//이미 충돌했던적이 있음.
	if (m_iHitColliderIndexs.end() != iter)
		return;

	m_iHitColliderIndexs.push_back(iOtherColliderIndex);

	Weak_Cast<CCharacter>(pOtherCollider.lock()->Get_Owner()).lock()->OnHit(m_pHitColliderCom, (HIT_TYPE)m_tWeaponDesc.iHitType, m_tWeaponDesc.fDamage);

	if (m_bFirstAttack)
	{
		if (m_pParentCharacter.lock())
		{
			m_pParentCharacter.lock()->Call_WeaponFirstAttack(pOtherCollider);
		}
		
		m_bFirstAttack = false;
	}
	else
	{
		if (m_pParentCharacter.lock())
		{
			m_pParentCharacter.lock()->Call_WeaponAttack(pOtherCollider);
		}
	}
}

void CAttackArea::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{


}

void CAttackArea::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
}

void CAttackArea::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	m_pHitColliderCom.lock()->Set_Enable(true);
}

void CAttackArea::OnDisable()
{
	__super::OnDisable();
	m_pHitColliderCom.lock()->Set_Enable(false);

}

void CAttackArea::Free()
{
}