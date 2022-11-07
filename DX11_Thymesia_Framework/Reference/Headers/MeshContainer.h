#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

struct MESH_DATA;
class CModel;
class CBoneNode;

class ENGINE_DLL CMeshContainer : public CVIBuffer
{
	GAMECLASS_H(CMeshContainer);
	CLONE_H(CMeshContainer, CComponent);

public:
	CMeshContainer(const CMeshContainer& rhs);

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

public:
	HRESULT Init_Mesh(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel);
	//HRESULT SetUp_BonesPtr(weak_ptr<CModel> pModel);
	void SetUp_BoneMatices(_float4x4* pBoneMatrices, _fmatrix TransformationMatrix);

private:
	HRESULT Ready_VertexBuffer_NonAnim(shared_ptr<MESH_DATA> tMeshData);
	HRESULT Ready_VertexBuffer_Anim(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel);
	

private:
	string								m_szName;

	_uint								m_iMaterialIndex = 0;

	_uint								m_iNumBones = 0;

	weak_ptr<MESH_DATA>					m_pMeshData;

	vector<weak_ptr<CBoneNode>>			m_pBoneNodes;

public:
	virtual void Free() override;
};

END