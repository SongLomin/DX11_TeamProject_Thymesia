#include "stdafx.h"
#include "Inventory.h"
#include "GameInstance.h"
#include "Item.h"
#include "UI_ItemPopup.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Status_Player.h"
#include "Player.h"

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

	m_szFieldName = "Inventory";

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

void CInventory::Push_Item(ITEM_NAME eItemName, _bool bCallPopup)
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

	if (bCallPopup)
	{
		GET_SINGLE(CUIManager)->Add_ItemPopup(eItemName);
	}
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

void CInventory::Use_Item(ITEM_NAME eItemName, _uint iUseQuantity)
{
	weak_ptr<CItem> pItem = Find_Item(eItemName);

	if (!pItem.lock())
	{
		return;//마우스오버 안올린 아이템을 쓴다? 이건 오류임.
	}
	weak_ptr<CStatus_Player> pPlayerStatus = m_pOwner.lock()->Get_Component<CStatus_Player>();

	switch (eItemName)
	{
	case Client::ITEM_NAME::BASIL:
		pPlayerStatus.lock()->Heal_PlayerFromMaxHP(0.1f);
		break;
	case Client::ITEM_NAME::THYME:
		pPlayerStatus.lock()->Add_Str(10);
		break;
	case Client::ITEM_NAME::CINNAMON:
		pPlayerStatus.lock()->MPHeal_PlayerFromMaxMP(0.1f);
		break;
	case Client::ITEM_NAME::MEMORY01:
		pPlayerStatus.lock()->Add_Memory(10000);
		break;
	case Client::ITEM_NAME::MEMORY02:
		pPlayerStatus.lock()->Add_Memory(100000);
		break;
	case Client::ITEM_NAME::SKILLPIECE_VARGSWORD:
		break;
	case Client::ITEM_NAME::SKILLPIECE_AXE:
		break;
	case Client::ITEM_NAME::SKILLPIECE_KNIFE:
		break;
	case Client::ITEM_NAME::SKILLPIECE_SCYTHE:
		break;
	case Client::ITEM_NAME::SKILLPIECE_HAMMER:
		break;
	case Client::ITEM_NAME::SKILLPIECE_BLOODSTORM:
		break;
	case Client::ITEM_NAME::SKILLPIECE_HALBERDS:
		break;
	case Client::ITEM_NAME::ITEM_NAME_END:
		return;
		break;
	}
	pItem.lock()->Minus_Quantity(iUseQuantity);

	if (pItem.lock()->Get_CurrentQuantity() == 0)
	{
		ITEMMAP::iterator pair = m_mapInventory.find(pItem.lock()->Get_Name());

		if (pair != m_mapInventory.end())
		{
			m_mapInventory.erase(pair);
		}
	}
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

void CInventory::Write_SaveData(json& Out_Json)
{

	json& writeJson = Out_Json;

	for (auto& pair : m_mapInventory)
	{
		json pairInfo;

		pairInfo.push_back(pair.second->Get_Name());
		pairInfo.push_back(pair.second->Get_CurrentQuantity());
		pairInfo.push_back(pair.second->Get_CreatedTime());

		writeJson[m_szFieldName].push_back(pairInfo);
	}
}

void CInventory::Load_SaveData(const json& In_Json)
{
	json LoadedJson = In_Json;

	if (LoadedJson.find(m_szFieldName) == LoadedJson.end())
	{
		return;
	}

	enum class INVENTORY_TYPE
	{
		ITEM_NAME,
		CURRENT_QUANTITY = 1,
		CREATED_TIME = 2
	};
	m_mapInventory.clear();

	shared_ptr<CItem> pItem;
	for (auto& elem : In_Json[m_szFieldName])
	{
		pItem = CItem::Create(elem[(_uint)INVENTORY_TYPE::ITEM_NAME]);

		pItem->Set_CurrentQuantity(elem[(_uint)INVENTORY_TYPE::CURRENT_QUANTITY]);

		pItem->Set_CreatedTime(elem[(_uint)INVENTORY_TYPE::CREATED_TIME]);

		m_mapInventory.emplace(pItem->Get_Name(), pItem);
	}
}
