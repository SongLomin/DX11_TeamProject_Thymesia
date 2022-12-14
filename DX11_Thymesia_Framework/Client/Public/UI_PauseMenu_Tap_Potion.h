#pragma once
#include "UI.h"


BEGIN(Client)

class CCustomUI;
class CUI_PlagueWeapon;


class CUI_PauseMenu_Tap_Potion : public CUI
{
public:
    GAMECLASS_H(CUI_PauseMenu_Tap_Potion)
        CLONE_H(CUI_PauseMenu_Tap_Potion, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;


private:
    TEXTINFO            m_PotionTapInfo;
    weak_ptr<CCustomUI> m_pFrame;
    weak_ptr<CCustomUI> m_pIcon;

    weak_ptr<CCustomUI> m_pDecoration[3];


};

END

