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
#include "InventorySorter.h"
#include "UIManager.h"
#include "InventorySorter.h"


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
    Create_SortImage();
    Set_Enable(false);

    m_pInventorySorter = CInventorySorter::Create();

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
    m_bSortCurrentFrame = false;


    __super::Tick(fTimeDelta);

    Update_KeyInput(fTimeDelta);

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

    m_fSlotOffset = 97.f;
    m_fItemSlotStart.x = 187.f;
    m_fItemSlotStart.y = 208.f;

    m_eSortType = INVENTORY_SORTTYPE::SORT_BY_TYPE;

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
           
            Set_ItemSlotPosFromWidthHeightIndex(pItemSlot, j,i);
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
    
    m_pScroll.lock()->SetUp_ScrollFromLeftTop(650.f, 179.f, 11.f, 500.f, 600.f, 1806.f);
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

void CUI_Inventory::Create_SortImage()
{
    m_pSortKeyImage = ADD_STATIC_CUSTOMUI;
    m_pSortKeyImage.lock()->Set_UIPosition
    (
        97.f,
        752.f,
        60.f,
        60.f,
        CUI::ALIGN_LEFTTOP
    );
    m_pSortKeyImage.lock()->Set_Texture("Inventory_SortKey");
    m_pSortKeyImage.lock()->Set_Depth(0.4f);
    
    Add_Child(m_pSortKeyImage);
    m_pSortKeyImage.lock()->Set_RenderGroup(RENDERGROUP::RENDER_AFTER_UI);

    m_pSortByImage = ADD_STATIC_CUSTOMUI;
    m_pSortByImage.lock()->Set_Texture("Inventory_SortByType");
    m_pSortByImage.lock()->Set_UIPosition
    (
        157.f,
        768.f,
        156.f,
        28.f,
        CUI::ALIGN_LEFTTOP
    );
    m_pSortByImage.lock()->Set_Depth(0.4f);
    Add_Child(m_pSortByImage);
    m_pSortByImage.lock()->Set_RenderGroup(RENDERGROUP::RENDER_AFTER_UI);

}

void CUI_Inventory::Set_ItemSlotPosFromWidthHeightIndex(weak_ptr<CUI_ItemSlot> pItemSlot, _uint iWidthIndex, _uint iHeightIndex)
{
    pItemSlot.lock()->Set_UIPosition(m_fItemSlotStart.x + ((_float)iWidthIndex * m_fSlotOffset), m_fItemSlotStart.y + ((_float)iHeightIndex * m_fSlotOffset),
        ALIGN_CENTER);
}

void CUI_Inventory::Start_AnimationSorting(_uint eSortingAnimType)
{
    vector<weak_ptr<CUI_ItemSlot>> pVecItem;

    for (auto& elem : m_vecItemSlot)
    {
        if (!elem.lock()->Get_BindItem().lock())
        {
            continue;
        }
        pVecItem.push_back(elem);
    }
    if (pVecItem.size() < 2)//2개보다 작다.->섞을게 없다.
    {
        GET_SINGLE(CUIManager)->Set_UIAnimation(false);//아까 걸어줬던 락 해제
        return;
    }

   // Start_AnimationPreSorting(pVecItem, m_eSortType);
   pVecItem =  m_pInventorySorter->Sorting_Start(pVecItem, m_fSlotOffset, (_uint)m_eSortType, 
       (CInventorySorter::SORTING_ANIMATION_TYPE)eSortingAnimType);
    for (_uint i = 0; i < pVecItem.size(); i++)
    {
        m_vecItemSlot[i] = pVecItem[i];
    }
    ResetPositionFromVectorIndex();
}

void CUI_Inventory::Start_AnimationPreSorting(vector<weak_ptr<CUI_ItemSlot>>& vecItemSlot, INVENTORY_SORTTYPE eSortType)
{
    sort(vecItemSlot.begin(), vecItemSlot.end(),
        [&](weak_ptr<CUI_ItemSlot> pFirst, weak_ptr<CUI_ItemSlot> pSecond)
        {
            if (!pFirst.lock()->Get_BindItem().lock() ||
            !pSecond.lock()->Get_BindItem().lock())
            {
                return false;
            }
    switch (m_eSortType)
    {
    case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_DATE:
    {
        if (pFirst.lock()->Get_BindItem().lock()->Get_CreatedTime() <
            pSecond.lock()->Get_BindItem().lock()->Get_CreatedTime())
        {
            return true;
        }
    }
    break;
    case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_TYPE:
        if (pFirst.lock()->Get_BindItem().lock()->Get_Type() <
            pSecond.lock()->Get_BindItem().lock()->Get_Type())
        {
          
            return true;

        }
        break;
    case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_QUANTITY:
        if (pFirst.lock()->Get_BindItem().lock()->Get_CurrentQuantity() <
            pSecond.lock()->Get_BindItem().lock()->Get_CurrentQuantity())
        {
            
            return true;
        }
        break;
    default:
        break;
    }
    return false;
        });
}


