#include "stdafx.h"
#include "Static_Instancing_Prop.h"

#include "Model.h"
#include "VIBuffer_Model_Instance.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "PhysXCollider.h"
#include "Client_Presets.h"
#include "Texture.h"

#include "ImGui_Manager.h"
#include "GameManager.h"

#ifdef _DEBUG
#include "Window_Optimization_Dev.h"
#endif // _DEBUG

#include "SMath.h"



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
	m_pInstanceModelCom  = Add_Component<CVIBuffer_Model_Instance>();
	m_pPhysXColliderCom  = Add_Component<CPhysXCollider>();

	m_pMaskingTextureCom.lock()->Use_Texture("UVMask");

	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));

    return S_OK;
}

HRESULT CStatic_Instancing_Prop::Start()
{
    __super::Start();

	m_pPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();

	for (auto& elem : m_pDynamicColliderComs)
	{
		elem.lock()->Add_PhysXActorAtScene();
	}

    return S_OK;
}

void CStatic_Instancing_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

	if (m_bDissolve)
		m_fDissolveRatio += fTimeDelta * m_fDissolveSpeed;
}

void CStatic_Instancing_Prop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CStatic_Instancing_Prop::Thread_PreLateTick(_float fTimeDelta)
{
#ifdef _INSTANCE_CULLING_
	ID3D11DeviceContext* pDeviceContext = GAMEINSTANCE->Get_BeforeRenderContext();

#ifdef _DEBUG
	if (m_pDynamicColliderComs.empty())
		m_pInstanceModelCom.lock()->Culling_Instance(std::ref(m_pPropInfos), GET_SINGLE(CWindow_Optimization_Dev)->Get_InstancingCullingRatio());
#else
	if (m_pDynamicColliderComs.empty())
		m_pInstanceModelCom.lock()->Culling_Instance(std::ref(m_pPropInfos), 0.5f);
#endif // _DEBUG
	

	if (m_pDynamicColliderComs.empty())
		m_pInstanceModelCom.lock()->Update_VisibleInstance(pDeviceContext);

	GAMEINSTANCE->Release_BeforeRenderContext(pDeviceContext);
#endif
}

void CStatic_Instancing_Prop::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);

	if(!m_pDynamicColliderComs.empty())
		Synchronize_DynamicColliderComs();

#ifdef _INSTANCE_CULLING_
#ifndef _USE_THREAD_
	// Don't Culling DynamicProps
	if (m_pDynamicColliderComs.empty())
		m_pInstanceModelCom.lock()->Update_VisibleInstance(DEVICECONTEXT);
#endif // _USE_THREAD_
#endif
}

HRESULT CStatic_Instancing_Prop::Render(ID3D11DeviceContext* pDeviceContext)
{
	if ("TEMP_Corvus" == m_pInstanceModelCom.lock()->Get_ModelKey())
		return S_OK;

	if ("Tent_P_Rag04" == m_pInstanceModelCom.lock()->Get_ModelKey())
		int a = 0;

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

	_float fCamFar = GAMEINSTANCE->Get_CameraFar();
	m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

	_vector vShaderFlag = { 0.f, 0.f, 0.f, 0.f };
	m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector));

	if (m_bDissolve)
	{
		for (auto& elem : m_pTextureGroupCom)
		{
			if (FAILED(elem.second.lock()->Set_ShaderResourceView
			(
				m_pShaderCom,
				elem.first.c_str(),
				0
			)))
				return E_FAIL;
		}

		m_pShaderCom.lock()->Set_RawValue("g_fDissolveRatio", &m_fDissolveRatio, sizeof(_float));
	}

	_uint iNumMeshContainers = m_pInstanceModelCom.lock()->Get_NumMeshContainers();

	_flag BindTextureFlag;

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		BindTextureFlag = 0;

		if (SUCCEEDED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
		}

		if (SUCCEEDED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			BindTextureFlag |= (1 << aiTextureType_NORMALS);
		}

		if (SUCCEEDED(m_pInstanceModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
		{
			BindTextureFlag |= (1 << aiTextureType_SPECULAR);
		}
		else
		{
			BEGIN_PERFROMANCE_CHECK(m_pInstanceModelCom.lock()->Get_ModelKey());
			END_PERFROMANCE_CHECK(m_pInstanceModelCom.lock()->Get_ModelKey());
		}

		m_iPassIndex = Preset::ShaderPass::ModelInstancingShaderPass(BindTextureFlag, m_bInvisibility, m_bNonCulling, m_bDissolve);

		m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
		m_pInstanceModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

    return S_OK;
}

HRESULT CStatic_Instancing_Prop::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext)
{
	m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

	_uint iNumMeshContainers = m_pInstanceModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		m_pShaderCom.lock()->Begin(2, pDeviceContext);
		m_pInstanceModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	return S_OK;
}

