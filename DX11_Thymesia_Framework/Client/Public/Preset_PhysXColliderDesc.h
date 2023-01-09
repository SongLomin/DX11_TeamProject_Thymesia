#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
END

namespace Preset
{
	namespace PhysXColliderDesc
	{
		//void PlayerBodySetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		//void PlayerBodyTriggerSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void GroundSetting(PHYSXCOLLIDERDESC& Out_Desc, const _bool bTrigger);
		void TestLightPropSetting(PHYSXCOLLIDERDESC& Out_Desc);
		void TestLightPropTriggerSetting(PHYSXCOLLIDERDESC& Out_Desc);
		void DynamicPropSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void DynamicBottleSetting(PHYSXCOLLIDERDESC& Out_Desc, _fmatrix In_WorldMatrix);
		void PlayerWeaponSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void ConvexStaticPropSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void StaticPropSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void DynamicPieceSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void StaticInstancingPropSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void StaticBoxDefaultSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform);
		void StaticInteriorBoxDefaultSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform, MESH_VTX_INFO& MeshData, _float* _pOut);
	}
}