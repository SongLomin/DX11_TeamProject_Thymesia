#pragma once
#include "UI_Container.h"

BEGIN(Client)

CLIENT_DECLATION_UI

class CFadeMask;

class CUI_EvolveMenu_Level : public CUI_Container
{
public:
	GAMECLASS_H(CUI_EvolveMenu_Level)
	CLONE_H(CUI_EvolveMenu_Level, CGameObject)


public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);



private:
	void    Create_Background();

private:
	weak_ptr<CCustomUI> m_pPauseMenuBackground;
	weak_ptr<CCustomUI> m_pPauseMenuBackground_Main;
	weak_ptr<CCustomUI> m_pPauseMenuBackground_Top;


private:
	virtual void	OnEnable(void* pArg) override;


private:
	weak_ptr<CFadeMask>	m_pFadeMask;


public:
	void				Call_ReturnToEvolveMenu();
};

END
