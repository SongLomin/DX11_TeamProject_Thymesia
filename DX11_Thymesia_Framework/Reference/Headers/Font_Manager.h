#pragma once
#include "Base.h"

BEGIN(Engine)

class CCustomFont;

class CFont_Manager final : public CBase
{
	DECLARE_SINGLETON(CFont_Manager)

public:
	void	Init_DeviceContext(ComPtr<ID3D11DeviceContext> pDeviceContext);

	HRESULT Add_Font(_uint iFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(_uint iFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor);
	HRESULT Render_Font(_uint iFontTag);
	HRESULT Render_AllFontWithRenderGroup(const RENDERGROUP In_eRenderGroup);
	HRESULT Render_AllFont();

	//void	Add_Text(const _uint& iFontTag, const tstring& pString, const _float2& vPosition, _fvector vColor, const _bool& bAlways, _bool bCenterAlign = false);
	void	Add_Text(const _uint& iFontTag, const TEXTINFO& In_tTextInfo);

private:
	map<_uint, shared_ptr<CCustomFont>>			m_Fonts;
	ComPtr<ID3D11DeviceContext>					m_pDeviceContext;

private:
	class shared_ptr<CCustomFont> Find_Font(_uint iFontTag);

public:
	void Free();
};

END