#include "stdafx.h"
#include "UI_Inventory.h"
#include "UI_Utils.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "UI_ItemSlot.h"

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

    Update_KeyInput(fTimeDelta);

}

void CUI_Inventory::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

}

void CUI_Inventory::Define_Variable()
{
    m_iWidth = 5;
    m_iHeight = 20;


    m_fOffset = 97.f;
    m_fItemSlotStart.x = 187.f;
    m_fItemSlotStart.y = 235.f;
}

void CUI_Inventory::Create_InventoryUI()
{
    m_pFrame = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);

    m_pFrame.lock()->Set_UIDesc(m_tUIDesc);
    m_pFrame.lock()->Set_Texture("Inventory_Frame");
    m_pFrame.lock()->Set_Depth(0.8f);
    
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

            Add_Child(pItemSlot);
            m_vecItemSlot.push_back(pItemSlot);
        }
    }
}

void CUI_Inventory::Update_KeyInput(_float fTimeDelta)
{
    if (KEY_INPUT(KEY::UP, KEY_STATE::HOLD))
    {
        m_fScrollOffsetY -= m_fScroolSpeed * fTimeDelta;
        Update_ItemSlotOffset();
    }

    if (KEY_INPUT(KEY::DOWN, KEY_STATE::HOLD))
    {
        m_fScrollOffsetY += m_fScroolSpeed * fTimeDelta;
        Update_ItemSlotOffset();
    }
}

void CUI_Inventory::Update_ItemSlotOffset()
{
    for (auto& elem : m_vecItemSlot)
    {
        elem.lock()->Set_OffsetY(m_fScrollOffsetY);
    }
}


