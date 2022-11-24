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
    enum class EVOLOVEMENU_TYPE { 
        EVOLVE_LEVELUP = 0,
        EVOLVE_UNLOCKTALENT, 
        EVOLVE_PLAGUEWEAPON,
        EVOLVE_POTION,
        EVOLVE_FEATHER,
        EVOLVE_END
    };
public:
    GAMECLASS_H(CUI_EvolveMenu)
    CLONE_H(CUI_EvolveMenu, CGameObject)

public:
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;

private://Child 
    weak_ptr<CCustomUI> m_pLeftBG;
    weak_ptr<CCustomUI> m_pSelectHighlight;



    weak_ptr<CCustomUI>  m_pMenuText[(_uint)EVOLOVEMENU_TYPE::EVOLVE_END];

    _bool               m_bOpenedPage[(_uint)EVOLOVEMENU_TYPE::EVOLVE_END];
    _uint               m_iSelectedIndex;


    _char              m_MenuTextKey[(_uint)EVOLOVEMENU_TYPE::EVOLVE_END][MAX_PATH];

private:
    void               ChangeButtonIndex();

private:
    virtual void Free() override;

};

END