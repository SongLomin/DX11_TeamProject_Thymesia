#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillInformation.h"
#include "PlayerSkillHeader.h"
#include "UI_Utils.h"
#include "UIManager.h"
#include "CustomUI.h"
#include "Player.h"
#include "Inventory.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillButton.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillView.h"
#include "UI_Utils.h"
#include "Item.h"
#include "GameManager.h"

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

    SetUp_UnlockedHintUI();
    SetUp_SkillInfoUI();

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

    if (m_pMouseOveredButton.lock())
    {
        if (!m_pMouseOveredButton.lock()->Get_Unlocked())
        {
            GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_tUnlockedHintTextCurrentPiece);
            GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_tUnlockedHintTextRequirePiece);

        }
    }

}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::SetUp_UnlockedHintUI()
{
     m_pUnlockedHintBG = ADD_STATIC_CUSTOMUI;
     m_pUnlockedHintIcon = ADD_STATIC_CUSTOMUI;
     m_pUnlockedHintTextImage = ADD_STATIC_CUSTOMUI;

     m_pUnlockedHintBG.lock()->Set_UIPosition
     (
         811.f,
         394.f,
         608.f,
         113.f,
         CUI::ALIGN_LEFTTOP
     );
     m_pUnlockedHintIcon.lock()->Set_UIPosition
     (
         822.f,
         401.f,
         80.f,
         80.f,
         CUI::ALIGN_LEFTTOP
     );
     m_pUnlockedHintTextImage.lock()->Set_UIPosition
     (
         1021.f,
         411.f,
         223.f,
         60.f,
         CUI::ALIGN_LEFTTOP
     );
     m_pUnlockedHintBG.lock()->Set_Depth(0.5f);
     m_pUnlockedHintIcon.lock()->Set_Depth(0.49f);
     m_pUnlockedHintTextImage.lock()->Set_Depth(0.49f);

     m_pUnlockedHintBG.lock()->Set_Texture("EvolveMenu_UnlockSkillHintBackground");
     m_pUnlockedHintIcon.lock()->Set_Texture("Item_Icon_SkillPiece");
     m_pUnlockedHintTextImage.lock()->Set_Texture("None");

     Add_Child(m_pUnlockedHintBG);
     Add_Child(m_pUnlockedHintIcon);
     Add_Child(m_pUnlockedHintTextImage);
}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::SetUp_SkillInfoUI()
{
    // m_pSkillInfoBG;
    // m_pSkillInfoIconFrame;
    // m_pSkillInfoIcon;
    // m_pSkillInfoActionDescriptionImage;
    // m_pSkillInfoActionTitle

    m_pSkillInfoBG = ADD_STATIC_CUSTOMUI;
    m_pSkillInfoIconFrame = ADD_STATIC_CUSTOMUI;
    m_pSkillInfoIcon = ADD_STATIC_CUSTOMUI;
    m_pSkillInfoActionTitle = ADD_STATIC_CUSTOMUI;
    m_pSkillInfoActionDescription = ADD_STATIC_CUSTOMUI;
    

    m_pSkillInfoBG.lock()->Set_UIPosition
    (
        733.f,
        150.f,
        803.f,
        601.f,
        ALIGN_LEFTTOP
    );

    m_pSkillInfoIconFrame.lock()->Set_UIPosition
    (
        739.f,
        151.f,
        114.f,
        114.f,
        ALIGN_LEFTTOP
    );

    m_pSkillInfoIcon.lock()->Set_UIPosition
    (
        672.f,
        83.f,
        256.f,
        256.f,
        ALIGN_LEFTTOP
    );

    m_pSkillInfoActionTitle.lock()->Set_UIPosition
    (
        1022.f,
        179.f,
        372.f,
        64.f,
        ALIGN_LEFTTOP
    );

    m_pSkillInfoActionDescription.lock()->Set_UIPosition
    (
        880.f,
        275.f,
        656.f,
        300.f,
        ALIGN_LEFTTOP
    );

    m_pSkillInfoBG.lock()->Set_Depth(0.5f);
    m_pSkillInfoIconFrame.lock()->Set_Depth(0.49f);
    m_pSkillInfoIcon.lock()->Set_Depth(0.48f);
    m_pSkillInfoActionTitle.lock()->Set_Depth(0.48f);
    m_pSkillInfoActionDescription.lock()->Set_Depth(0.48f);

    m_pSkillInfoBG.lock()->Set_Texture("EvolveMenu_PW_Background_Right");
    m_pSkillInfoIconFrame.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Ready");
    m_pSkillInfoIcon.lock()->Set_Texture("None");
    m_pSkillInfoActionTitle.lock()->Set_Texture("None");
    m_pSkillInfoActionDescription.lock()->Set_Texture("None");

    Add_Child(m_pSkillInfoBG);
    Add_Child(m_pSkillInfoIconFrame);
    Add_Child(m_pSkillInfoIcon);
    Add_Child(m_pSkillInfoActionDescription);
    Add_Child(m_pSkillInfoActionTitle);
}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::OnEnable(void* pArg)
{
    //부모 호출 X
    for (auto& elem : m_vecChildUI)
    {
        elem.lock()->Set_Enable(false);
    }
}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::OnDisable()
{
    __super::OnDisable();
}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::View_Information(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton)
{
    Clear_Information();
    m_pMouseOveredButton = pSkillButton;
    
    if (!m_pMouseOveredButton.lock())
    { 
        return;
    }
    if (Check_MouseOveredButtonIsUnLocked())
    {
        //스킬 해금되어있음
        Set_SkillInformationUI();
    }
    else
    {
        Set_UnlockedHintUI();
    }
}


