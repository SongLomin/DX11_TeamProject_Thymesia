#include "Font_Manager.h"
#include "CustomFont.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CFont_Manager)

void CFont_Manager::Init_DeviceContext(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	m_pDeviceContext = DEVICECONTEXT;
}

HRESULT CFont_Manager::Add_Font(_uint iFontTag, const _tchar* pFontFilePath)
{
	if (nullptr != Find_Font(iFontTag))
		return E_FAIL;

	shared_ptr<CCustomFont> pFont = CCustomFont::Create(pFontFilePath, m_pDeviceContext);
	if (nullptr == pFont)
		return E_FAIL;

	m_Fonts.emplace(iFontTag, pFont);

	return S_OK;
}

HRESULT CFont_Manager::Render_Font(_uint iFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor)
{
	shared_ptr<CCustomFont> pFont = Find_Font(iFontTag);
	if (nullptr == pFont)
		return E_FAIL;

	return pFont->Render(pString, vPosition, vColor);
}

HRESULT CFont_Manager::Render_Font(_uint iFontTag)
{
	shared_ptr<CCustomFont> pFont = Find_Font(iFontTag);
	if (nullptr == pFont)
		return E_FAIL;

	pFont->Render(DEVICECONTEXT);

	return S_OK;
}

HRESULT CFont_Manager::Render_AllFontWithRenderGroup(const RENDERGROUP In_eRenderGroup)
{
	for (auto& elem : m_Fonts)
	{
		elem.second->RenderWithRenderGroup(In_eRenderGroup);
	}

	return S_OK;
}

HRESULT CFont_Manager::Render_AllFont()
{
	for (auto& elem : m_Fonts)
	{
		elem.second->Render(DEVICECONTEXT);
	}


	return S_OK;
}

//void CFont_Manager::Add_Text(const _uint& iFontTag, const tstring& pString, const _float2& vPosition, _fvector vColor, const _bool& bAlways, _bool bCenterAlign)
//{
//	shared_ptr<CCustomFont> pFont = Find_Font(iFontTag);
//	if (!pFont)
//		return;
//
//	pFont->Add_Text(pString, vPosition, vColor, bAlways, bCenterAlign);
//}

void CFont_Manager::Add_Text(const _uint& iFontTag, const TEXTINFO& In_tTextInfo)
{
	shared_ptr<CCustomFont> pFont = Find_Font(iFontTag);
	if (!pFont)
		return;

	pFont->Add_Text(In_tTextInfo);
}

shared_ptr<CCustomFont> CFont_Manager::Find_Font(_uint iFontTag)
{
	auto	iter = m_Fonts.find(iFontTag);

	if (iter == m_Fonts.end())
		return nullptr;

	return iter->second;
}

void CFont_Manager::Free()
{
}