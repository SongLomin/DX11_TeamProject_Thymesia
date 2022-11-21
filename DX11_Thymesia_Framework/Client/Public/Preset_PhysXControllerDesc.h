#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
END

namespace Preset
{
	namespace PhysXControllerDesc
	{
		PxCapsuleControllerDesc PlayerSetting(weak_ptr<CTransform> pTransform);
	}
}