#pragma once
#include "UI.h"

BEGIN(Client)

class CCustomUI;

class CUI_PauseMenu_Tap_Status :
    public CUI
{
public:
    GAMECLASS_H(CUI_PauseMenu_Tap_Status)
        CLONE_H(CUI_PauseMenu_Tap_Status, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    weak_ptr< CCustomUI> m_pStatusBG;
};

END

