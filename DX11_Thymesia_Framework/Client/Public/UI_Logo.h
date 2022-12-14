#pragma once
#include "UI.h"


BEGIN(Client)

class CCustomUI;
class CHUD_Hover;


class CUI_Logo : public CUI
{
public:
	enum LOGO_BUTTON_TYPE {
		LOGO_BUTTON_START_GAME = 0, 
		LOGO_BUTTON_CONTINUE_GAME, 
		LOGO_BUTTON_LOAD_GAME,
		LOGO_BUTTON_TOOL,
		LOGO_BUTTON_OPTION, 
		LOGO_BUTTON_CREDITS, 
		LOGO_BUTTON_QUIT,
		LOGO_BUTTON_END};


public:
	GAMECLASS_H(CUI_Logo)
	CLONE_H(CUI_Logo, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

protected:
	virtual HRESULT SetUp_ShaderResource();

private:
	void		ChageButtonIndex(_uint iButtonIndex);
	void		SelectButton(_uint iButtonIndex);

private:
	weak_ptr<CHUD_Hover>	m_pLogo;
	weak_ptr<CCustomUI>		m_pMainMenuBG;
	weak_ptr<CHUD_Hover>	m_pSelectButton[LOGO_BUTTON_END];
	weak_ptr<CHUD_Hover>	m_pTextTexture[LOGO_BUTTON_END];


	string					m_fontPath[LOGO_BUTTON_END];
	_float2					m_fFontSize[LOGO_BUTTON_END];

	_bool		m_bSelect;
	_uint		m_iSelect;

public:
	void Free();




};

END
