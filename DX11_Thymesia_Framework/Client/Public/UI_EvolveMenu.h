#pragma once
#include "UI_Container.h"


BEGIN(Engine)

ENGINE_DECLATION_UI;

END

BEGIN(Client)

CLIENT_DECLATION_UI

class CUI_EvolveMenu final : public CUI_Container
{

public:
    enum class EVOLVEMENU_TYPE {
        EVOLVE_LEVELUP = 0,
        EVOLVE_UNLOCKTALENT, 
        EVOLVE_PLAGUEWEAPON,
        EVOLVE_POTION,
        EVOLVE_FEATHER,
        EVOLVE_CEASE_RECALL,
        EVOLVE_RESUME_GAME,
        EVOLVE_END
    };
public:
    GAMECLASS_H(CUI_EvolveMenu)
    CLONE_H(CUI_EvolveMenu, CGameObject)

public:
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;

    virtual void OnEnable(void* _Arg = nullptr);



private: //Left UI

    weak_ptr<CCustomUI> m_pLeftBG;
    weak_ptr<CCustomUI> m_pSelectHighlight;

    weak_ptr<CCustomUI> m_pMenuTitleBG;
    weak_ptr<CCustomUI> m_pMenuTitleText;

    weak_ptr<CCustomUI>  m_pMenuText[(_uint)EVOLVEMENU_TYPE::EVOLVE_END];

    _bool               m_bOpenedPage[(_uint)EVOLVEMENU_TYPE::EVOLVE_END];
    _uint               m_iSelectedIndex;
    _char              m_MenuTextKey[(_uint)EVOLVEMENU_TYPE::EVOLVE_END][MAX_PATH];



private://Right UI
    weak_ptr<CCustomUI> m_pRightBG;
    weak_ptr<CCustomUI> m_pRightTitle;
    weak_ptr<CCustomUI> m_pRightTitleDecoration;
    weak_ptr<CCustomUI> m_pRightMapImage;
    

private:
   LEVEL      m_eLastOpenedLevel;
   _bool               m_bEnabledThisFrame;

private:
    void               ChangeButtonIndex();
    void               SelectButton();
    void               ChangeUIFromCurrentLevel();
    

private:
    virtual void Free() override;

};

END