#include "stdafx.h"
#include "PhysXColliderObject.h"
#include "PhysXCollider.h"
#include "Client_Presets.h"
#include "VIBuffer_DynamicCube.h"
#include "Renderer.h"
#include "Shader.h"

GAMECLASS_C(CPhysXColliderObject);
CLONE_C(CPhysXColliderObject, CGameObject);

HRESULT CPhysXColliderObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhysXColliderObject::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if ((_uint)LEVEL_EDIT != m_CreatedLevel)
	{
		m_pPhysXColliderCom = Add_Component<CPhysXCollider>();
	}
	else
	{
		m_pVIBufferCom = Add_Component<CVIBuffer_DynamicCube>();
		m_pShaderCom = Add_Component<CShader>();
		m_pShaderCom.lock()->Set_ShaderInfo
		(
			TEXT("Shader_VtxCubeTex"),
			VTXCUBETEX_DECLARATION::Element,
			VTXCUBETEX_DECLARATION::iNumElements
		);
		m_pRendererCom = Add_Component<CRenderer>();
	}
	

	return S_OK;
}

HRESULT CPhysXColliderObject::Start()
{
	__super::Start();

	return S_OK;
}

void CPhysXColliderObject::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if ((_uint)LEVEL_EDIT == m_CreatedLevel)
	{
		MESH_VTX_INFO VtxInfo;
		VtxInfo.vMax = { 1.f, 1.f,1.f };
		VtxInfo.vMin = { 0.f, 0.f, 0.f };

		//m_pVIBufferCom.lock()->Update(VtxInfo, m_pTransformCom.lock()->Get_WorldMatrix());
	}
	
}

void CPhysXColliderObject::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if ((_uint)LEVEL_EDIT == m_CreatedLevel && m_bViewPhysXInfo)
	{
		m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONLIGHT, Weak_StaticCast<CGameObject>(m_this));
	}
}

HRESULT CPhysXColliderObject::Render()
{
	__super::Render();

	if ((_uint)LEVEL_EDIT == m_CreatedLevel)
	{
		if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
			DEBUG_ASSERT;
		if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW)), sizeof(_float4x4))))
			DEBUG_ASSERT;
		if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ)), sizeof(_float4x4))))
			DEBUG_ASSERT;

		m_pShaderCom.lock()->Begin(2);
		m_pVIBufferCom.lock()->Render();
	}

	return S_OK;
}

void CPhysXColliderObject::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

}

void CPhysXColliderObject::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	if ((_uint)LEVEL_EDIT != m_CreatedLevel)
	{
		PHYSXCOLLIDERDESC tDesc;
		Preset::PhysXColliderDesc::StaticBoxDefaultSetting(tDesc, m_pTransformCom);
		m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
		m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene();
	}

}

void CPhysXColliderObject::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);
	
	if ((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO == iArg)
	{
		m_bViewPhysXInfo = true;
	}
	else if ((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO_N == iArg	)
	{
		m_bViewPhysXInfo = false;
	}
}

void CPhysXColliderObject::Free()
{
}