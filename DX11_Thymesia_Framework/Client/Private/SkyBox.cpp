#include "stdafx.h"
#include "SkyBox.h"
#include "VIBuffer_Cube.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "GameInstance.h"

GAMECLASS_C(CSkyBox)
CLONE_C(CSkyBox, CGameObject)

HRESULT CSkyBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkyBox::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pRendererCom = Add_Component<CRenderer>();
	m_pShaderCom = Add_Component<CShader>();
	m_pTextureCom = Add_Component<CTexture>();
	m_pVIBufferCom = Add_Component<CVIBuffer_Cube>();

	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxCubeTex"), 
		VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements);
	m_pTextureCom.lock()->Use_Texture("Sky");


	return S_OK;
}

HRESULT CSkyBox::Start()
{
	return S_OK;
}

void CSkyBox::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkyBox::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_fUVFlow -= fTimeDelta * 0.015f;

	m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fUVFlow);

	m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_PRIORITY, 
		Weak_StaticCast<CGameObject>(m_this));
}

HRESULT CSkyBox::Render(ID3D11DeviceContext* pDeviceContext)
{
	_float4 vCamPosition = GAMEINSTANCE->Get_CamPosition();

	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&vCamPosition));
	m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
	m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0);

	m_pShaderCom.lock()->Begin(0, pDeviceContext);

	__super::Render(pDeviceContext);

	m_pVIBufferCom.lock()->Render(pDeviceContext);

	return S_OK;
}

void CSkyBox::Free()
{
}
