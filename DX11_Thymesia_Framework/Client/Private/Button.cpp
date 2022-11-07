#include "stdafx.h"
#include "Texture.h"
#include "Shader.h"
#include "Fader.h"

GAMECLASS_C(CButton)
CLONE_C(CButton, CGameObject)

HRESULT CButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxTex"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);

	m_pTextureCom.lock()->Use_Texture(("ButtonDefault"));
	m_iPassIndex = 2;


	return S_OK;
}

HRESULT CButton::Start()
{
	__super::Start();

	return S_OK;
}

void CButton::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	//m_pTransformCom.lock()->Set_Scaled(_float3(100.f, 100.f, 100.f));
}

void CButton::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CButton::Render()
{
	__super::Render();
	


	return S_OK;
}

HRESULT CButton::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	_float FaderAhlpaScale = m_pFaderCom.lock()->Get_FadeColor().w;
	m_pShaderCom.lock()->Set_RawValue("g_MaskAhlpaScale", &FaderAhlpaScale, sizeof(_float));

	return S_OK;
}

void CButton::Free()
{
}

