#include "stdafx.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"
#include "Weapon.h"
#include "Effect_Trail.h"
#include "VIBuffer_Trail.h"

GAMECLASS_C(CWeapon);
CLONE_C(CWeapon, CGameObject);

HRESULT CWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::Initialize(void* pArg)
{
	CGameObject::Initialize(pArg);

	m_pModelCom = Add_Component<CModel>();
	m_pShaderCom = Add_Component<CShader>();
	m_pRendererCom = Add_Component<CRenderer>();

	m_vOffset = { 0.f, 0.f, 0.f };

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements);

	return S_OK;
}

HRESULT CWeapon::Start()
{
	return S_OK;
}

void CWeapon::Tick(_float fTimeDelta)
{
	// 부모 틱 돌면 안됨!!

	//부모 게임 오브젝트가 없음.
	if (!m_pParentTransformCom.lock())
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

	m_pTransformCom.lock()->Set_WorldMatrix(ParentMatrix * m_pParentTransformCom.lock()->Get_WorldMatrix());
		
	for (auto& elem : m_pHitColliderComs)
	{
		elem.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
	}
}

void CWeapon::LateTick(_float fTimeDelta)
{
	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

HRESULT CWeapon::Render()
{
	SetUp_ShaderResource();

	__super::Render();

	_int iPassIndex;
	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{

		}

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			iPassIndex = 0;
		}
		else
		{
			iPassIndex = 3;
		}

		m_pShaderCom.lock()->Begin(iPassIndex);

		m_pModelCom.lock()->Render_Mesh(i);
	}

	

	return S_OK;
}

void CWeapon::Init_Weapon(weak_ptr<CModel> In_pModelCom, weak_ptr<CTransform> In_ParentTransformCom, const string& szTargetNode)
{
	//m_pModelCom.lock()->Init_Model(In_pModelCom.lock()->Get_ModelKey());
	m_pParentTransformCom = In_ParentTransformCom;
	m_pTargetBoneNode = In_pModelCom.lock()->Find_BoneNode(szTargetNode);
	m_TransformationMatrix = In_pModelCom.lock()->Get_TransformationMatrix();

	weak_ptr<CCharacter> pParentFromCharacter = Weak_Cast<CCharacter>(In_ParentTransformCom.lock()->Get_Owner());

	if (!pParentFromCharacter.lock())
	{
		// 부모 객체가 캐릭터가 아니거나 삭제된 객체임.
		DEBUG_ASSERT;
	}

	m_pParentCharacter = pParentFromCharacter;
}

void CWeapon::Enable_Weapon()
{
	for (auto& elem : m_pHitColliderComs)
	{
		elem.lock()->Set_Enable(true);
#ifdef _DEBUG_COUT_
		cout << "Enable Weapon!" << endl;
#endif
	}
}

void CWeapon::Init_Trail(TRAIL_DESC& TrailDesc)
{
	m_pTrailEffect = GAMEINSTANCE->Add_GameObject<CEffect_Trail>(m_CreatedLevel, &TrailDesc);
	m_pTrailEffect.lock()->Set_OwnerDesc(m_pTransformCom, m_pTargetBoneNode, m_pModelCom.lock()->Get_ModelData());
	m_pTrailEffect.lock()->Set_Enable(false);
}

void CWeapon::Add_Collider(_fvector In_vOffset, const _float In_fScale, const COLLISION_LAYER In_Layer)
{
	m_pHitColliderComs.push_back(Add_Component<CCollider>());

	COLLIDERDESC tDesc;
	tDesc.iLayer = (_uint)In_Layer;
	tDesc.vRotation = { 0.f, 0.f, 0.f, 0.f };
	tDesc.vScale = { In_fScale, 0.f, 0.f};
	tDesc.vTranslation = {0.f, 0.f, 0.f};
	XMStoreFloat3(&tDesc.vOffset, In_vOffset);

	m_pHitColliderComs.back().lock()->Init_Collider(COLLISION_TYPE::SPHERE, tDesc);
}

_bool CWeapon::Set_TrailEnable(const _bool In_bEnable)
{
	if (!m_pTrailEffect.lock())
		return false;

	return m_pTrailEffect.lock()->Set_Enable(In_bEnable);
	
}


void CWeapon::Disable_Weapon()
{
	for (auto& elem : m_pHitColliderComs)
	{
		elem.lock()->Set_Enable(false);
#ifdef _DEBUG_COUT_
		cout << "Disable Weapon!" << endl;
#endif
		m_iHitColliderIndexs.clear();
	}
}

weak_ptr<CCharacter> CWeapon::Get_ParentCharacter()
{
	return m_pParentCharacter;
}

void CWeapon::Set_WeaponDesc(const HIT_TYPE In_eHitType, const _float In_fDamage)
{
	m_eHitType = In_eHitType;
	m_fDamage = In_fDamage;
}

void CWeapon::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");
	m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

}

void CWeapon::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
	_uint iOtherColliderIndex = pOtherCollider.lock()->Get_ColliderIndex();
	list<_uint>::iterator iter = find(m_iHitColliderIndexs.begin(), m_iHitColliderIndexs.end(), iOtherColliderIndex);

	//이미 충돌했던적이 있음.
	if (m_iHitColliderIndexs.end() != iter)
		return;

	m_iHitColliderIndexs.push_back(iOtherColliderIndex);

	Weak_Cast<CCharacter>(pOtherCollider.lock()->Get_Owner()).lock()->OnHit(m_pHitColliderComs.front(), m_eHitType, m_fDamage);

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

void CWeapon::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
}

void CWeapon::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
}

void CWeapon::OnSetDead()
{
	int i = 0;
}

void CWeapon::Init_Model(const string& strWeaponName, TIMESCALE_LAYER eLayer)
{
	m_pModelCom.lock()->Init_Model(strWeaponName.c_str(), "", (_uint)eLayer);
}

void CWeapon::Free()
{
}