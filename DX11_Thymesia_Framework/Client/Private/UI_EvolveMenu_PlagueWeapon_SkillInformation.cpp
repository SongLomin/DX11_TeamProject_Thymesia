#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillInformation.h"
#include "PlayerSkillHeader.h"
#include "UI_Utils.h"
#include "UIManager.h"

GAMECLASS_C(CUI_EvolveMenu_PlagueWeapon_SkillInformation)
CLONE_C(CUI_EvolveMenu_PlagueWeapon_SkillInformation, CGameObject)


HRESULT CUI_EvolveMenu_PlagueWeapon_SkillInformation::Initialize_Prototype()
{
    __super::Initialize_Prototype();


    return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon_SkillInformation::Initialize(void* pArg)
{
    __super::Initialize(pArg);


    return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon_SkillInformation::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::Call_OnSkillButtonMouseOver(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton)
{
}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::Call_OnSkillButtonMouseOut()
{
}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::Free()
{
}
