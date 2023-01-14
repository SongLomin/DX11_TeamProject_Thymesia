#include "stdafx.h"
#include "Item.h"
#include "Texture.h"
#include "Preset_ItemData.h"
#include "GameInstance.h"

GAMECLASS_C(CItem)


HRESULT CItem::Initalize(ITEM_NAME eItemName)
{

    

    CPreset_ItemData::CreateItemData(Weak_StaticCast<CItem>(m_this), eItemName);

    
    return S_OK;
}

void CItem::Set_CurrentQuantity(_int iQuantity)
{
    m_iCurrentQuantity = iQuantity;
}

void CItem::Set_CreatedTime(time_t iCreatedTime)
{
    m_iCreatedTime = iCreatedTime;
}

void CItem::Add_Quantity()
{
    if (m_iCurrentQuantity >= m_iMaxQuantity)
        return;

    m_iCurrentQuantity++;
}

void CItem::Minus_Quantity()
{
    if (m_iCurrentQuantity < 1)
        return;

    m_iCurrentQuantity--;
}

void CItem::Minus_Quantity(_uint iQuantity)
{
    if (m_iCurrentQuantity < iQuantity)
        m_iCurrentQuantity = 0;
    else
    {
        m_iCurrentQuantity -= iQuantity;
    }
}


shared_ptr<CItem> CItem::Create(ITEM_NAME eItemName)
{   
    shared_ptr<CItem>   pItem = make_shared<CItem>();

    pItem->Set_ThisInstance(pItem);

    pItem->Initalize(eItemName);

    return pItem;
}
