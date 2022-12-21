#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

struct MESH_DATA;
class CModel;
class CBoneNode;
class CShader;

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

	_int	Get_UpdatedBoneIndex() const { return m_iSwapBoneIndex; }

	

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

public:
	HRESULT Init_Mesh(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel = weak_ptr<CModel>(), const _bool In_bNvCloth = false);
	void SetUp_BoneMatices(_fmatrix TransformationMatrix);
	HRESULT Bind_BoneMatices(weak_ptr<CShader> pShader, const char* pConstantBoneName);
	void Set_NvCloth();

public: /* For. NvCloth */
	void Update_NvClothVertices(ID3D11DeviceContext* pDeviceContext);

private:
	void Update_NvClothVertices_NonAnim(ID3D11DeviceContext* pDeviceContext);
	void Update_NvClothVertices_Anim(ID3D11DeviceContext* pDeviceContext);

private:
	HRESULT Ready_VertexBuffer_NonAnim(shared_ptr<MESH_DATA> tMeshData);
	HRESULT Ready_VertexBuffer_NonAnim_NvCloth(shared_ptr<MESH_DATA> tMeshData);
	HRESULT Ready_VertexBuffer_Anim(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel);
	HRESULT Ready_VertexBuffer_Anim_NvCloth(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel);
	
	HRESULT Ready_IndexBuffer(shared_ptr<MESH_DATA> tMeshData);
	HRESULT Ready_IndexBuffer_NonAnim_NvCloth(shared_ptr<MESH_DATA> tMeshData);
	HRESULT Ready_IndexBuffer_Anim_NvCloth(shared_ptr<MESH_DATA> tMeshData);

private:
	string								m_szName;
	weak_ptr<MESH_DATA>					m_pMeshData;

	_uint								m_iMaterialIndex = 0;
	_uint								m_iNumBones = 0;

	_float4x4							m_BoneMatrices[2][256];
	

	_int								m_iSwapBoneIndex = -1;


	vector<weak_ptr<CBoneNode>>			m_pBoneNodes;

	_float4x4							m_ModelTransform;
	_bool								m_bSimulation = false;


private: /* For. NvCloth */
	Fabric*								m_pFabric{};
	Cloth*								m_pCloth{};
	PhaseConfig*						m_pPhases{};
	//PxVec3*								m_pVertices{};
	vector<_float3>						m_pPosVertices;
	vector<VTXMODEL>					m_pModelVertices;
	vector<VTXANIM>						m_pAnimVertices;
	vector<FACEINDICES32>				m_pIndices;
	vector<_float>						m_pInvMasses;
	vector<PxVec4>						m_pParticles;

public:
	void Free();
};

END