#include "stdafx.h"
#include "Preset_UIDesc.h"
#include "UI_MonsterFocus.h"
#include "Player_HPBar.h"
#include "Player_MPBar.h"
#include "Texture.h"
#include "CustomUI.h"
#include "ProgressBar.h"
#include "UI.h"
#include "HUD_Hover.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "UI_ItemInformation.h"
#include "Item.h"
#include "Preset_ItemData.h"
#include "UI_InventoryConfirmWindowButton.h"

void CPreset_UIDesc::Set_CUI_MonsterFocus(weak_ptr<class CUI_MonsterFocus> pUI)
{
	pUI.lock()->Set_UIPosition
	(
		0.f,0.f,150.f,150.f
	);
	pUI.lock()->Set_Depth(0.f);
	pUI.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);
	pUI.lock()->Set_Texture("Target_Icon");
    pUI.lock()->Set_PassIndex(13);
}

void CPreset_UIDesc::Set_CUI_PlayerHPBar(weak_ptr<class CPlayer_HPBar> pUI)
{

    pUI.lock()->Set_UIPosition(250.f, 800.f, 400.f, 15.f);

    CUI::UI_DESC tUIDesc = pUI.lock()->Get_UIDESC();

     pUI.lock()->m_pBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
     pUI.lock()->m_pBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_BG");
     pUI.lock()->m_pBG.lock()->Set_UIPosition(tUIDesc.fX, tUIDesc.fY, tUIDesc.fSizeX, tUIDesc.fSizeY);
     pUI.lock()->m_pBG.lock()->Set_Depth(0.2f);


    pUI.lock()->m_pMainBar = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
    pUI.lock()->m_pMainBar.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_MainBar");
    pUI.lock()->m_pMainBar.lock()->Set_UIPosition(tUIDesc.fX, tUIDesc.fY, tUIDesc.fSizeX, tUIDesc.fSizeY - 5.f);
    pUI.lock()->m_pMainBar.lock()->Set_Depth(0.1f);

    pUI.lock()->m_pBorderLeft = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    pUI.lock()->m_pBorderLeft.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Border_Left");
    pUI.lock()->m_pBorderLeft.lock()->Set_UIPosition(tUIDesc.fX - (tUIDesc.fSizeX * 0.5f), tUIDesc.fY, 26.f, 15.f);

    pUI.lock()->m_pBorderRight = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    pUI.lock()->m_pBorderRight.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Border_Right");
    pUI.lock()->m_pBorderRight.lock()->Set_UIPosition(tUIDesc.fX + (tUIDesc.fSizeX * 0.5f), tUIDesc.fY, 26.f, 15.f);


    Engine::FaderDesc tFaderDesc;

    CUI::UI_DESC   tTrackDesc;
    tTrackDesc.fX = tUIDesc.fX + (tUIDesc.fSizeX * 0.5f);
    tTrackDesc.fY = tUIDesc.fY;
    tTrackDesc.fSizeX = 19.f;
    tTrackDesc.fSizeY = 45.f;
    tTrackDesc.fDepth = 0.f;

    tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    tFaderDesc.eFaderType = FADER_TYPE::FADER_INOUTLOOPING;
    tFaderDesc.fDelayTime = 0.f;
    tFaderDesc.fFadeMaxTime = 1.f;
    tFaderDesc.vFadeColor = _float4(0, 0, 0, 1.f);

    CHUD_Hover::HUDHOVERDESC tHoverDesc;
    ZeroMemory(&tHoverDesc, sizeof(CHUD_Hover::HUDHOVERDESC));

    tHoverDesc.bSizeChange = false;
    tHoverDesc.eType = CHUD_Hover::HUD_HOVER_ANIMATION_FROM_ALPHA;


    pUI.lock()->m_pTrack = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &tTrackDesc);
    pUI.lock()->m_pTrack.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Track");
    pUI.lock()->m_pTrack.lock()->Init_Fader(tFaderDesc, tHoverDesc);
    pUI.lock()->m_pTrack.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);

   pUI.lock()-> m_fLerpHp = 0.f;
   pUI.lock()-> m_fCurrentHp = 0.f;
   pUI.lock()-> m_fMaxHp = 0.f;

    pUI.lock()->m_tTextInfo.bAlways = false;
    pUI.lock()->m_tTextInfo.bCenterAlign = false;
    pUI.lock()->m_tTextInfo.fRotation = 0.f;
    pUI.lock()->m_tTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    pUI.lock()->m_tTextInfo.vScale = _float2(0.5, 0.5f);
    pUI.lock()->m_tTextInfo.vPosition = _float2(tUIDesc.fX + tUIDesc.fSizeX * 0.5f + 20.f, tUIDesc.fY - 10.f);
    pUI.lock()->m_tTextInfo.eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    pUI.lock()->m_tUIDesc.fDepth = 0.f;

    pUI.lock()->m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    pUI.lock()->Add_Child(pUI.lock()->m_pMainBar);
    pUI.lock()->Add_Child(pUI.lock()->m_pBG);
    pUI.lock()->Add_Child(pUI.lock()->m_pBorderLeft);
    pUI.lock()->Add_Child(pUI.lock()->m_pBorderRight);

}

