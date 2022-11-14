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
	_uint Get_Stride() const
	{
		return m_iStride;
	}
	ComPtr<ID3D11Buffer> Get_VertexBuffer()
	{
		return m_pVB;
	}
	ComPtr<ID3D11Buffer> Get_IndexBuffer()
	{
		return m_pIB;
	}
	_uint Get_NumVertexBuffers() const
	{
		return m_iNumVertexBuffers;
	}
	_uint Get_NumIndices() const
	{
		return m_iNumIndices;
	}
	D3D11_PRIMITIVE_TOPOLOGY Get_Topology() const
	{
		return m_eToplogy;
	}
	DXGI_FORMAT Get_IndexFormat() const
	{
		return m_eIndexFormat;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

public:
	HRESULT Init_Mesh(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel = weak_ptr<CModel>());
	//HRESULT SetUp_BonesPtr(weak_ptr<CModel> pModel);
	void SetUp_BoneMatices(_float4x4* pBoneMatrices, _fmatrix TransformationMatrix);
	_bool Picking() const;

private:
	HRESULT Ready_VertexBuffer_NonAnim(shared_ptr<MESH_DATA> tMeshData);
	HRESULT Ready_VertexBuffer_Anim(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel);
	

private:
	string								m_szName;
	weak_ptr<MESH_DATA>					m_pMeshData;

	_uint								m_iMaterialIndex = 0;
	_uint								m_iNumBones = 0;


	vector<weak_ptr<CBoneNode>>			m_pBoneNodes;

public:
	void Free();
};

END