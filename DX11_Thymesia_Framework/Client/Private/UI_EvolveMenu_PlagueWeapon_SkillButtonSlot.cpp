#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillButtonSlot.h"
#include "CustomUI.h"
#include "EasingComponent_Alpha.h"
#include "UI_Utils.h"

GAMECLASS_C(CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot)
CLONE_C(CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot, CGameObject)


HRESULT CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Init_UI();

    return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();
    __super::Tick(fTimeDelta);

}

void CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();
    __super::LateTick(fTimeDelta);

}

void CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot::OnMouseOver()
{
    m_pHoverEffectEasingAlpha.lock()->Set_Lerp(0.f, 1.f, 0.5f, EASING_TYPE::QUAD_INOUT, CEasingComponent::LOOP_GO_AND_BACK, false);
}

void CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot::OnMouseOut()
{
    m_pHoverEffectEasingAlpha.lock()->Set_Lerp(m_pHoverEffect.lock()->Get_AlphaColor(), 0.f, 0.5f, EASING_TYPE::QUAD_OUT, CEasingComponent::ONCE, false);
}

void CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot::Init_UI()
{
    Set_Size(125.f, 125.f);
    
    m_pFrame = ADD_STATIC_CUSTOMUI;
    m_pHoverEffect = ADD_STATIC_CUSTOMUI;

    m_pFrame.lock()->Set_Size(125.f, 125.f);
    m_pHoverEffect.lock()->Set_Size(285.f, 285.f);

    m_pFrame.lock()->Set_Depth(0.59f);
    m_pHoverEffect.lock()->Set_Depth(0.6f);

    m_pFrame.lock()->Set_Texture("HUD_Frame");
    m_pHoverEffect.lock()->Set_Texture("EvolveMenu_PW_Frame_Hover");

    m_pHoverEffectEasingAlpha =  m_pHoverEffect.lock()->Add_Component<CEasingComponent_Alpha>();

    Add_Child(m_pFrame);
    Add_Child(m_pHoverEffect);
}

void CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    m_pHoverEffect.lock()->Set_AlphaColor(0.f);
}

void CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot::OnDisable()
{
    __super::OnDisable();

    m_pHoverEffect.lock()->Set_AlphaColor(0.f);
}
