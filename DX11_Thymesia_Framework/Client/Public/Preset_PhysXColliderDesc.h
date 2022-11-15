#pragma once
#include "Client_Defines.h"

namespace Preset
{
	namespace PhysXColliderDesc
	{
		void PlayerBodySetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void PlayerBodyTriggerSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void GroundSetting(PHYSXCOLLIDERDESC& Out_Desc, const _bool bTrigger);
		void TestLightPropSetting(PHYSXCOLLIDERDESC& Out_Desc);
	}
}