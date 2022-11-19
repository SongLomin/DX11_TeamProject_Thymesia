#pragma once
#include "UI.h"



BEGIN(Client)

class CCustomUI;
class CProgressBar;
class CHUD_Hover;

class CMonsterHPBar_Base : public CUI
{
public:
    GAMECLASS_H(CMonsterHPBar_Base)
    CLONE_H(CMonsterHPBar_Base, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


public:
	void			Green_Damaged(_float fRatio);

	void			Toggle_Recovery();

protected:
	virtual void    Add_Child(weak_ptr<CUI> pChild);
	virtual void	Check_Track();

protected:
    weak_ptr<CCustomUI> m_pBorder; // 0.6f;
	weak_ptr<CCustomUI> m_pDecoration_Head; // 0.0f;

	weak_ptr<CProgressBar>	m_pWhite;
	weak_ptr<CProgressBar>	m_pGreen;
	weak_ptr<CHUD_Hover>	m_pGreenTrack;

	weak_ptr<CCustomUI>		m_pTrack;
	weak_ptr<CHUD_Hover>	m_pRecovery;

public:
	void		Call_GreenTrackFadeEnd(FADER_TYPE eType);

	
};

END