_bool CUI_EvolveMenu_PlagueWeapon_SkillInformation::Check_MouseOveredButtonIsUnLocked()
{
    //해금되어있다면 true반환
    if (m_pMouseOveredButton.lock()->Get_Unlocked())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::Set_SkillInformationUI()
{
    m_pSkillInfoBG.lock()->Set_Enable(true);
    m_pSkillInfoIconFrame.lock()->Set_Enable(true);
    m_pSkillInfoIcon.lock()->Set_Enable(true);
    m_pSkillInfoActionDescription.lock()->Set_Enable(true);
    m_pSkillInfoActionTitle.lock()->Set_Enable(true);


    SKILL_NAME eSkillName = m_pMouseOveredButton.lock()->Get_SkillName();

    switch (eSkillName)
    {
    case Client::SKILL_NAME::SKILL_AXE:
        m_pSkillInfoActionTitle.lock()->Set_Texture("PW_Axe_Title");
        m_pSkillInfoActionDescription.lock()->Set_Texture("PW_Axe_Information");
        break;
    case Client::SKILL_NAME::SKILL_KNIFE:
        m_pSkillInfoActionTitle.lock()->Set_Texture("PW_Knife_Title");
        m_pSkillInfoActionDescription.lock()->Set_Texture("PW_Knife_Information");
        break;
    case Client::SKILL_NAME::SKILL_HAMMER:
        m_pSkillInfoActionTitle.lock()->Set_Texture("PW_Hammer_Title");
        m_pSkillInfoActionDescription.lock()->Set_Texture("PW_Hammer_Information");
        break;
    case Client::SKILL_NAME::SKILL_SCYTHE:
        m_pSkillInfoActionTitle.lock()->Set_Texture("PW_Scythe_Title");
        m_pSkillInfoActionDescription.lock()->Set_Texture("PW_Scythe_Information");
        break;
    case Client::SKILL_NAME::SKILL_VARGSWORD:
        m_pSkillInfoActionTitle.lock()->Set_Texture("PW_VargSword_Title");
        m_pSkillInfoActionDescription.lock()->Set_Texture("PW_VargSword_Information");
        break;
    case Client::SKILL_NAME::SKILL_BLOODSTORM:
        m_pSkillInfoActionTitle.lock()->Set_Texture("PW_BloodStorm_Title");
        m_pSkillInfoActionDescription.lock()->Set_Texture("PW_BloodStorm_Information");
        break;
    case Client::SKILL_NAME::SKILL_HALBERDS:
        m_pSkillInfoActionTitle.lock()->Set_Texture("PW_Halberds_Title");
        m_pSkillInfoActionDescription.lock()->Set_Texture("PW_Halberds_Information");
        break;
    case Client::SKILL_NAME::SKILL_BIGHAND:
        m_pSkillInfoActionTitle.lock()->Set_Texture("PW_BigHand_Title");
        m_pSkillInfoActionDescription.lock()->Set_Texture("PW_BigHand_Information");
        break;
    case Client::SKILL_NAME::SKILL_END:
        break;
    default:
        break;
    }
    CUI_Utils::Set_SkillIcon(m_pSkillInfoIcon, eSkillName);
}

//미해금 상태 UI옵션
void CUI_EvolveMenu_PlagueWeapon_SkillInformation::Set_UnlockedHintUI()
{
    m_pUnlockedHintBG.lock()->Set_Enable(true);
    m_pUnlockedHintIcon.lock()->Set_Enable(true);
    m_pUnlockedHintTextImage.lock()->Set_Enable(true);

    _uint iRequirementSkillPiece = m_pMouseOveredButton.lock()->Get_RequirementSkillPiece();

    ITEM_NAME eItemName = CUI_Utils::ConvertSkillNameToSkillPiece(m_pMouseOveredButton.lock()->Get_SkillName());

    weak_ptr<CItem> pItem = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock()->Find_Item(eItemName);

    m_tUnlockedHintTextCurrentPiece.bCenterAlign = true;
    m_tUnlockedHintTextCurrentPiece.eRenderGroup = RENDERGROUP::RENDER_UI;
    m_tUnlockedHintTextCurrentPiece.fRotation = 0.f;
    m_tUnlockedHintTextCurrentPiece.vScale = _float2(0.7f, 0.7f);
    m_tUnlockedHintTextCurrentPiece.vPosition = _float2(951.f, 426.f);

    m_tUnlockedHintTextRequirePiece.bCenterAlign = true;
    m_tUnlockedHintTextRequirePiece.eRenderGroup = RENDERGROUP::RENDER_UI;
    m_tUnlockedHintTextRequirePiece.fRotation = 0.f;
    m_tUnlockedHintTextRequirePiece.szText = TEXT(" / ");
    m_tUnlockedHintTextRequirePiece.szText += to_wstring(iRequirementSkillPiece);
    m_tUnlockedHintTextRequirePiece.vScale = _float2(0.7f, 0.7f);
    m_tUnlockedHintTextRequirePiece.vPosition = _float2(981.f, 426.f);
    m_tUnlockedHintTextRequirePiece.vColor = _float4(1.f, 1.f, 1.f, 1.f);

    if (pItem.lock())
    {
        m_tUnlockedHintTextCurrentPiece.szText = to_wstring(pItem.lock()->Get_CurrentQuantity());
        if (pItem.lock()->Get_CurrentQuantity() >= iRequirementSkillPiece)
        {
            //통과

            m_pUnlockedHintTextImage.lock()->Set_Texture("EvolveMenu_TextEnoughSkillPiece");
            m_tUnlockedHintTextCurrentPiece.vColor = _float4(28.f / 255.f, 209.f / 255.f, 139.f / 255.f, 1.f);
            
        }
        else
        {
            m_pUnlockedHintTextImage.lock()->Set_Texture("EvolveMenu_TextNotEnoughSkillPiece");
            m_tUnlockedHintTextCurrentPiece.vColor = _float4(238.f / 255.f, 94.f / 255.f, 83.f / 255.f, 1.f);

        }
    }
    else
    {
        //스킬피스가 없다면 개수 0개처리
        m_tUnlockedHintTextCurrentPiece.szText = TEXT("0");
        m_pUnlockedHintTextImage.lock()->Set_Texture("EvolveMenu_TextNotEnoughSkillPiece");
        m_tUnlockedHintTextCurrentPiece.vColor = _float4(238.f / 255.f, 94.f / 255.f, 83.f / 255.f, 1.f);

    }
}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::Clear_Information()
{
    for (auto& elem : m_vecChildUI)
    {
        elem.lock()->Set_Enable(false);
    }
    m_pMouseOveredButton = weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton>();
}

void CUI_EvolveMenu_PlagueWeapon_SkillInformation::Free()
{

}
