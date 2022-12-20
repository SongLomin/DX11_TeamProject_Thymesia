#pragma once
#include "Client_Defines.h"

BEGIN(Client)

class CPreset_UIDesc
{
public:
	static void		Set_CUI_MonsterFocus(weak_ptr<class CUI_MonsterFocus> pUI);

	static void		Set_CUI_PlayerHPBar(weak_ptr<class CPlayer_HPBar> pUI);

	static void		Set_CUI_PlayerMPBar(weak_ptr<class CPlayer_MPBar> pUI);

	static void		Set_CUI_ItemInformation(weak_ptr<class CUI_ItemInformation> pUI);

	static void		Set_CUI_ItemInformaiton_BindItem(weak_ptr<class CUI_ItemInformation> pUI, weak_ptr<class CItem> pItem);

	static void		Set_CUI_ItemPopup_Ready_Popup(weak_ptr<class CUI> pUI, ITEM_NAME eItemName);

};

END