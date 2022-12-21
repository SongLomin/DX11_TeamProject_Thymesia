#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CCustomUI;

class CUI_AppearEvent_Varg : public CUI_Container
{
	GAMECLASS_H(CUI_AppearEvent_Varg)
	CLONE_H(CUI_AppearEvent_Varg, CGameObject)
public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    
public:
	

private:
	void							Create_UI();

private:
	weak_ptr<CCustomUI>				m_pMainTitle;
	weak_ptr<CCustomUI>				m_pSubTitle;

private:
	void		Free();
};

END