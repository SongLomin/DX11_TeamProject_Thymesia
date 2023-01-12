#include "stdafx.h"
#include "Inventory.h"
#include "GameInstance.h"
#include "Item.h"
#include "UI_ItemPopup.h"
#include "GameManager.h"
#include "UIManager.h"


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
	if (eItemName == ITEM_NAME::ITEM_NAME_END)//모종의 이유로 예측불가능한 enum이 들어오면 즉시 나간다.
		return;

	map<ITEM_NAME, shared_ptr<CItem>>::iterator pair;
	
	pair = m_mapInventory.find(eItemName);

	if (pair == m_mapInventory.end())//인벤토리에 없다면 하나 생성
	{
		shared_ptr<CItem> pItem = CItem::Create(eItemName);
		m_mapInventory.emplace(make_pair(eItemName, pItem));
	}
	else
	{
		pair->second->Add_Quantity();//있다면 수량 하나 추가.
	}
	GET_SINGLE(CUIManager)->Add_ItemPopup(eItemName);

	//weak_ptr<CUI_ItemPopup> pPopupUI = GAMEINSTANCE->Get_GameObjects<CUI_ItemPopup>(LEVEL_STATIC).front();
	//pPopupUI.lock()->Ready_Popup(eItemName);
	//pPopupUI.lock()->Start_Popup();
}
void CInventory::Push_Item(MONSTERTYPE eMonsterType)
{
	switch (eMonsterType)
	{
	case Client::MONSTERTYPE::START_NORMAL_MONSTER:
		break;
	case Client::MONSTERTYPE::AXEMAN:
		Push_Item(ITEM_NAME::SKILLPIECE_AXE);
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		Push_Item(ITEM_NAME::SKILLPIECE_KNIFE);
		break;
	case Client::MONSTERTYPE::SKULL:
		Push_Item(ITEM_NAME::SKILLPIECE_AXE);
		break;
	case Client::MONSTERTYPE::GARDENER:
		Push_Item(ITEM_NAME::SKILLPIECE_SCYTHE);
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		Push_Item(ITEM_NAME::SKILLPIECE_AXE);
		break;
	case Client::MONSTERTYPE::BALLOON:
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		Push_Item(ITEM_NAME::SKILLPIECE_SCYTHE);
		break;
	case Client::MONSTERTYPE::JOKER:
		Push_Item(ITEM_NAME::SKILLPIECE_HAMMER);
		break;
	case Client::MONSTERTYPE::VARG:
		Push_Item(ITEM_NAME::SKILLPIECE_VARGSWORD);
		break;
	case Client::MONSTERTYPE::BAT:
		Push_Item(ITEM_NAME::SKILLPIECE_BLOODSTORM);
		break;
	case Client::MONSTERTYPE::URD:
		Push_Item(ITEM_NAME::SKILLPIECE_VARGSWORD);
		break;
	case Client::MONSTERTYPE::TYPE_END:
		break;
	default:
		break;
	}
}

_bool CInventory::Pop_Item(ITEM_NAME eItemName)
{
	map<ITEM_NAME, shared_ptr<CItem>>::iterator pair;

	pair = m_mapInventory.find(eItemName);

	if (pair != m_mapInventory.end())//인벤토리에 있다면
	{
		
		pair->second->Minus_Quantity();//있다면 수량 하나 추가.

		if (pair->second->Get_CurrentQuantity() < 1)
		{
			m_mapInventory.erase(eItemName);
			return true;
		}
	}
	return false;
}

void CInventory::Remove_Item(ITEM_NAME eItemName)
{
	map<ITEM_NAME, shared_ptr<CItem>>::iterator pair;

	pair = m_mapInventory.find(eItemName);

	if (pair != m_mapInventory.end())//인벤토리에 있다면
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
	map<ITEM_NAME, shared_ptr<CItem>>::iterator iter;
	weak_ptr<CItem> pItem;

	iter = m_mapInventory.find(eItemName);

	if (iter != m_mapInventory.end())
		pItem = iter->second;

	return pItem;
}

