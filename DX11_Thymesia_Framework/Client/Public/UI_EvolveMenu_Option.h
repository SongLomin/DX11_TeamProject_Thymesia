#pragma once
#include "UI_EvolveMenu_Page.h"

BEGIN(Client)

class CCustomUI;
class CUI_Scroll;

class CEasingComponent_Alpha;

class CUI_OptionItem;


class CUI_EvolveMenu_Option : public CUI_EvolveMenu_Page
{
public:
	GAMECLASS_H(CUI_EvolveMenu_Option)
	CLONE_H(CUI_EvolveMenu_Option, CGameObject)

public:
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    FDelegate<OPTION_TYPE, _uint>       Callback_OnChangeOption;
    void                                Call_OnChangeOption(OPTION_TYPE eOption, _uint   iAmount);

    void                                Call_OnWheelMove(_float fScrollOffsetY);

private:
    void                    Init_UI();
    void                    Init_OptionItem();

private:
    virtual void            OnEnable(void* pArg);
    virtual void            OnDisable();

private:
    weak_ptr<CCustomUI>     m_pBackground_Head;
    weak_ptr<CCustomUI>     m_pBackground_Body;


    weak_ptr<CUI_Scroll>    m_pScroll;
    weak_ptr<CEasingComponent_Alpha>    m_pEasingAlpha;

    vector <weak_ptr<CUI_OptionItem>> m_vecOptionItem;




private:
    _float                  m_fOptionItemOffsetY;


private:
	void		Free();
};

END