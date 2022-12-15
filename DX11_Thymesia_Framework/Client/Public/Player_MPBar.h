#pragma once
#include "UI.h"
#include "UI_LerpBar.h"

BEGIN(Client)

class CProgressBar;
class CEasingComponent_Float;


class CPlayer_MPBar final : public CUI
{
	friend class CPreset_UIDesc;
public:
	GAMECLASS_H(CPlayer_MPBar);
	CLONE_H(CPlayer_MPBar, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	virtual void OnEventMessage(_uint iArg) override;

	void		ChangeMP(_float fCurrentMP);



protected:
	virtual void	SetUp_Component() override;
	virtual HRESULT SetUp_ShaderResource() override;
	

public:
	virtual void	Bind_Player() override;
	virtual void	Update_MPBar();

private:
	weak_ptr< CProgressBar>	m_pMainBar;
	weak_ptr< CProgressBar>	m_pBG;
	weak_ptr< CProgressBar>	m_pBorderLeft;
	weak_ptr< CProgressBar>	m_pBorderRight;

private:
	weak_ptr<CEasingComponent_Float> m_pEasingFloatCom;


public:
	void			Call_UpdateStatus();
	void			Call_ChangeMP(_float	fCurrentMp);


private:
	_float			m_fMaxMp;

	_float			m_fLerpedMp;
	_float			m_fCurrentMp;


private:
	TEXTINFO		m_tTextInfo;

public:
	void Free();

};

END