void CUI_Inventory::Update_KeyInput(_float fTimeDelta)
{
    if (KEY_INPUT(KEY::R, KEY_STATE::TAP))
    {
        if (GET_SINGLE(CUIManager)->Is_Animation())
            return;

        GET_SINGLE(CUIManager)->Set_UIAnimation(true);

        m_pScroll.lock()->Reset_Scroll();
        _uint iSortTypeIndex = (_uint)m_eSortType;
        iSortTypeIndex++;

        if (iSortTypeIndex == (_uint)INVENTORY_SORTTYPE::SORT_BY_END)
            iSortTypeIndex = 0;
        m_eSortType = (INVENTORY_SORTTYPE)iSortTypeIndex;

        Start_AnimationSorting((_uint)CInventorySorter::SORTING_ANIMATION_TYPE::SORTING_ANIMATION_QUICK_FLOW_BEZIER);
       

        Update_SortImages(m_eSortType);
    }

    if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
    {

        if (m_pMouseOveredItem.lock())
        {   
            if (m_pMouseOveredItem.lock()->Get_Type() == ITEM_TYPE::CONSUMPTION || 
                m_pMouseOveredItem.lock()->Get_Type() == ITEM_TYPE::INGREDIENT)
            {
                m_bSortCurrentFrame = true;

                GAMEINSTANCE->PlaySound2D("ItemUse.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHOOSE_SELECT));

                GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>
                    ().lock()->Use_Item(m_pMouseOveredItem.lock()->Get_Name(), 1);
             
                Update_ItemSlotFromPlayerInventory();
                Start_AnimationSorting((_uint)CInventorySorter::SORTING_ANIMATION_TYPE::SORTING_ANIMATION_QUICK);

                if (m_pMouseOveredItem.lock())
                {
                    if (m_pMouseOveredItem.lock()->Get_CurrentQuantity() < 1)
                    {
                        Call_OnMouseOut();

                    }

                }

            }
        }
    }
}

void CUI_Inventory::Update_ItemSlotOffset()
{
    for (auto& elem : m_vecItemSlot)
    {
        elem.lock()->Set_ScrollOffsetY(m_pScroll.lock()->Get_CurrentProgressiveOffset());
        //elem.lock()->Set_ScroolOffsetY(m_fScrollOffsetY);
    }
}

void CUI_Inventory::Update_ItemSlotFromPlayerInventory()
{
    weak_ptr<CUI_ItemSlot> pSlot;

    map<ITEM_NAME, shared_ptr<CItem>> pMapItem = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()
        ->Get_Component<CInventory>().lock()->Get_Inventory();

    _uint       iIndex = 0;

    for (auto& elem : m_vecItemSlot)
    {
        elem.lock()->OffMouseHover();
        elem.lock()->UnBind_Item();
    }

    for(auto& pair : pMapItem)
    {
        m_vecItemSlot[iIndex++].lock()->Bind_Item(pair.second);
    }
    Update_TextInfoToInventorySize(pMapItem.size());
}

void CUI_Inventory::Update_SortImages(INVENTORY_SORTTYPE eCurrentSortType)
{
    switch (eCurrentSortType)
    {
    case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_DATE:
        m_pSortByImage.lock()->Set_Texture("Inventory_SortByDate");
        break;
    case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_TYPE:
        m_pSortByImage.lock()->Set_Texture("Inventory_SortByType");
        break;
    case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_QUANTITY:
        m_pSortByImage.lock()->Set_Texture("Inventory_SortByQuantity");
        break;
    default:
        break;
    }
}

void CUI_Inventory::Update_TextInfoToInventorySize(_uint iCurrentInventorySize)
{
    m_tTextInfoQuantity.szText = to_wstring(iCurrentInventorySize);
    m_tTextInfoQuantity.szText += TEXT("/");
    m_tTextInfoQuantity.szText += TEXT("100");
}

void CUI_Inventory::Sort_ItemList(INVENTORY_SORTTYPE eSortType)
{
    vector<weak_ptr<CUI_ItemSlot>> pVecItem;

    for(auto& elem : m_vecItemSlot)
    {
        if (!elem.lock()->Get_BindItem().lock())
        {
            continue;
        }
        pVecItem.push_back(elem);
    }
    if (pVecItem.size() < 2)//2개보다 작다.->섞을게 없다.
        return;

    sort(pVecItem.begin(), pVecItem.end(),
        [&](weak_ptr<CUI_ItemSlot> pFirst, weak_ptr<CUI_ItemSlot> pSecond)
        {
            if (!pFirst.lock()->Get_BindItem().lock() ||
            !pSecond.lock()->Get_BindItem().lock())
            {
                return false;
            }
            switch (m_eSortType)
            {
            case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_DATE:
            {
                if (pFirst.lock()->Get_BindItem().lock()->Get_CreatedTime() <
                    pSecond.lock()->Get_BindItem().lock()->Get_CreatedTime())
                {
                    weak_ptr<CItem> pItem = pFirst.lock()->Get_BindItem();
                    pFirst.lock()->UnBind_Item();
                    pFirst.lock()->Bind_Item(pSecond.lock()->Get_BindItem());
                    
                    pSecond.lock()->UnBind_Item();
                    pSecond.lock()->Bind_Item(pItem);
                    return false;
                }
            }
                break;
            case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_TYPE:
                if (pFirst.lock()->Get_BindItem().lock()->Get_Type() <
                    pSecond.lock()->Get_BindItem().lock()->Get_Type())
                {
                    weak_ptr<CItem> pItem = pFirst.lock()->Get_BindItem();
                    pFirst.lock()->UnBind_Item();
                    pFirst.lock()->Bind_Item(pSecond.lock()->Get_BindItem());
                    
                    pSecond.lock()->UnBind_Item();
                    pSecond.lock()->Bind_Item(pItem);
                    return false;

                }
                break;
            case Client::CUI_Inventory::INVENTORY_SORTTYPE::SORT_BY_QUANTITY:
                if (pFirst.lock()->Get_BindItem().lock()->Get_CurrentQuantity() >
                    pSecond.lock()->Get_BindItem().lock()->Get_CurrentQuantity())
                {
                    weak_ptr<CItem> pItem = pFirst.lock()->Get_BindItem();
                    pFirst.lock()->UnBind_Item();
                    pFirst.lock()->Bind_Item(pSecond.lock()->Get_BindItem());
                  
                    pSecond.lock()->UnBind_Item();
                    pSecond.lock()->Bind_Item(pItem);
                    return false;
                }
                break;
            default:
                break;
            }
            return false;//슬롯 자체는 스왑 X
        });
}

void CUI_Inventory::ResetPositionFromVectorIndex()
{
    weak_ptr<CUI_ItemSlot>  pItemSlot;

    for (int i = 0; i < m_iHeight; i++)
    {
        for (int j = 0; j < m_iWidth; j++)
        {
            pItemSlot = m_vecItemSlot[i * m_iWidth + j];

            if(pItemSlot.lock()->Is_Bind() == false)
            {
                continue;
            }
            Set_ItemSlotPosFromWidthHeightIndex(pItemSlot, j, i);
            pItemSlot.lock()->Set_OriginCenterPosFromThisPos();
            pItemSlot.lock()->Update_TextInfo();
        }
    }
}

void CUI_Inventory::Start_Shuffle()
{
}

void CUI_Inventory::End_Shuffle()
{
}


void CUI_Inventory::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    if (GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock())
    {
        Update_ItemSlotFromPlayerInventory();

        m_pScroll.lock()->Reset_Scroll();
        Start_AnimationSorting((_uint)CInventorySorter::SORTING_ANIMATION_TYPE::SORTING_ANIMATION_QUICK);

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
        elem.lock()->Set_ScrollOffsetY(-fAmount);
    }
}

void CUI_Inventory::Call_OnMouseOver(weak_ptr<CItem> pItem)
{
    if (m_pMouseOveredItem.lock())
    {
        if (m_pMouseOveredItem.lock()->Get_CurrentQuantity() < 1)
        {
            m_pMouseOveredItem = pItem;
        }
    }
    else
    {
        m_pMouseOveredItem = pItem;
    }

    if (pItem.lock()->Get_CurrentQuantity() > 0)
    {
        GAMEINSTANCE->PlaySound2D("UI_ChangeIndex0.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHOOSE_SELECT));
    }

    Callback_OnMouseOver(pItem);
}

void CUI_Inventory::Call_OnMouseOut()
{
    if (!m_bSortCurrentFrame)
    {
        m_pMouseOveredItem = weak_ptr<CItem>();
        Callback_OnMouseOut();
    }

}


