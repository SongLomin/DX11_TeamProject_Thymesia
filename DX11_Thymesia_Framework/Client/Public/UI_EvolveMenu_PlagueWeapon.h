#pragma once
#include "UI_EvolveMenu_Page.h"

BEGIN(Client)

class CCustomUI;
class CFadeMask;
class CUI_EvolveMenu_PlagueWeapon_SkillView;
class CUI_EvolveMenu_PlagueWeapon_SkillButton;
class CUI_EvolveMenu_PlagueWeapon_SkillInformation;

class CUI_EvolveMenu_PlagueWeapon : public CUI_EvolveMenu_Page
{
public:
	GAMECLASS_H(CUI_EvolveMenu_PlagueWeapon);
	CLONE_H(CUI_EvolveMenu_PlagueWeapon, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    
private://Init Functions
    void                    Init_Backgrounds();
    void                    Init_ChildUI();

private:
    virtual void	OnEnable(void* pArg) override;
    virtual void	OnDisable() override;

private: //For...Background
    weak_ptr<CCustomUI>     m_pBackgroundBack;
    weak_ptr<CCustomUI>     m_pBackgroundMain;
    weak_ptr<CCustomUI>     m_pBackgroundMainFill;
    weak_ptr<CCustomUI>     m_pBackgroundTop;

    weak_ptr<CCustomUI>     m_pTitle;

public:
    void                    Call_OnSkillButtonMouseOver(weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton);
    void                    Call_OnSkillButtonMouseOut();
    void                    Call_OnUnlockSkill(weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton);
    void                    Call_OnLButtonUp(weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton);


private://For...LeftSkillView
    weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillView>    m_pSkillView;

private://For...RightSkillInformationTap
    weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillInformation> m_pSkillInformation;

private:
    void    Free();

};

END
