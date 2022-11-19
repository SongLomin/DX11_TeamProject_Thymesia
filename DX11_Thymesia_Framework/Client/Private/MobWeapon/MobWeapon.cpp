#include "stdafx.h"
#include "MobWeapon/MobWeapon.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"

GAMECLASS_C(CMobWeapon);
CLONE_C(CMobWeapon, CGameObject);

HRESULT CMobWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMobWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pModelCom = Add_Component<CModel>();
	m_pShaderCom = Add_Component<CShader>();
	m_pRendererCom = Add_Component<CRenderer>();
	m_pHitColliderCom = Add_Component<CCollider>();

	m_vOffset = { 0.f, 0.f, 0.f };

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

	m_fOriginalWeaponScale = 0.9f;
	ColliderDesc.vScale = _float3(m_fOriginalWeaponScale, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.iLayer = (_uint)COLLISION_LAYER::PLAYER_ATTACK;

	//m_pHitColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements);

	return S_OK;
}

HRESULT CMobWeapon::Start()
{
	return S_OK;
}

void CMobWeapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//부모 게임 오브젝트가 없음.
	if (!m_pParent.lock().get())
		return;



	_matrix		ParentMatrix = m_pTargetBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&m_TransformationMatrix);

	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	//무기 오프셋 나중에 캐릭터별로 매개변수로 받아서 처리하자.
	ParentMatrix = SMath::Go_Right(ParentMatrix, m_vOffset.x);
	ParentMatrix = SMath::Go_Up(ParentMatrix, m_vOffset.y);
	ParentMatrix = SMath::Go_Straight(ParentMatrix, m_vOffset.z);

	m_pTransformCom.lock()->Set_WorldMatrix(ParentMatrix * m_pParent.lock()->Get_Component<CTransform>().lock()->Get_WorldMatrix());
	//m_pTransformCom.lock()->Set_WorldMatrix(m_pParent.lock()->Get_Component<CTransform>().lock()->Get_WorldMatrix());
	//m_pHitColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());




}

void CMobWeapon::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

HRESULT CMobWeapon::Render()
{
	SetUp_ShaderResource();

	__super::Render();


	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{

		}

		m_pShaderCom.lock()->Begin(0);

		m_pModelCom.lock()->Render_Mesh(i);
	}





#ifdef _DEBUG
	m_pHitColliderCom.lock()->Render();

#endif // _DEBUG



	return S_OK;
}

void CMobWeapon::Init_DefaultWeapon(weak_ptr<CModel> In_pModelCom, weak_ptr<CGameObject> In_pParent, const string& szTargetNode)
{

	//m_pModelCom.lock()->Init_Model(In_pModelCom.lock()->Get_ModelKey());
	m_pParent = In_pParent;
	m_pTargetBoneNode = In_pModelCom.lock()->Find_BoneNode(szTargetNode);
	m_TransformationMatrix = In_pModelCom.lock()->Get_TransformationMatrix();
}

void CMobWeapon::Enable_DefaultWeapon(const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	if (m_pHitColliderCom.lock()->Set_Enable(true))
	{
		#ifdef _DEBUG_COUT_
		cout << "Enable Weapon!" << endl;
#endif
		m_eHitType = In_eHitType;
		m_fDamage = In_fDamage;
		m_bFirstAttack = true;
	}
}

void CMobWeapon::Disable_DefaultWeapon()
{
	if (m_pHitColliderCom.lock()->Set_Enable(false))
	{
		#ifdef _DEBUG_COUT_
		cout << "Disable Weapon!" << endl;
#endif
		m_iHitColliderIndexs.clear();

	}
}

void CMobWeapon::Set_DefaultWeaponScale(const _float& In_fWeaponScale)
{
	m_fWeaponScale = In_fWeaponScale;
	m_pHitColliderCom.lock()->Set_ColliderScale(XMVectorSet(In_fWeaponScale, In_fWeaponScale, In_fWeaponScale, 0.f));

}

void CMobWeapon::Set_OriginalDefaultWeaponScale()
{
	Set_DefaultWeaponScale(m_fOriginalWeaponScale);
}

weak_ptr<CGameObject> CMobWeapon::Get_ParentObject()
{
	return m_pParent;
}

void CMobWeapon::Set_DefaultSaberDesc(const _float& In_fWeaponScale, const _float3& In_vOffset, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	m_fWeaponScale = In_fWeaponScale;
	m_pHitColliderCom.lock()->Set_ColliderScale(XMVectorSet(In_fWeaponScale, In_fWeaponScale, In_fWeaponScale, 0.f));
	m_vOffset = In_vOffset;
	m_eHitType = In_eHitType;
	m_fDamage = In_fDamage;
}

void CMobWeapon::Set_WeaponType(MONSTERWEAPONTYPE WeaponType)
{
	m_eWeaType = WeaponType;

	switch (m_eWeaType)
	{

	case MONSTERWEAPONTYPE::WEAPON_AXE:
		m_pModelCom.lock()->Init_Model("Mon_Weapon_Axe", "", (_uint)TIMESCALE_LAYER::MONSTER);
		break;
	case MONSTERWEAPONTYPE::WEAPON_KNIFE:
		m_pModelCom.lock()->Init_Model("Mon_Weapon_Knife", "", (_uint)TIMESCALE_LAYER::MONSTER);
		break;
	}
}

void CMobWeapon::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");
	//m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

}

void CMobWeapon::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
	// TODO : 콜라이더 작업은 나중에
	_uint iOtherColliderIndex = pOtherCollider.lock()->Get_ColliderIndex();
	list<_uint>::iterator iter = find(m_iHitColliderIndexs.begin(), m_iHitColliderIndexs.end(), iOtherColliderIndex);

	//이미 충돌했던적이 있음.
	if (m_iHitColliderIndexs.end() != iter)
		return;

	m_iHitColliderIndexs.push_back(iOtherColliderIndex);

	// TODO : 콜라이더 작업은 나중에
	// Weak_Cast<CCharacter>(pOtherCollider.lock()->Get_Owner()).lock()->OnHit(m_pHitColliderCom, m_eHitType, m_fDamage);

	if (m_bFirstAttack)
	{
		CallBack_AttackOnce(pOtherCollider);
		CallBack_Attack(pOtherCollider);

		m_bFirstAttack = false;
	}
	else
	{
		CallBack_Attack(pOtherCollider);
	}
}

void CMobWeapon::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
}

void CMobWeapon::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
}

void CMobWeapon::Free()
{
}