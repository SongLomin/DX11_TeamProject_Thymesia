#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillButton.h"
#include "CustomUI.h"
#include "UI_Utils.h"
#include "UIManager.h"
#include "EasingComponent_Alpha.h"

GAMECLASS_C(CUI_EvolveMenu_PlagueWeapon_SkillButton)
CLONE_C(CUI_EvolveMenu_PlagueWeapon_SkillButton, CGameObject)

HRESULT CUI_EvolveMenu_PlagueWeapon_SkillButton::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

    SetUp_UI();
    return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon_SkillButton::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::Check_Locked()
{
    if (m_eLockType == SKILLBUTTON_LOCK_TYPE::LOCKED)
    {
        m_pIcon.lock()->Set_AlphaColor(0.5f);
    }
    if (m_eLockType == SKILLBUTTON_LOCK_TYPE::UNLOCKED)
    {
        m_pIcon.lock()->Set_AlphaColor(1.f);
    }
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::Set_UIPosition(const _float fX, const _float fY)
{
   __super::Set_UIPosition(fX, fY);

    m_pHover.lock()->Set_UIPosition(fX,fY);
    m_pBorder.lock()->Set_UIPosition(fX, fY);
    m_pFrame.lock()->Set_UIPosition(fX,fY);
    m_pIcon .lock()->Set_UIPosition(fX,fY);
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::Set_Skill(SKILL_NAME eSkillName)
{
    m_eSkillName = eSkillName;

    CUI_Utils::Set_SkillIcon(m_pIcon, m_eSkillName);
}

_bool CUI_EvolveMenu_PlagueWeapon_SkillButton::Get_Unlocked()
{
    return (m_eLockType == SKILLBUTTON_LOCK_TYPE::UNLOCKED);
}

_uint CUI_EvolveMenu_PlagueWeapon_SkillButton::Get_RequirementSkillPiece()
{
    _uint iRequirementSkillPiece = 3;
    

    switch (m_eSkillName)
    {
    case Client::SKILL_NAME::SKILL_AXE:
        break;
    case Client::SKILL_NAME::SKILL_KNIFE:
        break;
    case Client::SKILL_NAME::SKILL_HAMMER:
        iRequirementSkillPiece = 1;
        break;
    case Client::SKILL_NAME::SKILL_SCYTHE:
        break;
    case Client::SKILL_NAME::SKILL_VARGSWORD:
        iRequirementSkillPiece = 1;
        break;
    case Client::SKILL_NAME::SKILL_END:
        break;
    default:
        break;
    }

    return iRequirementSkillPiece;
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::SetUp_UI()
{
    Set_Size(114.f, 114.f);

    m_pHover = ADD_STATIC_CUSTOMUI;
    m_pBorder = ADD_STATIC_CUSTOMUI;
    m_pFrame = ADD_STATIC_CUSTOMUI;
    m_pIcon = ADD_STATIC_CUSTOMUI;

    m_pHover.lock()->Set_Size(114.f, 114.f);
    m_pBorder.lock()->Set_Size(256.f, 256.f);
    m_pFrame.lock()->Set_Size(114.f, 114.f);
    m_pIcon.lock()->Set_Size(213.f, 213.f);

    m_pBorder.lock()->Set_Depth(0.6f);
    m_pFrame.lock()->Set_Depth(0.5f);
    m_pIcon.lock()->Set_Depth(0.4f);
    m_pHover.lock()->Set_Depth(0.3f);

    m_pFrame.lock()->Set_Texture("HUD_PlagueWeapon_Frame");
    m_pBorder.lock()->Set_Texture("EvolveMenu_PW_Frame_Hover");
    m_pHover.lock()->Set_Texture("None");
    m_pIcon.lock()->Set_Texture("None");

    m_pBorderEasingAlpha = m_pBorder.lock()->Add_Component<CEasingComponent_Alpha>();

    Add_Child(m_pBorder);
    Add_Child(m_pFrame);
    Add_Child(m_pIcon);

}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnMouseOver()
{
     m_pBorderEasingAlpha.lock()->Set_Lerp(0.f, 1.f, 0.5f, EASING_TYPE::QUAD_INOUT, CEasingComponent::LOOP_GO_AND_BACK, false);

     Callback_MouseOver(Weak_StaticCast<CUI_EvolveMenu_PlagueWeapon_SkillButton>(m_this));
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnMouseOut()
{
    m_pBorderEasingAlpha.lock()->Set_Lerp(m_pBorder.lock()->Get_AlphaColor(), 0.f, 0.5f, EASING_TYPE::QUAD_OUT, CEasingComponent::ONCE, false);

    Callback_MouseOut();
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnLButtonDown()
{
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    m_pHover.lock()->Set_Enable(false);
    m_pBorder.lock()->Set_AlphaColor(0.f);
    Check_Locked();
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnDisable()
{
    __super::OnDisable();

    m_pHover.lock()->Set_Enable(false);
    
}