void CPreset_UIDesc::Set_CUI_PlayerMPBar(weak_ptr<class CPlayer_MPBar> pUI)
{
    pUI.lock()->Set_UIPosition(200.f, 830.f, 300.f, 8.f);

    CUI::UI_DESC tUIDesc = pUI.lock()->Get_UIDESC();

    pUI.lock()->m_pBG = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
    pUI.lock()->m_pBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_BG");
    pUI.lock()->m_pBG.lock()->Set_UIPosition(tUIDesc.fX, tUIDesc.fY, tUIDesc.fSizeX, tUIDesc.fSizeY);
   
    pUI.lock()->m_pMainBar = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
    pUI.lock()->m_pMainBar.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_MainBar");
    pUI.lock()->m_pMainBar.lock()->Set_UIPosition(tUIDesc.fX, tUIDesc.fY, tUIDesc.fSizeX, tUIDesc.fSizeY - 5.f);

    pUI.lock()->m_pBorderLeft = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
    pUI.lock()->m_pBorderLeft.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_Border_Left");
    pUI.lock()->m_pBorderLeft.lock()->Set_UIPosition(tUIDesc.fX - (tUIDesc.fSizeX * 0.5f), tUIDesc.fY, 11.f, 8.f);
   
    pUI.lock()->m_pBorderRight = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
    pUI.lock()->m_pBorderRight.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_Border_Right");
    pUI.lock()->m_pBorderRight.lock()->Set_UIPosition(tUIDesc.fX + (tUIDesc.fSizeX * 0.5f), tUIDesc.fY, 26.f, 8.f);
    
    pUI.lock()->m_tUIDesc.fDepth = 0.f;
    
    pUI.lock()->m_fCurrentMp = 0.f;
    pUI.lock()->m_fLerpedMp = 0.f;
    
    pUI.lock()->m_tUIDesc.fDepth = 0.f;
    
    pUI.lock()->m_tTextInfo.bAlways = false;
    pUI.lock()->m_tTextInfo.bCenterAlign = false;
    pUI.lock()->m_tTextInfo.fRotation = 0.f;
    pUI.lock()->m_tTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    pUI.lock()->m_tTextInfo.vScale = _float2(0.5, 0.5f);
    pUI.lock()->m_tTextInfo.vPosition = _float2(tUIDesc.fX + tUIDesc.fSizeX * 0.5f + 20.f, tUIDesc.fY - 10.f);
    pUI.lock()->m_tTextInfo.eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;
    
    pUI.lock()->m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    pUI.lock()->Add_Child(pUI.lock()->m_pMainBar);
    pUI.lock()->Add_Child(pUI.lock()->m_pBG);
    pUI.lock()->Add_Child(pUI.lock()->m_pBorderLeft);
    pUI.lock()->Add_Child(pUI.lock()->m_pBorderRight);

}

