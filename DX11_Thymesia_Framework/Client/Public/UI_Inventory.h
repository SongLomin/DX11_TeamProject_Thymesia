#pragma once

#include "UI_Container.h"

BEGIN(Client)


class CCustomUI;
class CUI_ItemSlot;
class CUI_Scroll;
class CItem;
class CInventorySorter;

class CUI_Inventory : public CUI_Container
{
public:
    enum class INVENTORY_SORTTYPE
    {
        SORT_BY_TYPE,
        SORT_BY_QUANTITY,
        SORT_BY_DATE,
        SORT_BY_END
      
    };

public:
    GAMECLASS_H(CUI_Inventory)
    CLONE_H(CUI_Inventory, CGameObject)
public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;


private:
    void                    Define_Variable();
    void                    Create_InventoryUI();
    void                    Create_ItemSlot();
    void                    Create_Scroll();
    void                    Create_TextInfo();
    void                    Create_SortImage();

private:
    void                    Set_ItemSlotPosFromWidthHeightIndex(weak_ptr<CUI_ItemSlot> pItemSlot, _uint iWidthIndex, _uint iHeightIndex);


    void                    Start_AnimationSorting(_uint eSortingAnimType);
    void                    Start_AnimationPreSorting(vector<weak_ptr<CUI_ItemSlot>>& vecItemSlot, INVENTORY_SORTTYPE eSortType);


    void                    Update_KeyInput(_float fTimeDelta);
    void                    Update_ItemSlotOffset();

    void                    Update_ItemSlotFromPlayerInventory();
    void                    Update_SortImages(INVENTORY_SORTTYPE eCurrentSortType);
    void                    Update_TextInfoToInventorySize(_uint iCurrentInventorySize);

    void                    Sort_ItemList(INVENTORY_SORTTYPE eSortType);

    void                    ResetPositionFromVectorIndex();

private:
    void                    Start_Shuffle();               
    void                    End_Shuffle();


private:
    virtual void            OnEnable(void* pArg) override;
    virtual void            OnDisable() override;

//callBack
public:
    FDelegate<weak_ptr<CItem>> Callback_OnMouseOver;
    FDelegate<> Callback_OnMouseOut;

private:
    void                    Call_OnWheelMove(_float fAmount);
    void                    Call_OnMouseOver(weak_ptr<CItem>   pItem);
    void                    Call_OnMouseOut();


private:
    int     m_iWidth;
    int     m_iHeight;

private:
    weak_ptr<CItem>         m_pMouseOveredItem;

    _float2                 m_fItemSlotStart;
    _float                  m_fSlotOffset;
   

    _float                 m_fScrollOffsetY = 0;
    _float                 m_fScroolSpeed = 300.f;


	weak_ptr<CCustomUI>		m_pFrame;
	weak_ptr<CCustomUI>		m_pBG;

private:
    _bool					m_bSortCurrentFrame = false;

    vector<weak_ptr<CUI_ItemSlot>>  m_vecItemSlot;
    weak_ptr<CUI_Scroll>            m_pScroll;

    INVENTORY_SORTTYPE              m_eSortType;
    weak_ptr<CCustomUI>              m_pSortKeyImage;
    weak_ptr<CCustomUI>              m_pSortByImage;


    TEXTINFO                        m_tTextInfoQuantity;
    _float                          m_fFontSize;

    shared_ptr<CInventorySorter>    m_pInventorySorter;

    _uint                           m_iSoundKey;

private:
    _bool                            m_IsAnimating = false;
//Free
private:
	void		Free() {};


};

END