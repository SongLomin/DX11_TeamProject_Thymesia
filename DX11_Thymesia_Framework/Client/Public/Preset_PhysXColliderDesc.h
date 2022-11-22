#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
END

namespace Preset
{
	namespace PhysXColliderDesc
	{
		void PlayerBodySetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void PlayerBodyTriggerSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void GroundSetting(PHYSXCOLLIDERDESC& Out_Desc, const _bool bTrigger);
		void TestLightPropSetting(PHYSXCOLLIDERDESC& Out_Desc);
		void TestLightPropTriggerSetting(PHYSXCOLLIDERDESC& Out_Desc);
		void StaticPropSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform, const _float fCenterY);
		void StaticInstancingPropSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void StaticBoxDefaultSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
	}
}