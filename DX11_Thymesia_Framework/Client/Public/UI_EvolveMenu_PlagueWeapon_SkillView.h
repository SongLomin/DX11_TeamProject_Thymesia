#pragma once

#include "UI_Container.h"

BEGIN(Client)

class CUI_EvolveMenu_PlagueWeapon_SkillButton;

class CUI_EvolveMenu_PlagueWeapon_SkillView : public CUI_Container
{
public:
	GAMECLASS_H(CUI_EvolveMenu_PlagueWeapon_SkillView)
	CLONE_H(CUI_EvolveMenu_PlagueWeapon_SkillView, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;


private:
    void            SetUp_SkillPos();
    void            SetUp_UI();
    void            SetUp_SkillButtons();

private:
    void            OnEnable(void* pArg) override;
    void            OnDisable() override;


public:
    void            Bind_Callback(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pButton);

    FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>    Callback_OnMouseOver;
    FDelegate<>    Callback_OnMouseOut;

    void        Call_OnMouseOver(weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton);
    void        Call_OnMouseOut();




private:
    vector<_float2>             m_vecSkillPos;
    
    vector<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>    m_vecSkillButton;
    weak_ptr<CCustomUI>         EvolveMenu_SkillView_Frame;
    

private:
	void		Free();
};

END

