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
    GAMECLASS_H(CUI_EvolveMenu)
    CLONE_H(CUI_EvolveMenu, CGameObject)

public:
    virtual HRESULT Initialize(void* pArg) override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;

private://Child 
    weak_ptr<CCustomUI> m_pLeftBG;

private:
    virtual void Free() override;

};

END