void CPreset_UIDesc::Set_CUI_ItemInformation(weak_ptr<class CUI_ItemInformation> pUI)
{
    pUI.lock()->m_pItemTitle = ADD_STATIC_CUSTOMUI;
    pUI.lock()->m_pItemTitle.lock()->Set_Texture("None");
    pUI.lock()->m_pItemTitle.lock()->Set_UIPosition
    (
        810.f,
        155.f,
        400.f,
        55.f,
        CUI::ALIGN_LEFTTOP
    );
    pUI.lock()->m_pItemTitle.lock()->Set_Depth(0.6f);


    pUI.lock()->m_pItemInformation = ADD_STATIC_CUSTOMUI;
    pUI.lock()->m_pItemInformation.lock()->Set_Texture("None");
    pUI.lock()->m_pItemInformation.lock()->Set_UIPosition
    (
        810.f,
        342.f,
        594.f,
        447.f,
        CUI::ALIGN_LEFTTOP
    );
    pUI.lock()->m_pItemInformation.lock()->Set_Depth(0.6f);

    pUI.lock()->m_pItemType = ADD_STATIC_CUSTOMUI;
    pUI.lock()->m_pItemType.lock()->Set_Texture("None");
    pUI.lock()->m_pItemType.lock()->Set_UIPosition
    (
        810.f,
        209.f,
        300.f,
        42.f,
        CUI::ALIGN_LEFTTOP
    );
    pUI.lock()->m_pItemType.lock()->Set_Depth(0.6f);


    pUI.lock()->m_pItemQuantity = ADD_STATIC_CUSTOMUI;
    pUI.lock()->m_pItemQuantity.lock()->Set_Texture("None");
    pUI.lock()->m_pItemQuantity.lock()->Set_UIPosition
    (
        810.f,
        261.f,
        300.f,
        42.f,
        CUI::ALIGN_LEFTTOP
    );
    pUI.lock()->m_pItemQuantity.lock()->Set_Depth(0.6f);

    pUI.lock()->m_pIcon = ADD_STATIC_CUSTOMUI;
    pUI.lock()->m_pIcon.lock()->Set_Texture("None");
    pUI.lock()->m_pIcon.lock()->Set_UIPosition
    (
        1245.f,
        150.f,
        160.f,
        160.f,
        CUI::ALIGN_LEFTTOP
    );
    pUI.lock()->m_pIcon.lock()->Set_Depth(0.6f);

    pUI.lock()->m_pItemTitleDecoration = ADD_STATIC_CUSTOMUI;
    pUI.lock()->m_pItemTitleDecoration.lock()->Set_Texture("None");
    pUI.lock()->m_pItemTitleDecoration.lock()->Set_UIPosition
    (
        793.f,
        170.f,
        14.f,
        14.f,
        CUI::ALIGN_LEFTTOP
    );
    pUI.lock()->m_pItemTitleDecoration.lock()->Set_Depth(0.6f);
    

    pUI.lock()->m_pItemQuantityDecoration = ADD_STATIC_CUSTOMUI;
    pUI.lock()->m_pItemQuantityDecoration.lock()->Set_Texture("None");
    pUI.lock()->m_pItemQuantityDecoration.lock()->Set_UIPosition
    (
        818.f,
        250.f,
        228.f,
        2.f,
        CUI::ALIGN_LEFTTOP
    );
    pUI.lock()->m_pItemQuantityDecoration.lock()->Set_Depth(0.6f);
    
    pUI.lock()->m_pItemTypeDecoration = ADD_STATIC_CUSTOMUI;
    pUI.lock()->m_pItemTypeDecoration.lock()->Set_Texture("None");
    pUI.lock()->m_pItemTypeDecoration.lock()->Set_UIPosition
    (
        818.f,
        301.f,
        228.f,
        2.f,
        CUI::ALIGN_LEFTTOP
    );
    pUI.lock()->m_pItemTypeDecoration.lock()->Set_Depth(0.6f);

    TEXTINFO    tTextInfo;
    tTextInfo.bAlways = false;
    tTextInfo.bCenterAlign = true;
    tTextInfo.eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;
    tTextInfo.fRotation = 0.f;
    tTextInfo.vColor = _float4(1.f, 1.f, 1.f, .1f);
    tTextInfo.vPosition = _float2(990.f, 270.f);
    tTextInfo.vScale = _float2(0.7f, 0.7f);
    tTextInfo.szText = TEXT("");

    pUI.lock()->m_tTextInfoQuantity = tTextInfo;
}

