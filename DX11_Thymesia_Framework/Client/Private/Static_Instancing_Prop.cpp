#include "stdafx.h"
#include "Static_Instancing_Prop.h"
#include "Model.h"
#include "VIBuffer_Model_Instance.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "PhysXCollider.h"
#include "Client_Presets.h"
#include "ImGui_Manager.h"
#include "GameManager.h"
#include "Texture.h"

GAMECLASS_C(CStatic_Instancing_Prop);
CLONE_C(CStatic_Instancing_Prop, CGameObject);

HRESULT CStatic_Instancing_Prop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStatic_Instancing_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModelInstance"),
        VTXMODEL_INSTANCE_DECLARATION::Element,
        VTXMODEL_INSTANCE_DECLARATION::iNumElements
    );

	m_pMaskingTextureCom = Add_Component<CTexture>();
	m_pMaskingTextureCom.lock()->Use_Texture("UVMask");

	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));

	m_pInstanceModelCom = Add_Component<CVIBuffer_Model_Instance>();
	m_pPhysXColliderCom = Add_Component<CPhysXCollider>();
    return S_OK;
}

HRESULT CStatic_Instancing_Prop::Start()
{
    __super::Start();

    return S_OK;
}

void CStatic_Instancing_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CStatic_Instancing_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CStatic_Instancing_Prop::Custom_Thread1(_float fTimeDelta)
{
#ifdef _INSTANCE_CULLING_
	if (m_pDynamicColliderComs.empty())
		m_pInstanceModelCom.lock()->Culling_Instance(std::ref(m_pPropInfos));
#endif
}

void CStatic_Instancing_Prop::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);

	if(!m_pDynamicColliderComs.empty())
		Synchronize_DynamicColliderComs();

#ifdef _INSTANCE_CULLING_
	// Don't Culling DynamicProps
	if(m_pDynamicColliderComs.empty())
		m_pInstanceModelCom.lock()->Update_VisibleInstance();
#endif
}

HRESULT CStatic_Instancing_Prop::Render()
{
	// 오브젝트

	if ("TEMP_Corvus" == m_pInstanceModelCom.lock()->Get_ModelKey())
	{
		return S_OK;
	}

	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	_float4 vCamDesc;
	XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3]);
	m_pShaderCom.lock()->Set_RawValue("g_vCamPosition", &vCamDesc, sizeof(_float4));

	XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2]);
	m_pShaderCom.lock()->Set_RawValue("g_vCamLook", &vCamDesc, sizeof(_float4));

	_float4 vPlayerPos;
	XMStoreFloat4(&vPlayerPos, GET_SINGLE(CGameManager)->Get_PlayerPos());
	m_pShaderCom.lock()->Set_RawValue("g_vPlayerPosition", &vPlayerPos, sizeof(_float4));

	m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 92);


	_vector vShaderFlag = { 0.f, 0.f, 0.f, 0.f };
	m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector));

	_uint iNumMeshContainers = m_pInstanceModelCom.lock()->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (m_bNonCulling)
		{
			if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
				m_iPassIndex = 4;
			else
				m_iPassIndex = 6;
		}
		else
		{
			if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			{
				m_iPassIndex = 0;
			}
			else
			{
				if (m_bInvisibility)
				{
                	if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
						m_iPassIndex = 7;
					else
						m_iPassIndex = 8;
				}
				else
				{
					m_iPassIndex = 3;
				}
			}
		}

		m_pShaderCom.lock()->Begin(m_iPassIndex);
		m_pInstanceModelCom.lock()->Render_Mesh(i);
	}

    return S_OK;
}

HRESULT CStatic_Instancing_Prop::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
	m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

	_uint iNumMeshContainers = m_pInstanceModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		m_pShaderCom.lock()->Begin(2);
		m_pInstanceModelCom.lock()->Render_Mesh(i);
	}

	return S_OK;
}

void CStatic_Instancing_Prop::Write_Json(json& Out_Json)
{
	json PropInfo;

	_uint iIndex = 0;
	for (auto& iter : m_pPropInfos)
	{
		json Prop;

		_float4x4 PropMatrix;
		ZeroMemory(&PropMatrix, sizeof(_float4x4));

		memcpy(&PropMatrix.m[0], &iter.vRotation.x	 , sizeof(_float3));
		memcpy(&PropMatrix.m[1], &iter.vScale.x		 , sizeof(_float3));
		memcpy(&PropMatrix.m[2], &iter.vTarnslation.x, sizeof(_float3));

		PropInfo.emplace(string("Prop Matrix (" + to_string(iIndex++) + ")"), Prop);
	}

	Out_Json.emplace("PropDesc", PropInfo);
	Out_Json.emplace("ModelCom", m_pInstanceModelCom.lock()->Get_ModelKey());

	if (m_bNonCulling)
		Out_Json.emplace("NonCulling", 1);
}

