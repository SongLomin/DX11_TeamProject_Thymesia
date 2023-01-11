#pragma once

#include "UI_Container.h"

BEGIN(Client)

class CEasingComponent_Float;

class CUI_RadialBlurMask : public CUI_Container
{
public:
	GAMECLASS_H(CUI_RadialBlurMask)
	CLONE_H(CUI_RadialBlurMask, CGameObject)

public:
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Start() override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;


public:
    void                                    Set_Radial(_float fStart, _float fTarget, _float fTime, EASING_TYPE eType = EASING_TYPE::LINEAR);

public:
    FDelegate<>                             Callback_OnEndLerp;
    void                                    Call_OnEndLerp();

private:
    weak_ptr<CEasingComponent_Float>        m_pEasingFloat;

private:
    void                Free();


};

END

