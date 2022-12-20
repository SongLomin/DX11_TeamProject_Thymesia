#pragma once

#include "UI_Container.h"

BEGIN(Client)


class CCustomUI;
class CUI_ItemSlot;
class CUI_Scroll;
class CItem;

class CUI_Inventory : public CUI_Container
{
    enum class INVENTORY_SORTTYPE
    {
        SORT_BY_TYPE,
        SORT_BY_DATE,
        SORT_BY_QUANTITY
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
    void                    Update_KeyInput(_float fTimeDelta);
    void                    Update_ItemSlotOffset();

    void                    Update_ItemSlotFromPlayerInventory();
    void                    Update_SortImages(INVENTORY_SORTTYPE eCurrentSortType);
    void                    Update_TextInfoToInventorySize(_uint iCurrentInventorySize);


    void                    Sort_ItemList(INVENTORY_SORTTYPE eSortType);

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
    _float2                 m_fItemSlotStart;//Start to LeftTop
    _float                  m_fOffset;


    _float                 m_fScrollOffsetY = 0;
    _float                 m_fScroolSpeed = 300.f;


	weak_ptr<CCustomUI>		m_pFrame;
	weak_ptr<CCustomUI>		m_pBG;

private:
    vector<weak_ptr<CUI_ItemSlot>>  m_vecItemSlot;
    weak_ptr<CUI_Scroll>            m_pScroll;

    INVENTORY_SORTTYPE              m_eSortType;
    weak_ptr<CCustomUI>              m_pSortKeyImage;
    weak_ptr<CCustomUI>              m_pSortByImage;


    TEXTINFO                        m_tTextInfoQuantity;
    _float                          m_fFontSize;

//Free
private:
	void		Free() {};


};

END