#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_PauseMenu_Tap_Status;
class CUI_PauseMenu_Tap_PlagueWeapon;
class CUI_PauseMenu_Tap_Potion;

class CUI_PauseMenu_Page_Status : public CUI
{
    GAMECLASS_H(CUI_PauseMenu_Page_Status)
        CLONE_H(CUI_PauseMenu_Page_Status, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private:
        weak_ptr<CUI_PauseMenu_Tap_Status>       m_pStatusTap;
        weak_ptr<CUI_PauseMenu_Tap_PlagueWeapon> m_pPlagueWeaponTap;
        weak_ptr<CUI_PauseMenu_Tap_Potion>      m_pPotionTap;


};

END
