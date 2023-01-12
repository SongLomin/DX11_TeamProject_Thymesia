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
	if (eItemName == ITEM_NAME::ITEM_NAME_END)//������ ������ �����Ұ����� enum�� ������ ��� ������.
		return;

	map<ITEM_NAME, shared_ptr<CItem>>::iterator pair;
	
	pair = m_mapInventory.find(eItemName);

	if (pair == m_mapInventory.end())//�κ��丮�� ���ٸ� �ϳ� ����
	{
		shared_ptr<CItem> pItem = CItem::Create(eItemName);
		m_mapInventory.emplace(make_pair(eItemName, pItem));
	}
	else
	{
		pair->second->Add_Quantity();//�ִٸ� ���� �ϳ� �߰�.
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
	case Client::MONSTERTYPE::NMON_END:
		break;
	default:
		break;
	}
}

_bool CInventory::Pop_Item(ITEM_NAME eItemName)
{
	map<ITEM_NAME, shared_ptr<CItem>>::iterator pair;

	pair = m_mapInventory.find(eItemName);

	if (pair != m_mapInventory.end())//�κ��丮�� �ִٸ�
	{
		
		pair->second->Minus_Quantity();//�ִٸ� ���� �ϳ� �߰�.

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

	if (pair != m_mapInventory.end())//�κ��丮�� �ִٸ�
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

	shared_ptr<CItem> pItem;
	for (auto& elem : In_Json[m_szFieldName])
	{
		pItem = CItem::Create(elem[(_uint)INVENTORY_TYPE::ITEM_NAME]);

		pItem->Set_CurrentQuantity(elem[(_uint)INVENTORY_TYPE::CURRENT_QUANTITY]);

		pItem->Set_CreatedTime(elem[(_uint)INVENTORY_TYPE::CREATED_TIME]);

		m_mapInventory.emplace(pItem->Get_Name(), pItem);
	}
}
