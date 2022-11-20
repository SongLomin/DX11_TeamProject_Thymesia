#pragma once
#include "UI.h"

BEGIN(Client)

class CProgressBar;
class CCustomUI;
class CHUD_Hover;

class CPlayer_HPBar final :	public CUI
{
	GAMECLASS_H(CPlayer_HPBar);
	CLONE_H(CPlayer_HPBar, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void OnEventMessage(_uint iArg) override;

public:
	void		Set_CurrentHp(_float	_fCurrentHp);
	void		Set_MaxHp(_float	_fMaxHp) { m_fMaxHp = _fMaxHp; }

private:
	weak_ptr< CProgressBar>	m_pMainBar;
	weak_ptr< CCustomUI>	m_pBG;
	weak_ptr< CCustomUI>	m_pBorderLeft;
	weak_ptr< CCustomUI>	m_pBorderRight;
	weak_ptr< CHUD_Hover>	m_pTrack;


private:
	_float			m_fLerpAcc;

private:
	_float			m_fMaxHp;
	_float			m_fCurrentHp;
	_float			m_fLerpHp;

private:
	TEXTINFO		m_tTextInfo;

public:
	void Free();

};

END

