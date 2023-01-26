#include "stdafx.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "EasingComponent_Alpha.h"


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
		TEXT("Shader_UI"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);

	m_tUIDesc.fDepth = 0.0f;
	m_iDeffuseIndex = 0;
	m_fAlphaColor = 1.f;

	m_iPassIndex = 0;
	if (pArg != nullptr)
		memcpy(&m_tUIDesc, pArg, sizeof(UI_DESC));

	m_eRenderGroup = RENDERGROUP::RENDER_UI;
	Set_Texture("None");
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

HRESULT CCustomUI::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	return S_OK;
}

void CCustomUI::SetUp_Component()
{

}

void CCustomUI::Set_PassIndex(_uint _iPassIndex)
{
	m_iPassIndex = _iPassIndex;
}

void CCustomUI::Set_DeffuseIndex(_uint _iDeffuseIndex)
{
	m_iDeffuseIndex = _iDeffuseIndex;
}

HRESULT CCustomUI::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	//_float fTotalColor = m_fAlphaColor * m_fOwnerAlphaColor;

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_fAlphaColor", &m_fAlphaColor, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture",m_iDeffuseIndex)))
	{
		tstring strErrorText = TEXT("CCustomUI  \n  Errror Textre Key : ");
		
		string	strTextureKey = m_pTextureCom.lock()->Get_TextureKey();
		wstring	wstrTextureKey;

		wstrTextureKey.assign(strTextureKey.begin(), strTextureKey.end());

		strErrorText += wstrTextureKey;

		MessageBox(0, strErrorText.c_str(), TEXT("System Error"), MB_OK);
	}

	return S_OK;
}

void CCustomUI::Free()
{
	__super::Free();

}
