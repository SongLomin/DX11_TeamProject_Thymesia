#pragma once
#include "Client_Defines.h"

BEGIN(Client)

class CPreset_UIDesc
{
public:
	static void		Set_CUI_MonsterFocus(weak_ptr<class CUI_MonsterFocus> pUI);

	static void		Set_CUI_PlayerHPBar(weak_ptr<class CPlayer_HPBar> pUI);

	static void		Set_CUI_PlayerMPBar(weak_ptr<class CPlayer_MPBar> pUI);


};

END