void CStatic_Instancing_Prop::Load_FromJson(const json& In_Json)
{
	for (auto& iter : In_Json.items())
	{
		string szKey = iter.key();

		if ("NonCulling" == szKey)
		{
			m_bNonCulling = (1 == iter.value()) ? (true) : (false);
		}

		else if ("ModelCom" == szKey)
		{
			string szModelTag = iter.value();

			m_pInstanceModelCom.lock()->Init_Model(szModelTag.c_str());
		}

		else if ("PropDesc" == szKey)
		{
			json Desc = iter.value();

			_vector vPosition;
			_vector vOffsetRange;

			for (auto& iter_item : Desc.items())
			{
				_float4x4 PropMatrix;

				CJson_Utility::Load_JsonFloat4x4(iter_item.value(), PropMatrix);

				INSTANCE_MESH_DESC	Desc;
				ZeroMemory(&Desc, sizeof(INSTANCE_MESH_DESC));

				memcpy(&Desc.vRotation   , &PropMatrix.m[0][0], sizeof(_float3));
				memcpy(&Desc.vScale      , &PropMatrix.m[1][0], sizeof(_float3));
				memcpy(&Desc.vTarnslation, &PropMatrix.m[2][0], sizeof(_float3));

				vPosition = XMLoadFloat3(&Desc.vTarnslation);
				vPosition = XMVectorSetW(vPosition, 1.f);
				
				MESH_VTX_INFO tInfo = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;

				vOffsetRange = XMLoadFloat3(&tInfo.vMax) - XMLoadFloat3(&tInfo.vMin);
				vOffsetRange *= XMLoadFloat3(&Desc.vScale);
				Desc.fMaxRange = XMVectorGetX(XMVector3Length(vOffsetRange));
				Desc.vCenter = tInfo.vCenter;
				Desc.Bake_CenterWithMatrix();

				m_pPropInfos.push_back(Desc);
			}
		}

		else if ("Collider_Type" == szKey)
		{
			m_iColliderType = iter.value();
		}
		
	}


	m_pInstanceModelCom.lock()->Init_Instance((_uint)m_pPropInfos.size());
	m_pInstanceModelCom.lock()->Update(m_pPropInfos, true);

#ifdef _GENERATE_PROP_COLLIDER_
	if ((_uint)LEVEL_EDIT != m_CreatedLevel && m_iColliderType != 0)
	{
#ifdef _DEBUG_COUT_
		cout << "Create_PhysX: " << m_pInstanceModelCom.lock()->Get_ModelKey() << endl;
#endif // _DEBUG_COUT_

		Bake_PhysXColliderCom();
	}
#endif // _GENERATE_PROP_COLLIDER_

	
}

void CStatic_Instancing_Prop::Bake_PhysXColliderCom()
{
	_bool bConvex = m_iColliderType == 2;

	switch (m_iColliderType)
	{
	case 0:
		DEBUG_ASSERT;
		break;

		// Mesh, ConvexMesh
	case 1:
	case 2:
	{
		m_pPhysXColliderCom.lock()->Init_ModelInstanceCollider(m_pInstanceModelCom.lock()->Get_ModelData(), m_pPropInfos, m_iColliderType);
		PhysXColliderDesc tDesc;
		Preset::PhysXColliderDesc::StaticInstancingPropSetting(tDesc, m_pTransformCom);
		m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
		m_pPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();
	}
		break;

		// Dynamic Instancing ColliderComs;
	case 3:
	{
		Bake_DynamicColliderComs();
	}
		break;

	default:
		break;
	}
}

void CStatic_Instancing_Prop::Bake_DynamicColliderComs()
{
	weak_ptr<CPhysXCollider> pDynamicPhysXColliderCom;
	PhysXColliderDesc tDesc;
	weak_ptr<MODEL_DATA> pModelData = m_pInstanceModelCom.lock()->Get_ModelData();

	for (auto& elem : m_pPropInfos)
	{
		pDynamicPhysXColliderCom = Add_Component<CPhysXCollider>();
		pDynamicPhysXColliderCom.lock()->Init_ModelCollider(pModelData, true);
		Preset::PhysXColliderDesc::DynamicBottleSetting(tDesc, elem.Get_Matrix());
		pDynamicPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
		pDynamicPhysXColliderCom.lock()->Add_PhysXActorAtScene();
		pDynamicPhysXColliderCom.lock()->PutToSleep();
		m_pDynamicColliderComs.push_back(pDynamicPhysXColliderCom);
	}
}

void CStatic_Instancing_Prop::Synchronize_DynamicColliderComs()
{
	_matrix WorldMatrix;


	for (_size_t i = 0; i < m_pPropInfos.size(); ++i)
	{
		WorldMatrix = m_pDynamicColliderComs[i].lock()->Get_WorldMatrix();

		m_pPropInfos[i].vRotation = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(WorldMatrix));
		XMStoreFloat3(&m_pPropInfos[i].vTarnslation, WorldMatrix.r[3]);
	}

	m_pInstanceModelCom.lock()->Update(m_pPropInfos, false);
}



void CStatic_Instancing_Prop::Free()
{
}
