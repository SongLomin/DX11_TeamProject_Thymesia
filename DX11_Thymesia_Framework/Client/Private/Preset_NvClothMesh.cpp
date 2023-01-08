#include "stdafx.h"
#include "Preset_NvClothMesh.h"


void Preset::NvClothMesh::CorvusSetting(CModel::NVCLOTH_MODEL_DESC& Out_Desc)
{
	CModel::NVCLOTH_MESH_DESC MeshDesc;

	_vector vGravity = XMVectorSet(0.f, -9.81f, 0.f, 0.f);
	MeshDesc.InvMesses = vector<_float>(190, 0.001f);

	_int iAttachCount = 10;

	for (_int i = 0; i < iAttachCount; ++i)
	{
		MeshDesc.InvMesses[(190 - 1) - i] = 0.f;
	}

	//iAttachCount = (_int)(95 * 0.1f);

	// 190°³ÀÇ 10% = 19°³.
	for (_int i = 94 - iAttachCount + 1; i <= 94; ++i)
	{
		MeshDesc.InvMesses[i] = 0.f;
	}


	//190
	MeshDesc.eSimpleAttachType = SELECTION_TYPE::TYPE_END;
	//MeshDesc.fSimpleAttachRatio = 0.1f;
	//MeshDesc.fSimpleInvMess = 0.1f;
	MeshDesc.m_bUseBoneMatrix = true;
	XMStoreFloat3(&MeshDesc.vGravity, vGravity);
	//MeshDesc.fDragCoefficient = 1.f;
	//MeshDesc.fLiftCoefficient = 0.5f;

	Out_Desc.NvClothMeshDescs[2] = MeshDesc;
}

void Preset::NvClothMesh::VargWeaponSetting(CModel::NVCLOTH_MODEL_DESC& Out_Desc)
{

	CModel::NVCLOTH_MESH_DESC MeshDesc;

	MeshDesc.eSimpleAttachType = SELECTION_TYPE::FORWARD;
	MeshDesc.fSimpleAttachRatio = 0.3f;
	MeshDesc.vGravity = _float3(0.f, -9.81f, 0.f);
	MeshDesc.fDragCoefficient = 0.1f;
	MeshDesc.fLiftCoefficient = 0.2f;

	Out_Desc.NvClothMeshDescs[1] = MeshDesc;
}

void Preset::NvClothMesh::VargSetting(CModel::NVCLOTH_MODEL_DESC& Out_Desc, _fmatrix In_ModelTransformMatrix)
{
	CModel::NVCLOTH_MESH_DESC MeshDesc;

	_vector vGravity = XMVectorSet(0.f, -9.81f, 0.f, 0.f);
	//vGravity = XMVector3TransformNormal(vGravity, In_ModelTransformMatrix);

	MeshDesc.InvMesses = vector<_float>(2220, 0.001f);

	/*for (_size_t i = 0; i < MeshDesc.InvMesses.size(); i += (_size_t)(MeshDesc.InvMesses.size() / 111))
	{
		MeshDesc.InvMesses[i] = 0.f;
	}*/

	MeshDesc.InvMesses[2219] = 0.f;

	XMStoreFloat3(&MeshDesc.vGravity, vGravity);
	MeshDesc.fDragCoefficient = 1.f;
	//MeshDesc.fLiftCoefficient = 1.f;

	Out_Desc.NvClothMeshDescs[1] = MeshDesc;

	MeshDesc.eSimpleAttachType = SELECTION_TYPE::FORWARD;
	MeshDesc.fSimpleAttachRatio = 0.1f;
	MeshDesc.fSimpleInvMess = 0.001f;
	XMStoreFloat3(&MeshDesc.vGravity, vGravity);
	MeshDesc.fDragCoefficient = 1.f;
	//MeshDesc.fLiftCoefficient = 0.5f;
	
	Out_Desc.NvClothMeshDescs[3] = MeshDesc;

}
