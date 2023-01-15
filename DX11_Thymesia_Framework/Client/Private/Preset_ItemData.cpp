#include "stdafx.h"
#include "Preset_ItemData.h"
#include "Client_Defines.h"
#include "Item.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Base.h"
#include "GameObject.h"
#include "UI.h"


void CPreset_ItemData::CreateItemData(weak_ptr<class CItem> pItem, ITEM_NAME eItemName)
{
	pItem.lock()->m_eItemName = eItemName;
	pItem.lock()->m_iCurrentQuantity = 0;
	switch (eItemName)
	{
	case Client::ITEM_NAME::BASIL:
	{
		pItem.lock()->m_eItemType = ITEM_TYPE::INGREDIENT;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	}
	case Client::ITEM_NAME::THYME:
	{
		pItem.lock()->m_eItemType = ITEM_TYPE::INGREDIENT;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	}
	case Client::ITEM_NAME::CINNAMON:
		pItem.lock()->m_eItemType = ITEM_TYPE::INGREDIENT;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	case Client::ITEM_NAME::GARDEN_KEY:
		pItem.lock()->m_eItemType = ITEM_TYPE::COMMON;
		pItem.lock()->m_iMaxQuantity = 1;
		break;
	case Client::ITEM_NAME::VARG_KEY:
		pItem.lock()->m_eItemType = ITEM_TYPE::COMMON;
		pItem.lock()->m_iMaxQuantity = 1;
		break;
	case Client::ITEM_NAME::MEMORY01:
		pItem.lock()->m_eItemType = ITEM_TYPE::CONSUMPTION;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	case Client::ITEM_NAME::MEMORY02:
		pItem.lock()->m_eItemType = ITEM_TYPE::CONSUMPTION;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	case Client::ITEM_NAME::SKILLPIECE_AXE:
		pItem.lock()->m_eItemType = ITEM_TYPE::SKILLPIECE;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	case Client::ITEM_NAME::SKILLPIECE_HAMMER:
		pItem.lock()->m_eItemType = ITEM_TYPE::SKILLPIECE;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	case Client::ITEM_NAME::SKILLPIECE_KNIFE:
		pItem.lock()->m_eItemType = ITEM_TYPE::SKILLPIECE;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	case Client::ITEM_NAME::SKILLPIECE_SCYTHE:
		pItem.lock()->m_eItemType = ITEM_TYPE::SKILLPIECE;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	case Client::ITEM_NAME::SKILLPIECE_VARGSWORD:
		pItem.lock()->m_eItemType = ITEM_TYPE::SKILLPIECE;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	case Client::ITEM_NAME::SKILLPIECE_BLOODSTORM:
		pItem.lock()->m_eItemType = ITEM_TYPE::SKILLPIECE;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	case Client::ITEM_NAME::SKILLPIECE_HALBERDS:
		pItem.lock()->m_eItemType = ITEM_TYPE::SKILLPIECE;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	case Client::ITEM_NAME::SKILLPIECE_BIGHAND:
		pItem.lock()->m_eItemType = ITEM_TYPE::SKILLPIECE;
		pItem.lock()->m_iMaxQuantity = 99;
		break;
	
	default:
		break;
	}
	pItem.lock()->m_iCreatedTime = time(NULL);
	pItem.lock()->Add_Quantity();
}

void CPreset_ItemData::SetUITextureFromItemName(weak_ptr<class CUI> Out_pUI, ITEM_NAME eItemName)
{
	switch (eItemName)
	{
	case Client::ITEM_NAME::BASIL:
		Out_pUI.lock()->Set_Texture("Item_Icon_Basil");
		break;
	case Client::ITEM_NAME::THYME:
		Out_pUI.lock()->Set_Texture("Item_Icon_Thyme");
		break;
	case Client::ITEM_NAME::CINNAMON:
		Out_pUI.lock()->Set_Texture("Item_Icon_Cinnamon");
		break;
	case Client::ITEM_NAME::GARDEN_KEY:
		Out_pUI.lock()->Set_Texture("Item_Icon_Key01");
		break;
	case Client::ITEM_NAME::VARG_KEY:
		Out_pUI.lock()->Set_Texture("Item_Icon_Key02");
		break;
	case Client::ITEM_NAME::MEMORY01:
		Out_pUI.lock()->Set_Texture("Item_Icon_Memory01");
		break;
	case Client::ITEM_NAME::MEMORY02:
		Out_pUI.lock()->Set_Texture("Item_Icon_Memory02");
		break;
	case Client::ITEM_NAME::SKILLPIECE_AXE:
		Out_pUI.lock()->Set_Texture("Item_Icon_SkillPiece");
		break;
	case Client::ITEM_NAME::SKILLPIECE_HAMMER:
		Out_pUI.lock()->Set_Texture("Item_Icon_SkillPiece");
		break;
	case Client::ITEM_NAME::SKILLPIECE_KNIFE:
		Out_pUI.lock()->Set_Texture("Item_Icon_SkillPiece");
		break;
	case Client::ITEM_NAME::SKILLPIECE_SCYTHE:
		Out_pUI.lock()->Set_Texture("Item_Icon_SkillPiece");
		break;
	case Client::ITEM_NAME::SKILLPIECE_VARGSWORD:
		Out_pUI.lock()->Set_Texture("Item_Icon_SkillPiece");
		break;
	case Client::ITEM_NAME::SKILLPIECE_BLOODSTORM:
		Out_pUI.lock()->Set_Texture("Item_Icon_SkillPiece");
		break;
	case Client::ITEM_NAME::SKILLPIECE_HALBERDS:
		Out_pUI.lock()->Set_Texture("Item_Icon_SkillPiece");
		break;
	case Client::ITEM_NAME::SKILLPIECE_BIGHAND:
		Out_pUI.lock()->Set_Texture("Item_Icon_SkillPiece");
		break;
	default:
		break;
	}

}
