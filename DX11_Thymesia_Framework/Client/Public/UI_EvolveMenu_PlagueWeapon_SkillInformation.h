#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CCustomUI;

class CUI_EvolveMenu_PlagueWeapon_SkillButton;


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
    void            Call_OnSkillButtonMouseOver(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton);
    void            Call_OnSkillButtonMouseOut();

private:
    void            Free();



};

END

