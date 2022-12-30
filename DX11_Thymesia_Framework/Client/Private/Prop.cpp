#include "stdafx.h"
#include "Prop.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameManager.h"
#include "Texture.h"

#include "imgui.h"

GAMECLASS_C(CProp);
CLONE_C(CProp, CGameObject);

HRESULT CProp::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CProp::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pModelCom          = Add_Component<CModel>();
    m_pShaderCom         = Add_Component<CShader>();
    m_pRendererCom       = Add_Component<CRenderer>();
	m_pMaskingTextureCom = Add_Component<CTexture>();

	m_pMaskingTextureCom.lock()->Use_Texture("UVMask");

#ifdef _USE_THREAD_
	Use_Thread(THREAD_TYPE::PRE_LATETICK);
#endif

    return S_OK;
}

HRESULT CProp::Start()
{
	weak_ptr<MODEL_DATA> pModelData = m_pModelCom.lock()->Get_ModelData();
	
	if (pModelData.lock())
	{
		m_vCenterOffset       = pModelData.lock()->VertexInfo.vCenter;
		m_fCullingOffsetRange = pModelData.lock()->Get_MaxOffsetRange() * 1.2f;
	}

    return S_OK;
}

void CProp::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CProp::Thread_PreLateTick(_float fTimeDelta)
{
	_vector vCenterOffsetToVector = XMLoadFloat3(&m_vCenterOffset);

	if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pTransformCom.lock()->Get_Position() + vCenterOffsetToVector, m_fCullingOffsetRange))
	{
		m_bRendering = true;
	}
	else
	{
		m_bRendering = false;
	}
}

void CProp::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

#ifdef _USE_THREAD_
	if (m_bRendering)
	{
		m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Weak_StaticCast<CGameObject>(m_this));
	}
#else
	//if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pTransformCom.lock()->Get_Position() + vCenterOffsetToVector, m_fCullingOffsetRange))
	//{
	//	if (RENDERGROUP::RENDER_END != m_eRenderGroup)
	//		m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Weak_StaticCast<CGameObject>(m_this));
	//}
	m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Weak_StaticCast<CGameObject>(m_this));
#endif // !_USE_THREAD_

}

HRESULT CProp::Render(ID3D11DeviceContext* pDeviceContext)
{
	return __super::Render(pDeviceContext);
}

void CProp::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	Out_Json["Invisibility"] = m_bInvisibility;
}

void CProp::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	if (In_Json.find("Invisibility") != In_Json.end())
		m_bInvisibility = In_Json["Invisibility"];
}

HRESULT CProp::SetUp_ShaderResource()
{
	if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_float4 vCamDesc;
	XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3]);
	m_pShaderCom.lock()->Set_RawValue("g_vCamPosition", &vCamDesc, sizeof(_float4));

	XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2]);
	m_pShaderCom.lock()->Set_RawValue("g_vCamLook", &vCamDesc, sizeof(_float4));
	
	_float4 vPlayerPos;
	XMStoreFloat4(&vPlayerPos,GET_SINGLE(CGameManager)->Get_PlayerPos());
	m_pShaderCom.lock()->Set_RawValue("g_vPlayerPosition", &vPlayerPos, sizeof(_float4));

	if (FAILED(m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 92)))
		return E_FAIL;

	_vector	vShaderFlag = { 0.f,0.f,0.f,0.f };
	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
		return E_FAIL;
	
	_float fCamFar= GAMEINSTANCE->Get_CameraFar();
	m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

	return S_OK;
}

void CProp::SetUp_Invisibility()
{
	ImGui::Checkbox("Invisibility", &m_bInvisibility);
	ImGui::Text("");
	ImGui::Separator();
}

void CProp::Free()
{
	__super::Free();
}
