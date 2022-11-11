#include "stdafx.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"

GAMECLASS_C(CCustomUI)
CLONE_C(CCustomUI, CGameObject)

HRESULT CCustomUI::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CCustomUI::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxTex"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);

	m_tUIDesc.fDepth = 0.0f;
	m_iPassIndex = 4;
	if (pArg != nullptr)
		memcpy(&m_tUIDesc, pArg, sizeof(UI_DESC));

	m_eRenderGroup = RENDERGROUP::RENDER_UI;

	return S_OK;
}

HRESULT CCustomUI::Start()
{
	__super::Start();

	return S_OK;
}

void CCustomUI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCustomUI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CCustomUI::Render()
{
	__super::Render();

	return S_OK;
}

void CCustomUI::Set_PassIndex(_uint _iPassIndex)
{
	m_iPassIndex = _iPassIndex;
}

HRESULT CCustomUI::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture")))
	{
		MSG_BOX("CustomUI Error");
	}

	return S_OK;
}

void CCustomUI::Free()
{
	__super::Free();

}
