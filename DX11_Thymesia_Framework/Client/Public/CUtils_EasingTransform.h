#pragma once
#include "Client_Defines.h"

BEGIN(Client)

class CEasingTransform;

class CUtils_EasingTransform
{
public:
	static void	SetEasingTransform_Loop_RightLeft(weak_ptr<CEasingTransform>	pEasingTransform,
		_float fOffset, _float fTime);


};

END
