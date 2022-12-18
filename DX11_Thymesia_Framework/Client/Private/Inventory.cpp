#include "stdafx.h"
#include "Inventory.h"
#include "GameInstance.h"
#include "Item.h"

GAMECLASS_C(CInventory)
CLONE_C(CInventory, CComponent)


HRESULT CInventory::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}


HRESULT CInventory::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	

	return S_OK;
}

void CInventory::Start()
{
	__super::Start();
}

void CInventory::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CInventory::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CInventory::Push_Item(ITEM_NAME eItemName)
{
	map<ITEM_NAME, shared_ptr<CItem>>::iterator iter;
	
	iter = m_mapInventory.find(eItemName);

	if (iter == m_mapInventory.end())//인벤토리에 없다면 하나 생성
	{
		shared_ptr<CItem> pItem = CItem::Create(eItemName);
		m_mapInventory.emplace(make_pair(eItemName, pItem));
	}
	else
	{
		iter->second->Add_Quantity();//있다면 수량 하나 추가.
	}

}

_bool CInventory::Pop_Item(ITEM_NAME eItemName)
{
	map<ITEM_NAME, shared_ptr<CItem>>::iterator iter;

	iter = m_mapInventory.find(eItemName);

	if (iter != m_mapInventory.end())//인벤토리에 있다면
	{
		
		iter->second->Minus_Quantity();//있다면 수량 하나 추가.

		if (iter->second->Get_CurrentQuantity() < 1)
		{
			m_mapInventory.erase(eItemName);
			return true;
		}
	}
	return false;
}

void CInventory::Remove_Item(ITEM_NAME eItemName)
{
	map<ITEM_NAME, shared_ptr<CItem>>::iterator iter;

	iter = m_mapInventory.find(eItemName);

	if (iter != m_mapInventory.end())//인벤토리에 있다면
	{
		m_mapInventory.erase(eItemName);
	}
}

_uint CInventory::Get_Size()
{
	return m_mapInventory.size();
}

weak_ptr<CItem> CInventory::Find_Item(ITEM_NAME eItemName)
{
	return m_mapInventory.find(eItemName)->second;
}

