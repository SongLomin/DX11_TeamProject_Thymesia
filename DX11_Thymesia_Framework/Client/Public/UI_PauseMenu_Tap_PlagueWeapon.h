#pragma once
#include "UI.h"

BEGIN(Client)

class CCustomUI;
class CUI_PlagueWeapon;

class CUI_PauseMenu_Tap_PlagueWeapon : public CUI
{
public:
    GAMECLASS_H(CUI_PauseMenu_Tap_PlagueWeapon)
        CLONE_H(CUI_PauseMenu_Tap_PlagueWeapon, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    weak_ptr< CUI_PlagueWeapon> m_PlagueWeaponMain;
    weak_ptr< CUI_PlagueWeapon> m_PlagueWeaponSub;
    weak_ptr< CUI_PlagueWeapon> m_PlagueWeaponSteal;
    weak_ptr<CCustomUI> m_PlagueWeaponSteal_Decoration;

    TEXTINFO            m_PlagueWeaponText;


private:
    



};

END