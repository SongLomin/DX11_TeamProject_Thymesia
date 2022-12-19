#include "stdafx.h"
#include "UI_Inventory.h"
#include "UI_Utils.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "UI_ItemSlot.h"
#include "UI_Scroll.h"
#include "Inventory.h"
#include "Player.h"
#include "GameManager.h"
#include "Item.h"

GAMECLASS_C(CUI_Inventory)
CLONE_C(CUI_Inventory, CGameObject)

HRESULT CUI_Inventory::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_eRenderGroup = RENDERGROUP::RENDER_UI;

    Set_UIPosition(103.f, 150.f, 600.f, 600.f, CUI::ALIGN_LEFTTOP);
    
    Define_Variable();
    Create_InventoryUI();
    Create_ItemSlot();
    Create_Scroll();
    Create_TextInfo();
    return S_OK;
}

HRESULT CUI_Inventory::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Inventory::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

   // Update_KeyInput(fTimeDelta);

}

void CUI_Inventory::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_tTextInfoQuantity);
}

void CUI_Inventory::Define_Variable()
{
    m_iWidth = 5;
    m_iHeight = 20;

    m_fOffset = 97.f;
    m_fItemSlotStart.x = 187.f;
    m_fItemSlotStart.y = 208.f;
}

void CUI_Inventory::Create_InventoryUI()
{
    m_pFrame = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);

    m_pFrame.lock()->Set_UIDesc(m_tUIDesc);
    m_pFrame.lock()->Set_Texture("Inventory_Frame");
    m_pFrame.lock()->Set_Depth(0.2f);
    
    m_pBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
       
    m_pBG.lock()->Set_UIDesc(m_tUIDesc);
    m_pBG.lock()->Set_Texture("Inventory_FrameBG");
    m_pBG.lock()->Set_Depth(0.9f);

    Add_Child(m_pFrame);
    Add_Child(m_pBG);

}

void CUI_Inventory::Create_ItemSlot()
{
    weak_ptr<CUI_ItemSlot>  pItemSlot;

    for (int i = 0; i < m_iHeight; i++)
    {
        for(int j = 0 ; j < m_iWidth; j++)
        {
            pItemSlot = GAMEINSTANCE->Add_GameObject<CUI_ItemSlot>(LEVEL_STATIC);
           
            pItemSlot.lock()->Set_UIPosition(m_fItemSlotStart.x + (j * m_fOffset), m_fItemSlotStart.y + (i * m_fOffset),
                ALIGN_CENTER);
            pItemSlot.lock()->Set_OriginCenterPosFromThisPos();
            pItemSlot.lock()->Callback_OnMouseOver +=
                bind(&CUI_Inventory::Call_OnMouseOver, this, placeholders::_1);

            pItemSlot.lock()->Callback_OnMouseOut +=
                bind(&CUI_Inventory::Call_OnMouseOut , this);

            
            
            Add_Child(pItemSlot);
            m_vecItemSlot.push_back(pItemSlot);
        }
    }
}

void CUI_Inventory::Create_Scroll()
{
    m_pScroll = GAMEINSTANCE->Add_GameObject<CUI_Scroll>(LEVEL_STATIC);
    
    
    m_pScroll.lock()->SetUp_ScrollFromLeftTop(650.f, 179.f, 500.f, 600.f, 1806.f);
    m_pScroll.lock()->Set_Depth(0.6f);


    m_pScroll.lock()->Callback_OnWheelMove += bind(&CUI_Inventory::Call_OnWheelMove, this, placeholders::_1);

    Add_Child(m_pScroll);

}

void CUI_Inventory::Create_TextInfo()
{
    m_fFontSize = 17.f;
    _float fOriginalFontSize = 23.f;

    _float  fFontRatio = m_fFontSize / fOriginalFontSize;

    m_tTextInfoQuantity.bAlways = false;
    m_tTextInfoQuantity.vScale = _float2(fFontRatio, fFontRatio);
    m_tTextInfoQuantity.bCenterAlign = true;
    m_tTextInfoQuantity.fRotation = 0.f;
    m_tTextInfoQuantity.vColor = _float4(0.8f, 0.8f, 0.8f, 0.8f);
    m_tTextInfoQuantity.vPosition = _float2(653.f, 710.f);
    m_tTextInfoQuantity.szText = TEXT("");
    m_tTextInfoQuantity.eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;
}

void CUI_Inventory::Update_KeyInput(_float fTimeDelta)
{
    if (KEY_INPUT(KEY::UP, KEY_STATE::HOLD))
    {
        m_fScrollOffsetY += m_fScroolSpeed * fTimeDelta;
        Update_ItemSlotOffset();
    }

    if (KEY_INPUT(KEY::DOWN, KEY_STATE::HOLD))
    {
        m_fScrollOffsetY -= m_fScroolSpeed * fTimeDelta;
        Update_ItemSlotOffset();
    }

    if (KEY_INPUT(KEY::ENTER, KEY_STATE::TAP))
    {
        int mcdonalds = 10;
    }
}

void CUI_Inventory::Update_ItemSlotOffset()
{
    for (auto& elem : m_vecItemSlot)
    {
        elem.lock()->Set_ScroolOffsetY(m_pScroll.lock()->Get_CurrentProgressiveOffset());
        //elem.lock()->Set_ScroolOffsetY(m_fScrollOffsetY);
    }
}

void CUI_Inventory::Update_ItemSlotFromPlayerInventory()
{
    weak_ptr<CUI_ItemSlot> pSlot;

    map<ITEM_NAME, shared_ptr<CItem>> pMapItem = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()
        ->Get_Component<CInventory>().lock()->Get_Inventory();

    _uint       iIndex = 0;

    for(auto& pair : pMapItem)
    {
        m_vecItemSlot[iIndex++].lock()->Bind_Item(pair.second);
    }

    Update_TextInfoToInventorySize(pMapItem.size());
}

void CUI_Inventory::Update_TextInfoToInventorySize(_uint iCurrentInventorySize)
{
    m_tTextInfoQuantity.szText = to_wstring(iCurrentInventorySize);
    m_tTextInfoQuantity.szText += TEXT("/");
    m_tTextInfoQuantity.szText += TEXT("100");
}

void CUI_Inventory::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    if (GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock())
    {
        Update_ItemSlotFromPlayerInventory();
    }
}

void CUI_Inventory::OnDisable()
{
    __super::OnDisable();

    for (auto& elem : m_vecItemSlot)
    {
        elem.lock()->UnBind_Item();
    }
}

void CUI_Inventory::Call_OnWheelMove(_float fAmount)
{
    for (auto& elem : m_vecItemSlot)
    {
        elem.lock()->Set_ScroolOffsetY(-fAmount);
    }
}

void CUI_Inventory::Call_OnMouseOver(weak_ptr<CItem> pItem)
{
    Callback_OnMouseOver(pItem);

}

void CUI_Inventory::Call_OnMouseOut()
{
    Callback_OnMouseOut();
}


