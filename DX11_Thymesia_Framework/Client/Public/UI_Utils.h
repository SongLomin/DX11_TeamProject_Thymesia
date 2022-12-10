#pragma once
#include "Client_Defines.h"
#include "Status_Player.h"

BEGIN(Client)

class CUI_Utils
{
public:
	static _float2		ConvertWorldPosToUIPos(_fvector	vWorldPos);
	static _float		UI_TimeDelta(_float fTimeDelta);

};

END
