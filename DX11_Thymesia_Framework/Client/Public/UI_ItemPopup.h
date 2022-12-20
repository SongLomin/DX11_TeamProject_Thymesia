#pragma once

#include "CustomUI.h"

BEGIN(Client)

class CCustomUI;
class CEasingComponent_Float;
class CEasingComponent_Alpha;
class CRequirementChecker;

class CUI_ItemPopup : public  CCustomUI
{
public:
    GAMECLASS_H(CUI_ItemPopup)
    CLONE_H(CUI_ItemPopup, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void    Reset();
    void    Ready_Popup(ITEM_NAME eItemName, _float fLifeTime = 2.f);
    void    Start_Popup();


public:
    FDelegate<weak_ptr<CUI_ItemPopup>>     Callback_EndFadeOut;

private:
    virtual void    SetUp_Component() override;


//Variable
private:
    weak_ptr<CEasingComponent_Float>   m_pEasingFloat;
    weak_ptr<CEasingComponent_Float>   m_pEasingAlarm;


    weak_ptr<CEasingComponent_Alpha>   m_pEasingAlpha;

private:
    void        Call_EndFadeIn();


    void        Call_StartFadeOut();

    void        Call_EndFadeOut();


private:
    _float      m_fLifeTime;

private:
    void    Free() {}

};

END

