#pragma once
#include "UI.h"

BEGIN(Client)
class CCustomUI;
class CProgressBar;
class CHUD_Hover;

class CPlayer_FeatherUI :
	public CUI
{
	GAMECLASS_H(CPlayer_FeatherUI);
	CLONE_H(CPlayer_FeatherUI, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);


public:
	_uint			Get_CrrrentFeather();

public:
	void			Set_MaxFeather(_uint _iMaxFeather);
	void			Set_CurrentFeather(_uint _iCurrentFeather);

private:
	_uint			m_iMaxFeather;
	_uint			m_iCurrentFeather;

private:
	weak_ptr<CCustomUI> m_pFrame;
	weak_ptr<CProgressBar>	m_pFrameBorder;
	weak_ptr<CCustomUI> m_pIcon;
	weak_ptr<CCustomUI> m_pFontBG;
	weak_ptr<CHUD_Hover> m_pHover;

private:
	Engine::FaderDesc m_tFaderDesc; //For HUD_Hover;
	
private:
	_float				m_fRatio;
	
private:
	TEXTINFO		m_tCurrentFeatherTextInfo;
	TEXTINFO		m_tMaxFeatherTextInfo;


public:
	void			Call_FadeEnd(FADER_TYPE eFaderType);

};
END
