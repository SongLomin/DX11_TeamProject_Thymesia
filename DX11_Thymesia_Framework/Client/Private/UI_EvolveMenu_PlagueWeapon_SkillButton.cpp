#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillButton.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillButtonSlot.h"
#include "CustomUI.h"
#include "UI_Utils.h"
#include "UIManager.h"
#include "EasingComponent_Alpha.h"
#include "UI_EvolveMenu_SKillIcon.h"
#include "Inventory.h"
#include "Player.h"
#include "Item.h"
#include "GameManager.h"


GAMECLASS_C(CUI_EvolveMenu_PlagueWeapon_SkillButton)
CLONE_C(CUI_EvolveMenu_PlagueWeapon_SkillButton, CGameObject)

HRESULT CUI_EvolveMenu_PlagueWeapon_SkillButton::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_eRenderGroup = RENDERGROUP::RENDER_UI;

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

    //해금 가능하고, 최대치로 꾹 누르고 있었다면
    if (((m_pIcon.lock()->Get_Ratio() > 0.99)) && Get_Unlockable())
    {
        Unlock_Skill();
        Callback_UnLockSkill(Weak_StaticCast<CUI_EvolveMenu_PlagueWeapon_SkillButton>(m_this));
    }

    if (m_eLockType == SKILLBUTTON_LOCK_TYPE::UNLOCKED && m_eButtonState == UI_BUTTON_BUTTON_DOWN)
    {
        POINT		pt;

        GetCursorPos(&pt);
        ScreenToClient(g_hWnd, &pt);
       
       Set_UIPositionAllChilds((_float)pt.x, (_float)pt.y);
    }
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);
}

_bool CUI_EvolveMenu_PlagueWeapon_SkillButton::Check_UnLocked()
{
    if (m_eLockType == SKILLBUTTON_LOCK_TYPE::UNLOCKED)
    {
        return true;
    }

    return false;
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::Set_UIPosition(const _float fX, const _float fY)
{
   __super::Set_UIPosition(fX, fY);

    m_pHover.lock()->Set_UIPosition(fX,fY);
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

_bool CUI_EvolveMenu_PlagueWeapon_SkillButton::Get_Unlockable()
{
    if (Check_UnLocked())//이미 열려있는 녀석은 해금 불가하다.
        return false;

    ITEM_NAME eItemName = CUI_Utils::ConvertSkillNameToSkillPiece(m_eSkillName);

    weak_ptr<CItem> pItem = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()->Find_Item(eItemName);

    
    if (!pItem.lock())
    {
        return false;
    }

    if (pItem.lock()->Get_CurrentQuantity() < Get_RequirementSkillPiece())
    {
        return false;
    }

    return true;
}

_uint CUI_EvolveMenu_PlagueWeapon_SkillButton::Get_RequirementSkillPiece()
{
    _uint iRequirementSkillPiece = 3;
    
    switch (m_eSkillName)
    {
    case Client::SKILL_NAME::SKILL_HAMMER:
        iRequirementSkillPiece = 1;
        break;
    case Client::SKILL_NAME::SKILL_VARGSWORD:
        iRequirementSkillPiece = 1;
        break;
    case Client::SKILL_NAME::SKILL_BLOODSTORM:
        iRequirementSkillPiece = 1;
    case Client::SKILL_NAME::SKILL_BIGHAND:
        iRequirementSkillPiece = 1;
        break;
    case Client::SKILL_NAME::SKILL_END:
        break;
    default:
        break;
    }
    return iRequirementSkillPiece;
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::Unlock_Skill()
{
    m_eLockType = SKILLBUTTON_LOCK_TYPE::UNLOCKED;
    m_pIcon.lock()->Set_PassIndex(0);
    m_pIcon.lock()->Set_AlphaColor(1.f);
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::Set_Slot(weak_ptr<CUI> pSlot)
{
    if (!m_pMyOriginSlot.lock())
    {
        m_pMyOriginSlot = pSlot;
    }
    m_pMyCurrentSlot = pSlot;
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::SetPosToMyCurrentSlot()
{
    _float2 SlotPos = m_pMyCurrentSlot.lock()->GetPos();
        
    Set_UIPositionAllChilds(SlotPos.x, SlotPos.y);
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::SetPosToMyOriginSlot()
{
    _float2 SlotPos = m_pMyOriginSlot.lock()->GetPos();

    Set_UIPositionAllChilds(SlotPos.x, SlotPos.y);
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::SetUp_UI()
{
    Set_Size(114.f, 114.f);

    m_pHover = ADD_STATIC_CUSTOMUI;

    m_pFrame = ADD_STATIC_CUSTOMUI;
    m_pIcon = GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu_SKillIcon>(LEVEL_STATIC);

    m_pHover.lock()->Set_Size(114.f, 114.f);

    m_pFrame.lock()->Set_Size(114.f, 114.f);
    m_pIcon.lock()->Set_Size(213.f, 213.f);

    m_pFrame.lock()->Set_Depth(0.5f);
    m_pIcon.lock()->Set_Depth(0.4f);
    m_pHover.lock()->Set_Depth(0.3f);

    m_pFrame.lock()->Set_Depth(0.55f);
    m_pIcon.lock()->Set_Depth(0.54f);
    m_pHover.lock()->Set_Depth(0.53f);

    m_pFrame.lock()->Set_Texture("HUD_PlagueWeapon_Frame");
    m_pHover.lock()->Set_Texture("None");
    m_pIcon.lock()->Set_Texture("None");


    Add_Child(m_pFrame);
    Add_Child(m_pIcon);
    Add_Child(m_pHover);
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnMouseOver()
{
    GAMEINSTANCE->PlaySound2D("UI_Select0.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));



     Callback_MouseOver(Weak_StaticCast<CUI_EvolveMenu_PlagueWeapon_SkillButton>(m_this));
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnLButtonUp()
{
    if (Get_Unlockable())
    {
        m_pIcon.lock()->EndGauge();
        return;
    }
    else if (Get_Unlocked())
    {
        Callback_ButtonUp(Weak_StaticCast<CUI_EvolveMenu_PlagueWeapon_SkillButton>(m_this));
    }
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnMouseOut()
{

    Callback_MouseOut();

    if (Get_Unlockable())
    {
        m_pIcon.lock()->EndGauge();
    }
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnLButtonDown()
{
    __super::OnLButtonDown();

    if (Get_Unlockable())
    {
        m_pIcon.lock()->StartGauge();
        return;
    }
    if (Get_Unlocked())
    {
        GAMEINSTANCE->PlaySound2D("UI_Select0.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));
        Callback_ButtonDown(Weak_StaticCast<CUI_EvolveMenu_PlagueWeapon_SkillButton>(m_this));
    }

}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    m_pHover.lock()->Set_Enable(false);

    if (Check_UnLocked())
    {
        m_pIcon.lock()->Set_AlphaColor(1.f);
    }
    else if (!Check_UnLocked())
    {
        m_pIcon.lock()->Set_AlphaColor(0.2f);
    }
}

void CUI_EvolveMenu_PlagueWeapon_SkillButton::OnDisable()
{
    __super::OnDisable();


    
}
