#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CCustomUI;

class CUI_EvolveMenu_PlagueWeapon : public CUI_Container
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



private: //For...Background
    weak_ptr<CCustomUI>     m_pBackgroundBack;
    weak_ptr<CCustomUI>     m_pBackgroundMain;
    weak_ptr<CCustomUI>     m_pBackgroundMainFill;
    weak_ptr<CCustomUI>     m_pBackgroundTop;

private://For...LeftSkillListTap


private://For...RightSkillInformationTap






};

END