void CStatic_Instancing_Prop::Load_FromJson(const json& In_Json)
{
	m_bNonCulling    = In_Json["NonCulling"];
	m_bInvisibility  = In_Json["Invisibility"];
	m_iColliderType  = In_Json["Collider_Type"];
	m_fDissolveSpeed = In_Json["Dissolve"];

	if (In_Json.end() != In_Json.find("SectionIndex"))
	{
		m_iSectionIndex = In_Json["SectionIndex"];

		if (0 <= m_iSectionIndex)
			GET_SINGLE(CGameManager)->Registration_Section(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
	}

	if (0.f > m_fDissolveSpeed)
	{
		m_pTextureGroupCom.emplace("g_DissolveTexture"    , Add_Component<CTexture>());
		m_pTextureGroupCom.emplace("g_DissolveDiffTexture", Add_Component<CTexture>());

		m_pTextureGroupCom["g_DissolveTexture"].lock()->Use_Texture("T_Fire_Tile_BW_03");
		m_pTextureGroupCom["g_DissolveDiffTexture"].lock()->Use_Texture("Diff_Fire_Tile0");
	}

	string szModelTag = In_Json["ModelCom"];

	if ("" != szModelTag)
		m_pInstanceModelCom.lock()->Init_Model(szModelTag.c_str());

	_vector vPosition;
	_vector vOffsetRange;

	for (auto& iter_item : In_Json["PropDesc"].items())
	{
		_float4x4 PropMatrix;

		CJson_Utility::Load_JsonFloat4x4(iter_item.value(), PropMatrix);

		INSTANCE_MESH_DESC	Desc;
		ZeroMemory(&Desc, sizeof(INSTANCE_MESH_DESC));

		memcpy(&Desc.vRotation   , &PropMatrix.m[0][0], sizeof(_float3));
		memcpy(&Desc.vScale      , &PropMatrix.m[1][0], sizeof(_float3));
		memcpy(&Desc.vTarnslation, &PropMatrix.m[2][0], sizeof(_float3));

		vPosition = XMVectorSetW(XMLoadFloat3(&Desc.vTarnslation), 1.f);
				
		MESH_VTX_INFO tInfo = m_pInstanceModelCom.lock()->Get_ModelData().lock()->VertexInfo;

		_vector vMin, vMax, vCenter;
		vMin    = XMLoadFloat3(&tInfo.vMin);
		vMax    = XMLoadFloat3(&tInfo.vMax);
		vOffsetRange = (vMax - vMin) / 2.f;
		vCenter = vOffsetRange + vMin;
		
		vOffsetRange  *= XMLoadFloat3(&Desc.vScale);
		Desc.fMaxRange = XMVectorGetX(XMVector3Length(vOffsetRange));
		XMStoreFloat3(&Desc.vCenter, vCenter);
		Desc.Bake_CenterWithMatrix();

		m_pPropInfos.push_back(Desc);
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

void CStatic_Instancing_Prop::OnEventMessage(_uint iArg)
{
	switch ((EVENT_TYPE)iArg)
	{
		case EVENT_TYPE::ON_ENTER_SECTION:
		{
			m_bDissolve      = true;
			m_fDissolveRatio = 0.f;
		}
		break;

		case EVENT_TYPE::ON_EXIT_SECTION:
		{
			m_bDissolve      = false;
			m_fDissolveRatio = 0.f;
		}
		break;
	}
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
