#pragma once
#include "UI.h"

BEGIN(Client)

class CPlayer_ProgressBar;

class CPlayer_MPBar final : public CUI
{
	GAMECLASS_H(CPlayer_MPBar);
	CLONE_H(CPlayer_MPBar, CGameObject);

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
	void		Set_CurrentMp(_float	_fCurrentMp);

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:
	weak_ptr< CPlayer_ProgressBar>	m_pMainBar;
	weak_ptr< CPlayer_ProgressBar>	m_pBG;
	weak_ptr< CPlayer_ProgressBar>	m_pBorderLeft;
	weak_ptr< CPlayer_ProgressBar>	m_pBorderRight;
	weak_ptr< CPlayer_ProgressBar>	m_pTrack;

private:
	_float			m_fMaxMp;
	_float			m_fCurrentMp;

private:
	TEXTINFO		m_tTextInfo;

public:
	void Free();

};

END

