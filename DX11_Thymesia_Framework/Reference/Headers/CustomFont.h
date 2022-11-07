#pragma once
#include "Base.h"

BEGIN(Engine)

class CCustomFont final : public CBase
{
	GAMECLASS_H(CCustomFont)

	

public:
	void	Add_Text(const tstring& pString, const _float2& vPosition, _fvector vColor, _bool bAlways, _bool bCenterAlign = false);
	void	Add_Text(const TEXTINFO& In_tTextInfo);
	
	HRESULT Initialize(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pString, const _float2& vPosition, _fvector vColor);
	void	Render();



private:
	unique_ptr<SpriteBatch> m_pBatch;
	unique_ptr<SpriteFont>	m_pFont;

	list<TEXTINFO>			m_TextInfos;

public:
	static shared_ptr<CCustomFont> Create(const _tchar* pFontFilePath);
	virtual void Free() override;
};

END