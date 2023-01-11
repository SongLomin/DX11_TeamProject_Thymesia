#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CFadeMask;

class CUI_EvolveMenu_Page : public CUI_Container
{
public:
	GAMECLASS_H(CUI_EvolveMenu_Page)
	CLONE_H(CUI_EvolveMenu_Page, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;

protected:
	virtual	void	Update_KeyInput(_float fTimeDelta);
	void			Call_ExitTap();


protected:
	weak_ptr<CFadeMask>	m_pFadeMask;

protected:
	void			OnEnable(void* pArg);
	void			OnDisable();
private:
	void			Free() { };

};

END