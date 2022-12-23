#include "stdafx.h"
#include "Preset_NvClothMesh.h"

void Preset::NvClothMesh::CorvusSetting(CModel::NVCLOTH_MODEL_DESC& Out_Desc)
{
	Out_Desc;
}

void Preset::NvClothMesh::VargWeaponSetting(CModel::NVCLOTH_MODEL_DESC& Out_Desc)
{

	CModel::NVCLOTH_MESH_DESC MeshDesc;

	MeshDesc.eSimpleAttachType = SELECTION_TYPE::FORWARD;
	MeshDesc.fSimpleAttachRatio = 0.1f;
	MeshDesc.vGravity = _float3(0.f, -9.81f, 0.f);
	MeshDesc.fDragCoefficient = 0.1f;
	MeshDesc.fLiftCoefficient = 0.2f;

	Out_Desc.NvClothMeshDescs[1] = MeshDesc;
}

void Preset::NvClothMesh::VargSetting(CModel::NVCLOTH_MODEL_DESC& Out_Desc)
{
	CModel::NVCLOTH_MESH_DESC MeshDesc;

	MeshDesc.eSimpleAttachType = SELECTION_TYPE::FORWARD;
	MeshDesc.fSimpleAttachRatio = 0.1f;
	MeshDesc.vGravity = _float3(0.f, -9.81f, 0.f);
	MeshDesc.fDragCoefficient = 0.1f;
	MeshDesc.fLiftCoefficient = 0.2f;

	Out_Desc.NvClothMeshDescs[1] = MeshDesc;

}
