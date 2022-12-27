#pragma once
#include "Base.h"

typedef struct tag_NvCloth_Mesh_desc
{
	SELECTION_TYPE	eSimpleAttachType = SELECTION_TYPE::TYPE_END;
	_float			fSimpleAttachRatio = 0.f;

	// It is not used when using Simple Attach.
	vector<_float>	InvMesses;

	_float3			vGravity = _float3(0.f, 9.81f, 0.f);
	_float			fDragCoefficient = 0.f;
	_float			fLiftCoefficient = 0.f;
}NVCLOTH_MESH_DESC;

typedef struct tag_NvCloth_Model_desc
{
	map<_uint, tag_NvCloth_Mesh_desc> NvClothMeshDescs;
}NVCLOTH_MODEL_DESC;