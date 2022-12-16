#pragma once
#include "Client_Defines.h"
#include "Status_Player.h"

BEGIN(Client)

class CUI_Utils
{

public:
	static _float2		ConvertWorldPosToUIPos(_fvector	vWorldPos, _fvector vOffset);
	static _float		UI_TimeDelta();
	static _float4&		GET_COLOR(COLOR_PALETTE eColor);
};

END
