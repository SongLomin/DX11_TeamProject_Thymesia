#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CCustomUI;

class CUI_EvolveMenu_PlagueWeapon_SkillButton;

class CEasingComponent_Alpha;

class CUI_EvolveMenu_PlagueWeapon_SkillInformation : public CUI_Container
{
public:
	GAMECLASS_H(CUI_EvolveMenu_PlagueWeapon_SkillInformation)
	CLONE_H(CUI_EvolveMenu_PlagueWeapon_SkillInformation, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;


public:
    void    View_Information(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton);
    void    Clear_Information();

    _bool    Check_MouseOveredButtonIsUnLocked();

    void    Set_SkillInformationUI();
    void    Set_UnlockedHintUI();


private:
    void                    SetUp_UnlockedHintUI();
    void                    SetUp_SkillInfoUI();
private:
    virtual void            OnEnable(void* pArg);
    virtual void            OnDisable();

private:
    weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> m_pMouseOveredButton;


private://UnlockedHintUI
    weak_ptr<CCustomUI>     m_pUnlockedHintBG;
    weak_ptr<CCustomUI>     m_pUnlockedHintIcon;
    weak_ptr<CCustomUI>     m_pUnlockedHintTextImage;

    TEXTINFO                m_tUnlockedHintTextCurrentPiece;
    TEXTINFO                m_tUnlockedHintTextRequirePiece;


private://SkillInformationUI
    weak_ptr<CCustomUI>     m_pSkillInfoBG;
    weak_ptr<CCustomUI>     m_pSkillInfoIconFrame;
    weak_ptr<CCustomUI>     m_pSkillInfoIcon;
    weak_ptr<CCustomUI>     m_pSkillInfoActionDescription;
    weak_ptr<CCustomUI>     m_pSkillInfoActionTitle;

  
    



private:
    void            Free();



};

END

