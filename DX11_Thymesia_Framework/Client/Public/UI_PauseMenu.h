#pragma once
#include "UI.h"

BEGIN(Client)

class CCustomUI;


class CUI_PauseMenu : public CUI
{
public:
    enum PAUSE_MENU_TYPE{MENU_STATUS, MENU_TALENT, MENU_ITEM, MENU_COLLETION, MENU_OPTION, MENU_QUIT};

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
    //��ü�� ���°� ��Ȱ��ȭ ���·� ����� ��, ȣ��Ǵ� �̺�Ʈ�Դϴ�.
    virtual void OnDisable() override;


private:
    weak_ptr<CCustomUI> m_pPauseMenuBackground;
    weak_ptr<CCustomUI> m_pPauseMenuBackground_Main;
    weak_ptr<CCustomUI> m_pPauseMenuBackground_Top;

    
    
private:
    _uint       m_iPageIndex;
    _bool       m_bOpenThisFrame;
};

END