void CPreset_UIDesc::Set_CUI_ItemInformaiton_BindItem(weak_ptr<class CUI_ItemInformation> pUI, weak_ptr<class CItem> pItem)
{
    //set To Decoration->¾ê³×´Â ¹Ù²îÁö ¾ÊÀ¸´Ï ¾ê³×µéºÎÅÍ ÇØÁÜ.
    pUI.lock()->m_pItemTitleDecoration.lock()->Set_Texture("Font_Diamond");
    pUI.lock()->m_pItemTypeDecoration.lock()->Set_Texture("Font_Decoration4");
    pUI.lock()->m_pItemQuantityDecoration.lock()->Set_Texture("Font_Decoration4");

    pUI.lock()->m_pItemQuantity.lock()->Set_Texture("Item_Quantity");

    ITEM_NAME eItemName= pItem.lock()->Get_Name();

    switch (eItemName)
    {
    case Client::ITEM_NAME::BASIL:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_Basil_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_Basil_Information");
        break;
    case Client::ITEM_NAME::CINNAMON:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_Cinnamon_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_Cinnamon_Information");
        break;
    case Client::ITEM_NAME::THYME:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_Thyme_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_Thyme_Information");
        break;
    case Client::ITEM_NAME::GARDEN_KEY:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_GardenKey_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_GardenKey_Information");
        break;
    case Client::ITEM_NAME::VARG_KEY:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_VargKey_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_VargKey_Information");
        break;
    case Client::ITEM_NAME::MEMORY01:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_Memory01_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_Memory01_Information");
        break;
    case Client::ITEM_NAME::MEMORY02:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_Memory02_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_Memory02_Information");
        break;

    case Client::ITEM_NAME::SKILLPIECE_AXE:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_SkillPiece_Axe_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_SkillPiece_Information");
        break;
    case Client::ITEM_NAME::SKILLPIECE_HAMMER:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_SkillPiece_Hammer_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_SkillPiece_Information");
        break;
    case Client::ITEM_NAME::SKILLPIECE_KNIFE:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_SkillPiece_Knife_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_SkillPiece_Information");
        break;
    case Client::ITEM_NAME::SKILLPIECE_SCYTHE:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_SkillPiece_Scythe_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_SkillPiece_Information");
        break;
    case Client::ITEM_NAME::SKILLPIECE_VARGSWORD:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_SkillPiece_VargSword_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_SkillPiece_Information");
        break;
    case Client::ITEM_NAME::SKILLPIECE_BLOODSTORM:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_SkillPiece_BloodStorm_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_SkillPiece_Information");
        break;
    case Client::ITEM_NAME::SKILLPIECE_HALBERDS:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_SkillPiece_Halberd_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_SkillPiece_Information");
        break;
    case Client::ITEM_NAME::SKILLPIECE_BIGHAND:
        pUI.lock()->m_pItemTitle.lock()->Set_Texture("Item_SkillPiece_BigHand_Title");
        pUI.lock()->m_pItemInformation.lock()->Set_Texture("Item_SkillPiece_Information");
        break;
    default:
        break;
    }
    CPreset_ItemData::SetUITextureFromItemName(pUI.lock()->m_pIcon, eItemName);

    ITEM_TYPE eItemType = pItem.lock()->Get_Type();

    switch (eItemType)
    {
    case Client::ITEM_TYPE::INGREDIENT:
        pUI.lock()->m_pItemType.lock()->Set_Texture("Item_Type_Ingredient");
        break;
    case Client::ITEM_TYPE::CONSUMPTION:
        pUI.lock()->m_pItemType.lock()->Set_Texture("Item_Type_Consumption");
        break;
    case Client::ITEM_TYPE::SKILLPIECE:
        pUI.lock()->m_pItemType.lock()->Set_Texture("Item_Type_SkillPiece");
        break;
    case Client::ITEM_TYPE::COMMON:
        pUI.lock()->m_pItemType.lock()->Set_Texture("Item_Type_Common");
        break;
    default:
        break;
    }
}
void CPreset_UIDesc::Set_CUI_ItemPopup_Ready_Popup(weak_ptr<class CUI> pUI, ITEM_NAME eItemName)
{
    switch (eItemName)
    {
    case Client::ITEM_NAME::BASIL:
        pUI.lock()->Set_Texture("Popup_Item_Basil");
        break;
    case Client::ITEM_NAME::THYME:
        pUI.lock()->Set_Texture("Popup_Item_Thyme");
        break;
    case Client::ITEM_NAME::CINNAMON:
        pUI.lock()->Set_Texture("Popup_Item_Cinnamon");
        break;
    case Client::ITEM_NAME::GARDEN_KEY:
        pUI.lock()->Set_Texture("Popup_Item_GardenKey");
        break;
    case Client::ITEM_NAME::VARG_KEY:
        pUI.lock()->Set_Texture("Popup_Item_VargKey");
        break;
    case Client::ITEM_NAME::MEMORY01:
        pUI.lock()->Set_Texture("Popup_Item_Memory01");
        break;
    case Client::ITEM_NAME::MEMORY02:
        pUI.lock()->Set_Texture("Popup_Item_Memory02");
        break;

    case Client::ITEM_NAME::SKILLPIECE_AXE:
        pUI.lock()->Set_Texture("Popup_SkillPiece_Axe");
        break;
    case Client::ITEM_NAME::SKILLPIECE_HAMMER:
        pUI.lock()->Set_Texture("Popup_SkillPiece_Hammer");
        break;
    case Client::ITEM_NAME::SKILLPIECE_KNIFE:
        pUI.lock()->Set_Texture("Popup_SkillPiece_Knife");
        break;
    case Client::ITEM_NAME::SKILLPIECE_SCYTHE:
        pUI.lock()->Set_Texture("Popup_SkillPiece_Scythe");
        break;
    case Client::ITEM_NAME::SKILLPIECE_VARGSWORD:
        pUI.lock()->Set_Texture("Popup_SkillPiece_Varg");
        break;
    case Client::ITEM_NAME::SKILLPIECE_BLOODSTORM:
        pUI.lock()->Set_Texture("Popup_SkillPiece_BloodStorm");
        break;
    case Client::ITEM_NAME::SKILLPIECE_HALBERDS:
        pUI.lock()->Set_Texture("Popup_SkillPiece_Halberd");
        break;
    case Client::ITEM_NAME::SKILLPIECE_BIGHAND:
        pUI.lock()->Set_Texture("Popup_SkillPiece_BigHand");
        break;
    default:
        break;
    }
}



