#include "stdafx.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Character.h"
#include "Weapon.h"


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
	if (!m_pParentTransformCom.lock() || !m_pTargetBoneNode.lock())
		return;

	_matrix		BoneMatrix 
		= m_pTargetBoneNode.lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&m_TransformationMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	//무기 오프셋 나중에 캐릭터별로 매개변수로 받아서 처리하자.
	//ParentMatrix = SMath::Go_Right(ParentMatrix, m_vOffset.x);
	//ParentMatrix = SMath::Go_Up(ParentMatrix, m_vOffset.y);
	//ParentMatrix = SMath::Go_Straight(ParentMatrix, m_vOffset.z);

	m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * m_pParentTransformCom.lock()->Get_WorldMatrix());
		
	for (auto& elem : m_pHitColliderComs)
		elem.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CWeapon::LateTick(_float fTimeDelta)
{
	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONALPHABLEND, Cast<CGameObject>(m_this));
}

HRESULT CWeapon::Render(ID3D11DeviceContext* pDeviceContext)
{
	SetUp_ShaderResource();

	__super::Render(pDeviceContext);

	_int iPassIndex;
	m_iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	if (m_bWeaponRenderOnOff)
	{
		for (_uint i(0); i < m_iNumMeshContainers; ++i)
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

				if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
				{
					iPassIndex = 14;
				}
				else
					iPassIndex = 13;
			}

			m_pShaderCom.lock()->Begin(iPassIndex, pDeviceContext);
			m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
		}
	}
	

	return S_OK;
}

void CWeapon::Init_Weapon(weak_ptr<CModel> In_pModelCom, weak_ptr<CTransform> In_ParentTransformCom, const string& szTargetNode)
{
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

void CWeapon::Weapon_BoneChange(weak_ptr<CModel> In_pModelCom,const string& szTargetNode)
{
	m_pTargetBoneNode = In_pModelCom.lock()->Find_BoneNode(szTargetNode);
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

void CWeapon::Set_WeaponDesc(const WEAPON_DESC& In_WeaponDesc)
{
	m_tWeaponDesc = In_WeaponDesc;
}

void CWeapon::Set_WeaponDesc(const HIT_TYPE In_eHitType, const _float In_fDamage, const ATTACK_OPTION In_eOptionType)
{
	m_tWeaponDesc.iHitType = (_int)In_eHitType;
	m_tWeaponDesc.iOptionType = (_int)In_eOptionType;
	m_tWeaponDesc.fDamage = In_fDamage;
}

void CWeapon::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");
	m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	_float fCamFar = GAMEINSTANCE->Get_CameraFar();
	m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

	_vector	vShaderFlag = { 0.f,0.f,0.f,0.f };
	m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector));


}

void CWeapon::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	_uint iOtherColliderIndex = pOtherCollider.lock()->Get_ColliderIndex();
	list<_uint>::iterator iter = find(m_iHitColliderIndexs.begin(), m_iHitColliderIndexs.end(), iOtherColliderIndex);

	//이미 충돌했던적이 있음.
	if (m_iHitColliderIndexs.end() != iter)
		return;

	m_iHitColliderIndexs.push_back(iOtherColliderIndex);

	if ((_uint)COLLISION_LAYER::PLAYER == pOtherCollider.lock()->Get_CollisionLayer())
	{
		Weak_Cast<CCharacter>(pOtherCollider.lock()->Get_Owner()).lock()->OnHit(pOtherCollider, pMyCollider, (HIT_TYPE)m_tWeaponDesc.iHitType, m_tWeaponDesc.fDamage);

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
	
}

void CWeapon::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CWeapon::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CWeapon::Init_Model(const string& strWeaponName, TIMESCALE_LAYER eLayer)
{
	m_pModelCom.lock()->Init_Model(strWeaponName.c_str(), "", (_uint)eLayer);
	m_iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
}

void CWeapon::Free()
{
}