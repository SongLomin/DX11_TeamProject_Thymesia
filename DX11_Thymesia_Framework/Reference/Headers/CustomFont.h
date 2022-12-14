#pragma once
#include "Base.h"

BEGIN(Engine)

class CCustomFont final : public CBase
{
	GAMECLASS_H(CCustomFont)

	

public:
	void	Add_Text(const tstring& pString, const _float2& vPosition, _fvector vColor, _bool bAlways, _bool bCenterAlign = false);
	void	Add_Text(const TEXTINFO& In_tTextInfo);
	
	HRESULT Initialize(const _tchar* pFontFilePath, ComPtr<ID3D11DeviceContext> pDeviceContext);
	HRESULT Render(const _tchar* pString, const _float2& vPosition, _fvector vColor);
	void	Render(ID3D11DeviceContext* pDeviceContext);
	void	RenderWithRenderGroup(const RENDERGROUP In_eRenderGroup);



private:
	unique_ptr<SpriteBatch> m_pBatch;
	unique_ptr<SpriteFont>	m_pFont;

	list<TEXTINFO>			m_TextInfos;

	ComPtr<ID3D11DeviceContext> m_pDeviceContext;

public:
	static shared_ptr<CCustomFont> Create(const _tchar* pFontFilePath, ComPtr<ID3D11DeviceContext> pDeviceContext);
	void Free();
};

END