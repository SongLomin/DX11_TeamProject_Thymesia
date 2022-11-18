#include "MeshContainer.h"
#include "ModelData.h"
#include "BoneNode.h"
#include "Model.h"

GAMECLASS_C(CMeshContainer)
CLONE_C(CMeshContainer, CComponent)

CMeshContainer::CMeshContainer(const CMeshContainer& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CMeshContainer::Initialize_Prototype()
{
	/*UINT ByteWidth;
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
	UINT StructureByteStride;*/


//#pragma region VERTEXBUFFER
//	m_iStride = sizeof(VTXTEX);
//	m_iNumVertices = 4;
//	m_iNumVertexBuffers = 1;
//
//	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	m_BufferDesc.StructureByteStride = m_iStride;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//
//	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];
//
//	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
//	pVertices[0].vTexUV = _float2(0.0f, 0.f);
//
//	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
//	pVertices[1].vTexUV = _float2(1.0f, 0.f);
//
//	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
//	pVertices[2].vTexUV = _float2(1.0f, 1.f);
//
//	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
//	pVertices[3].vTexUV = _float2(0.0f, 1.f);
//
//	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	m_SubResourceData.pSysMem = pVertices;
//
//	if (FAILED(__super::Create_VertexBuffer()))
//		return E_FAIL;
//
//	Safe_Delete_Array(pVertices);
//#pragma endregion
//
//	m_iIndicesStride = sizeof(FACEINDICES16);
//	m_iNumPrimitive = 2;
//	m_iNumIndices = 3 * m_iNumPrimitive;
//	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
//	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//
//	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	m_BufferDesc.StructureByteStride = 0;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//
//	shared_ptr<FACEINDICES16[]> pIndices(new FACEINDICES16[m_iNumPrimitive]);
//
//	//FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitive];
//	//ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);
//
//	pIndices[0]._1 = 0;
//	pIndices[0]._2 = 1;
//	pIndices[0]._3 = 2;
//
//	pIndices[1]._1 = 0;
//	pIndices[1]._2 = 2;
//	pIndices[1]._3 = 3;
//
//	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	m_SubResourceData.pSysMem = pIndices.get();
//
//	if (FAILED(Create_IndexBuffer()))
//		return E_FAIL;


#pragma endregion
	return S_OK;
}

HRESULT CMeshContainer::Initialize(void* pArg)
{
	return S_OK;
}

void CMeshContainer::Start()
{
}

HRESULT CMeshContainer::Init_Mesh(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel)
{
	m_pMeshData = tMeshData;
	m_szName    = tMeshData->szName;

#pragma region VERTEXBUFFER

	m_iMaterialIndex = tMeshData->iMaterialIndex;

	HRESULT		hr = 0;

	if (MODEL_TYPE::NONANIM == tMeshData->eModelType)
		hr = Ready_VertexBuffer_NonAnim(tMeshData);
	else
		hr = Ready_VertexBuffer_Anim(tMeshData, pModel);

	if (FAILED(hr))
		DEBUG_ASSERT;
#pragma endregion

#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = tMeshData->iNumFaces;
	//m_iNumPrimitive = pAIMesh->mNumFaces;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	/*FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._3 = pAIMesh->mFaces[i].mIndices[2];
	}*/

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = tMeshData->pIndices.get();
	//m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	//Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CMeshContainer::Ready_VertexBuffer_NonAnim(shared_ptr<MESH_DATA> tMeshData)
{
	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = tMeshData->iNumVertices;
	//m_iNumVertices = pAIMesh.mNumVertices;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = tMeshData->pVertices.get();
	//m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	//Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Ready_VertexBuffer_Anim(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel)
{
	m_iStride = sizeof(VTXANIM);
	m_iNumVertices = tMeshData->iNumVertices;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	/*VTXANIM* pVertices = new VTXANIM[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIM) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}*/

	m_iNumBones = tMeshData->iNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		shared_ptr<BONE_DATA> BoneData = tMeshData->Bone_Datas[i];

		_float4x4	OffsetMatrix;

		memcpy(&OffsetMatrix, &BoneData->OffsetMatrix, sizeof(_float4x4));


		weak_ptr<CBoneNode> pBoneNode = pModel.lock()->Find_BoneNode(BoneData->szName);
		if (nullptr == pBoneNode.lock().get())
			DEBUG_ASSERT;

		pBoneNode.lock()->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_pBoneNodes.push_back(pBoneNode);
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		weak_ptr<CBoneNode> pBoneNode = pModel.lock()->Find_BoneNode(m_szName);
		if (nullptr == pBoneNode.lock().get())
			DEBUG_ASSERT;

		m_pBoneNodes.push_back(pBoneNode);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = tMeshData->pAnimVertices.get();

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	return S_OK;
}

//HRESULT CMeshContainer::SetUp_BonesPtr(weak_ptr<CModel> pModel)
//{
//	if (!pModel.lock().get())
//		DEBUG_ASSERT;
//
//	m_iNumBones = m_pMeshData.lock()->iNumBones;
//
//	for (_uint i = 0; i < m_iNumBones; ++i)
//	{
//		aiBone* pAIBone = m_pMeshData.lock()->pAIMesh->mBones[i];
//
//		_float4x4	OffSetMatrix;
//
//		memcpy(&OffSetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
//
//		weak_ptr<CBoneNode> pBoneNode = pModel.lock()->Find_BoneNode(pAIBone->mName.data);
//		if (nullptr == pBoneNode.lock().get())
//			DEBUG_ASSERT;
//
//		pBoneNode.lock()->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffSetMatrix)));
//
//		m_pBoneNodes.push_back(pBoneNode);
//	}
//
//	if (0 == m_iNumBones)
//	{
//		m_iNumBones = 1;
//
//		weak_ptr<CBoneNode> pBoneNode = pModel.lock()->Find_BoneNode(m_szName);
//		if (nullptr == pBoneNode.lock().get())
//			DEBUG_ASSERT;
//
//		m_pBoneNodes.push_back(pBoneNode);
//
//	}
//
//	return S_OK;
//}

void CMeshContainer::SetUp_BoneMatices(_float4x4* pBoneMatrices, _fmatrix TransformationMatrix)
{
	if (m_iNumBones > 256)
		return;


	if (m_szName == "SWORD")
	{
		int i = 0;
	}


	_uint		iIndex = 0;

	for (auto& pBone : m_pBoneNodes)
	{
		if (!pBone.lock().get())
			DEBUG_ASSERT;

		XMStoreFloat4x4(&pBoneMatrices[iIndex++], XMMatrixTranspose(pBone.lock()->Get_OffsetMatrix() * pBone.lock()->Get_CombinedMatrix() * TransformationMatrix));
	}

	int i = 0;
}

void CMeshContainer::Free()
{

}
