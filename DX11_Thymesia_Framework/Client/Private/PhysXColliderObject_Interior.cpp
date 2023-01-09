#include "stdafx.h"
#include "PhysXColliderObject_Interior.h"

#include "PhysXCollider.h"
#include "Client_Presets.h"
#include "VIBuffer_Cube.h"
#include "Renderer.h"
#include "Shader.h"

#include "SMath.h"

GAMECLASS_C(CPhysXColliderObject_Interior);
CLONE_C(CPhysXColliderObject_Interior, CGameObject);

HRESULT CPhysXColliderObject_Interior::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhysXColliderObject_Interior::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pPhysXColliderCom = Add_Component<CPhysXCollider>();
	m_pVIBufferCom		= Add_Component<CVIBuffer_Cube>();
	m_pShaderCom		= Add_Component<CShader>();
	m_pRendererCom		= Add_Component<CRenderer>();

	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxCubeTex"),
		VTXCUBETEX_DECLARATION::Element,
		VTXCUBETEX_DECLARATION::iNumElements
	);

	return S_OK;
}

HRESULT CPhysXColliderObject_Interior::Start()
{
	__super::Start();

	MESH_VTX_INFO tInfo;
	tInfo.vCenter	= _float3(0.f, 0.f, 0.f);
	tInfo.vMax		= _float3(0.5f, 0.5f, 0.5f);
	tInfo.vMin		= _float3(-0.5f, -0.5f, -0.5f);

	PHYSXCOLLIDERDESC tDesc;
	Preset::PhysXColliderDesc::StaticInteriorBoxDefaultSetting(tDesc, m_pTransformCom, tInfo, &m_fPhyxOffset);
	m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
	m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene();

	return S_OK;
}

void CPhysXColliderObject_Interior::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CPhysXColliderObject_Interior::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//if ((_uint)LEVEL_EDIT == m_CreatedLevel && m_bViewPhysXInfo)

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONLIGHT, Weak_StaticCast<CGameObject>(m_this));
}

HRESULT CPhysXColliderObject_Interior::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	if ((_uint)LEVEL_EDIT == m_CreatedLevel)
	{
		if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
			return E_FAIL;
		if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
			return E_FAIL;

		m_pShaderCom.lock()->Begin(3, pDeviceContext);
		m_pVIBufferCom.lock()->Render(pDeviceContext);
	}

	return S_OK;
}

_bool CPhysXColliderObject_Interior::IsPicking(const RAY& In_Ray, _float& Out_fRange)
{
	MESH_VTX_INFO tInfo;

	tInfo.vMax = {  0.5f,  0.5f,  0.5f };
	tInfo.vMin = { -0.5f, -0.5f, -0.5f };

	_float fDist;

	if (SMath::Is_Picked_AbstractCube(In_Ray, tInfo, m_pTransformCom.lock()->Get_WorldMatrix(), &fDist))
	{
		if (Out_fRange > fDist)
		{
			Out_fRange = fDist;
			return true;
		}
	}

	return false;
}

void CPhysXColliderObject_Interior::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);
}

void CPhysXColliderObject_Interior::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);
}

void CPhysXColliderObject_Interior::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);
	
	switch (EVENT_TYPE(iArg))
	{
		case EVENT_TYPE::ON_EDIT_PHYSXINFO: 
		{
			m_bViewPhysXInfo = true;
		}
		break;

		case EVENT_TYPE::ON_EDIT_PHYSXINFO_N:
		{
			m_bViewPhysXInfo = false;
		}
		break;

		case EVENT_TYPE::ON_EDITDRAW_NONE:
		{
			m_vColor = { 1.f, 0.f, 0.f, 1.f };
		}
		break;

		case EVENT_TYPE::ON_EDITDRAW_ACCEPT:
		{
			m_vColor = { 0.f, 1.f, 0.f, 1.f };
		}
		break;
	}
}

void CPhysXColliderObject_Interior::Free()
{
}