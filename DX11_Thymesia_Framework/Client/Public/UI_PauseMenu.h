#pragma once
#include "UI.h"

BEGIN(Client)

class CCustomUI;


class CUI_PauseMenu : public CUI
{
public:
    enum PAUSE_MENU_TYPE{PAUSE_MENU_STATUS, PAUSE_MENU_TALENT, PAUSE_MENU_ITEM, PAUSE_MENU_COLLETION, PAUSE_MENU_OPTION, PAUSE_MENU_QUIT, PAUSE_MENU_END};

public:
	GAMECLASS_H(CUI_PauseMenu)
	CLONE_H(CUI_PauseMenu, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    virtual void OnEnable(void* _Arg = nullptr) override;
    //객체의 상태가 비활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void OnDisable() override;


private:
    void                OnPaging();


private:
    weak_ptr<CCustomUI> m_pPauseMenuBackground;
    weak_ptr<CCustomUI> m_pPauseMenuBackground_Main;
    weak_ptr<CCustomUI> m_pPauseMenuBackground_Top;

private:
    weak_ptr<CCustomUI> m_pPageIndicator[PAUSE_MENU_END];

    weak_ptr<CCustomUI> m_pPageIndicatorDecoration;

    weak_ptr<CCustomUI> m_pPageTitleUnderLine;


    
    
    
    //PauseMenu Text
private:
    wstring         m_szPageTextData[PAUSE_MENU_END];
    TEXTINFO        m_PageTexInfo[PAUSE_MENU_END];

private:
    _uint       m_iPageIndex;
    _bool       m_bOpenThisFrame;


private:
    void    Create_Background();
    void    Create_PageText();
    void    Create_PageIndicator();




};

END

