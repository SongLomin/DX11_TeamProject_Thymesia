#pragma once
#include "UI.h"

BEGIN(Client)

class CCustomUI;

class CUI_PauseMenu_Tap_PlagueWeapon :
    public CUI
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
    void    CreatePlagueWeapon_Main();
    void    CreatePlagueWeapon_Sub();
    void    CreatePlagueWeapon_Steal();

private:
    
};

END