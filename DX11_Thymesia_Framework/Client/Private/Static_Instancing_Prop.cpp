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
	//m_pInstanceModelCom.lock()->Culling_Instance(m_pPropInfos);


}

void CStatic_Instancing_Prop::Custom_Thread1(_float fTimeDelta)
{

}

void CStatic_Instancing_Prop::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);

#ifdef _INSTANCE_CULLING_
	m_pInstanceModelCom.lock()->Culling_Instance(std::ref(m_pPropInfos));
#endif

#ifdef _INSTANCE_CULLING_
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

	_vector vLightFlag = { 1.f, 1.f, 1.f, 1.f };
	m_pShaderCom.lock()->Set_RawValue("g_vLightFlag", &vLightFlag, sizeof(_vector));

	_uint iNumMeshContainers = m_pInstanceModelCom.lock()->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			DEBUG_ASSERT;
		}

		if (FAILED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			m_iPassIndex = 0;
		}
		else
		{
			m_iPassIndex = 1;
		}

		if (m_bEdit && m_iColliderType != 0)
		{
			m_iPassIndex = 3;
		}

		m_pShaderCom.lock()->Begin(m_iPassIndex);
		m_pInstanceModelCom.lock()->Render_Mesh(i);
	}

	// 피킹 오브젝트

	CGameObject::Render();

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
}

void CStatic_Instancing_Prop::Load_FromJson(const json& In_Json)
{
	for (auto& iter : In_Json.items())
	{
		string szKey = iter.key();

		if ("ModelCom" == szKey)
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

				memcpy(&Desc.vRotation, &PropMatrix.m[0][0], sizeof(_float3));
				memcpy(&Desc.vScale, &PropMatrix.m[1][0], sizeof(_float3));
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
	if ((_uint)LEVEL_GAMEPLAY == m_CreatedLevel && strcmp(m_pInstanceModelCom.lock()->Get_ModelKey().c_str(), "SM_Weed"))
	{
#ifdef _DEBUG_COUT_
		cout << "Create_PhysX: " << m_pInstanceModelCom.lock()->Get_ModelKey() << endl;
#endif // _DEBUG_COUT_

		// _bool bConvex = m_iColliderType == 2;

		m_pPhysXColliderCom.lock()->Init_ModelInstanceCollider(m_pInstanceModelCom.lock()->Get_ModelData(), m_pPropInfos, _GENERATE_PROP_COLLIDER_);
		PhysXColliderDesc tDesc;
		Preset::PhysXColliderDesc::StaticInstancingPropSetting(tDesc, m_pTransformCom);
		m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
		m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene();
	}
#endif // _GENERATE_PROP_COLLIDER_

	
}



void CStatic_Instancing_Prop::Free()
{
}
