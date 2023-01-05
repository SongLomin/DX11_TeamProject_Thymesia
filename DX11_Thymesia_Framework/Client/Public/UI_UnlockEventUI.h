#pragma once
#include "UI_Container.h"

BEGIN(Client)
  
class CCustomUI;
class CEasingComponent_Float;
class CEasingComponent_Alpha;
class CEasingComponent_Transform;

class CUI_UnlockEventUI : public CUI_Container
{
public:
	GAMECLASS_H(CUI_UnlockEventUI)
	CLONE_H(CUI_UnlockEventUI, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

private:

};

END
