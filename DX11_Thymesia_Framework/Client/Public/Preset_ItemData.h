#pragma once
#include "Client_Defines.h"


BEGIN(Client)


class CPreset_ItemData
{
public:
	static void	CreateItemData(weak_ptr<class CItem> pItem, ITEM_NAME eItemName);


	static void	SetUITextureFromItemName(weak_ptr<class CUI> Out_pUI, ITEM_NAME eItemName);


};

END
