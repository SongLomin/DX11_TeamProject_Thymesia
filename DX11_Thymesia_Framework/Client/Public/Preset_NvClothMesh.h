#pragma once

#include "Model.h"

namespace Preset
{
	namespace NvClothMesh
	{
		void CorvusSetting(CModel::NVCLOTH_MODEL_DESC& Out_Desc);
		void VargWeaponSetting(CModel::NVCLOTH_MODEL_DESC& Out_Desc);
		void VargSetting(CModel::NVCLOTH_MODEL_DESC& Out_Desc, _fmatrix In_ModelTransformMatrix);
	}
}