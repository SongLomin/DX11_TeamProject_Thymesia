#pragma once
#include "UI.h"

BEGIN(Client)

class CCustomUI;

class CFadeMask;

class CUI_PauseMenu : public CUI
{
public:
    enum PAUSE_MENU_TYPE{PAUSE_MENU_STATUS, PAUSE_MENU_ITEM, PAUSE_MENU_END};

public:
	GAMECLASS_H(CUI_PauseMenu)
	CLONE_H(CUI_PauseMenu, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    virtual void OnEnable(void* _Arg = nullptr) override;
    //객체의 상태가 비활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void OnDisable() override;


private:
    void                OnPaging();
    void                Update_KeyInput();

private:
    weak_ptr<CCustomUI> m_pPauseMenuBackground;
    weak_ptr<CCustomUI> m_pPauseMenuBackground_MainFrame;
    weak_ptr<CCustomUI> m_pPauseMenuBackground_Main;
    weak_ptr<CCustomUI> m_pPauseMenuBackground_Top;

//OnlyInventory
private:
    weak_ptr<CCustomUI> m_pInventoryBG;


private:
    weak_ptr<CCustomUI> m_pPageIndicator[PAUSE_MENU_END];

    weak_ptr<CCustomUI> m_pPageIndicatorDecoration;

    weak_ptr<CCustomUI> m_pPageTitleUnderLine;


    weak_ptr<CCustomUI> m_pFonts[PAUSE_MENU_END];


private:
    weak_ptr<CUI>       m_pPages[PAUSE_MENU_END];
    
    
    //PauseMenu Text
private:
    wstring         m_szPageTextData[PAUSE_MENU_END];
    TEXTINFO        m_PageTexInfo[PAUSE_MENU_END];

private:
    _uint       m_iPageIndex;
    _bool       m_bOpenThisFrame;
    _bool       m_bFonts;

private:
    weak_ptr<CFadeMask>    m_pFadeMask;


public:
    void            Call_FadeInPauseMenu();

private:
    void    Create_Background();
    void    Create_Pages();
    void    Create_PageText();
    void    Create_PageIndicator();




};

END

