#pragma once

#include "UI_Container.h"

BEGIN(Client)

class CUI_EvolveMenu_PlagueWeapon_SkillButton;
class CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot;


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
    void            SetUp_PlayerSkillSlot();

private:
    void            OnEnable(void* pArg) override;
    void            OnDisable() override;


public:
    void            Bind_Callback(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pButton);

    FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>    Callback_OnMouseOver;
    FDelegate<>    Callback_OnMouseOut;
    FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>    Callback_OnUnlockSkill;
    FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>    Callback_OnLButtonUp;
    FDelegate<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>    Callback_OnLButtonDown;


    void        Call_OnMouseOver(weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton);
    void        Call_OnMouseOut();
    void        Call_OnUnlockSkill(weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton);
    void        Call_OnLButtonUp(weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton);
    void        Call_OnLButtonDown(weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton);



private:
    typedef map<SKILL_NAME, weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>  SKILLMAP;


    vector<_float2>             m_vecSkillPos;
    
    vector<weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>>    m_vecSkillButton;
    weak_ptr<CCustomUI>         EvolveMenu_SkillView_Frame;
    
    vector<weak_ptr<CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot>>    m_vecPlayerSkillSlot;

    
private:


private:
	void		Free();
};

END

