#include "stdafx.h"
#include "VargWeapon.h"
#include "Client_Components.h"
#include "BoneNode.h"
#include "GameManager.h"
#include "Effect_Trail_Distortion.h"
#include "Character.h"

GAMECLASS_C(CVargWeapon);
CLONE_C(CVargWeapon, CGameObject);

HRESULT CVargWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVargWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	TRAIL_DESC TrailDesc;
	ZeroMemory(&TrailDesc, sizeof(TRAIL_DESC));


	CModel::NVCLOTH_MODEL_DESC ClothDesc;
	Preset::NvClothMesh::VargWeaponSetting(ClothDesc);

	m_pModelCom.lock()->Init_Model("Boss_VargWeapon", "", (_uint)TIMESCALE_LAYER::MONSTER, &ClothDesc);
	m_iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();

	TrailDesc.iMaxCnt = 100;
	TrailDesc.vPos_0 = _float3(0.f, 0.f, 0.f);
	TrailDesc.vPos_1 = _float3(0.f, 3.f, 0.f);
	m_pTrailDistortion = GAMEINSTANCE->Add_GameObject<CEffect_Trail_Distortion>(LEVEL_GAMEPLAY, &TrailDesc);
	m_pTrailDiffuse = GAMEINSTANCE->Add_GameObject<CEffect_Trail>(LEVEL_GAMEPLAY, &TrailDesc);
	
#ifdef _USE_THREAD_
	Use_Thread(THREAD_TYPE::PRE_BEFORERENDER);
#endif // _USE_THREAD_


	return S_OK;
}

HRESULT CVargWeapon::Start()
{
	__super::Start();
	m_pTrailDistortion.lock()->Set_OwnerDesc(m_pTransformCom, weak_ptr<CBoneNode>(), m_pModelCom.lock()->Get_ModelData());
	m_pTrailDiffuse.lock()->Set_OwnerDesc(m_pTransformCom, weak_ptr<CBoneNode>(), m_pModelCom.lock()->Get_ModelData());

	m_pTrailDistortion.lock()->Set_TextureIndex(0, 869, 8);//781
	m_pTrailDiffuse.lock()->Set_TextureIndex(83, 701, 0);

	m_pTrailDistortion.lock()->Set_Enable(false);
	m_pTrailDiffuse.lock()->Set_Enable(false);

	return S_OK;
}

void CVargWeapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pTrailDistortion.lock()->Update(fTimeDelta, m_pTransformCom, weak_ptr<CBoneNode>(), m_pModelCom.lock()->Get_ModelData());
	m_pTrailDiffuse.lock()->Update(fTimeDelta, m_pTransformCom, weak_ptr<CBoneNode>(), m_pModelCom.lock()->Get_ModelData());

}

void CVargWeapon::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CVargWeapon::Thread_PreBeforeRender(_float fTimeDelta)
{
	__super::Thread_PreBeforeRender(fTimeDelta);


	ID3D11DeviceContext* pDeferredContext = GAMEINSTANCE->Get_BeforeRenderContext();

	m_pModelCom.lock()->Get_MeshContainer(1).lock()->Update_NvClothVertices(pDeferredContext,
		m_pTransformCom.lock()->Get_WorldMatrix(),
		XMVectorSet(0.f, -9.81f, 0.f, 0.f));


	GAMEINSTANCE->Release_BeforeRenderContext(pDeferredContext);
}


HRESULT CVargWeapon::Render(ID3D11DeviceContext* pDeviceContext)
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
				iPassIndex = 11;
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

			// NvCloth
			if (1 == i)
			{
				m_pShaderCom.lock()->Set_Matrix("g_WorldMatrix", XMMatrixIdentity());
			}
			else
			{
				m_pShaderCom.lock()->Set_Matrix("g_WorldMatrix", m_pTransformCom.lock()->Get_WorldMatrix());
			}

			m_pShaderCom.lock()->Begin(iPassIndex, pDeviceContext);
			m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
		}
	}
	return S_OK;
}

_bool CVargWeapon::Set_TrailEnable(const _bool In_bEnable)
{
	if (!m_pTrailDistortion.lock() || !m_pTrailDiffuse.lock())
		return false;
	m_pTrailDistortion.lock()->Set_Enable(In_bEnable);
	m_pTrailDiffuse.lock()->Set_Enable(In_bEnable);
	return true;

}

void CVargWeapon::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CVargWeapon::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CVargWeapon::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CVargWeapon::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CVargWeapon::Free()
{
}