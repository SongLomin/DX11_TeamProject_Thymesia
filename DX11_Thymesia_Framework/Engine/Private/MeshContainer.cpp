#include "MeshContainer.h"
#include "ModelData.h"
#include "BoneNode.h"
#include "Model.h"
#include "Shader.h"
#include "NvCloth_Manager.h"
#include <set>

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

HRESULT CMeshContainer::Init_Mesh(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel, const void* In_pNvClothMeshDesc)
{
	m_pMeshData = tMeshData;
	m_szName    = tMeshData->szName;

	if (In_pNvClothMeshDesc)
	{
		const CModel::NVCLOTH_MESH_DESC* pCustomDesc = (const CModel::NVCLOTH_MESH_DESC*)In_pNvClothMeshDesc;
		m_pNvCloth_Mesh_Desc = *pCustomDesc;
	}

#pragma region VERTEXBUFFER

	m_iMaterialIndex = tMeshData->iMaterialIndex;

	if (pModel.lock())
	{
		m_ModelTransform = pModel.lock()->Get_ModelData().lock()->TransformMatrix;
	}
	else
	{
		XMStoreFloat4x4(&m_ModelTransform, XMMatrixIdentity());
	}
	

	HRESULT		hr = 0;

	if (MODEL_TYPE::NONANIM == tMeshData->eModelType)
	{
		if (In_pNvClothMeshDesc)
		{
			hr = Ready_VertexBuffer_NonAnim_NvCloth(tMeshData);
		}
		else
		{
			hr = Ready_VertexBuffer_NonAnim(tMeshData);
		}
	}
		
	else
	{
		if (In_pNvClothMeshDesc)
		{
			hr = Ready_VertexBuffer_Anim_NvCloth(tMeshData, pModel);
		}
		else
		{
			hr = Ready_VertexBuffer_Anim(tMeshData, pModel);
		}
	}
		

	if (FAILED(hr))
		DEBUG_ASSERT;
#pragma endregion

#pragma region INDEXBUFFER
	
	if (In_pNvClothMeshDesc)
	{
		m_pIndices.resize(tMeshData->iNumFaces);

		for (PxU32 i = 0; i < tMeshData->iNumFaces; ++i)
		{
			memcpy(&m_pIndices[i], &m_pMeshData.lock()->pIndices[i], sizeof(FACEINDICES32));
		}

		if (MODEL_TYPE::NONANIM == tMeshData->eModelType)
		{
			Ready_IndexBuffer_NonAnim_NvCloth(tMeshData);
		}
		else
		{
			Ready_IndexBuffer_Anim_NvCloth(tMeshData);
		}
	}
	else
	{
		Ready_IndexBuffer(tMeshData);
	}



	if (FAILED(Create_IndexBuffer()))
		DEBUG_ASSERT;


	if (In_pNvClothMeshDesc)
	{
		Set_NvCloth(In_pNvClothMeshDesc);
	}

	return S_OK;
}

HRESULT CMeshContainer::Ready_VertexBuffer_NonAnim(shared_ptr<MESH_DATA> tMeshData)
{
	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = tMeshData->iNumVertices;
	
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;

	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.MiscFlags = 0;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = tMeshData->pVertices.get();

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;


	return S_OK;
}

HRESULT CMeshContainer::Ready_VertexBuffer_NonAnim_NvCloth(shared_ptr<MESH_DATA> tMeshData)
{
	m_pModelVertices.push_back(tMeshData->pVertices[0]);

	for (_int i = 1; i < tMeshData->iNumVertices; ++i)
	{
		_vector vVertice = XMLoadFloat3(&tMeshData->pVertices[i].vPosition);
		_bool bFlag = false;
		for (_int j = 0; j < m_pModelVertices.size(); ++j)
		{
			_vector vTemp = XMLoadFloat3(&m_pModelVertices[j].vPosition);

			_float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
			if (fLength <= DBL_EPSILON)
			{
				bFlag = true;
				break;
			}
		}

		if (!bFlag)
		{
			m_pModelVertices.push_back(tMeshData->pVertices[i]);
		}

	}


	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = m_pModelVertices.size();

	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;

	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.MiscFlags = 0;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = &m_pModelVertices[0];

	for (_int i = 0; i < m_iNumVertices; ++i)
	{
		m_pPosVertices.push_back(m_pModelVertices[i].vPosition);
	}

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	

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
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.MiscFlags = 0;

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

HRESULT CMeshContainer::Ready_VertexBuffer_Anim_NvCloth(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel)
{
	m_pAnimVertices.push_back(tMeshData->pAnimVertices[0]);

	for (_int i = 1; i < tMeshData->iNumVertices; ++i)
	{
		_vector vVertice = XMLoadFloat3(&tMeshData->pAnimVertices[i].vPosition);
		_bool bFlag = false;
		for (_int j = 0; j < m_pAnimVertices.size(); ++j)
		{
			_vector vTemp = XMLoadFloat3(&m_pAnimVertices[j].vPosition);

			_float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
			if (fLength <= DBL_EPSILON)
			{
				bFlag = true;
				break;
			}
		}

		if (!bFlag)
		{
			m_pAnimVertices.push_back(tMeshData->pAnimVertices[i]);
		}

	}

	m_iStride = sizeof(VTXANIM);
	m_iNumVertices = m_pAnimVertices.size();
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;

	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.MiscFlags = 0;

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
	m_SubResourceData.pSysMem = &m_pAnimVertices[0];

	/*for (_int i = 0; i < m_iNumVertices; ++i)
	{
		_vector vPosVerticeToVector = XMLoadFloat3(&m_pAnimVertices[i].vPosition);

		_float	fWeightW = 1.f - (m_pAnimVertices[i].vBlendWeight.x + m_pAnimVertices[i].vBlendWeight.y + m_pAnimVertices[i].vBlendWeight.z);

		_matrix BoneMatrixFromIndex[4];
		BoneMatrixFromIndex[0] = XMLoadFloat4x4(&m_BoneMatrices[m_pAnimVertices[i].vBlendIndex.x]);
		BoneMatrixFromIndex[1] = XMLoadFloat4x4(&m_BoneMatrices[m_pAnimVertices[i].vBlendIndex.y]);
		BoneMatrixFromIndex[2] = XMLoadFloat4x4(&m_BoneMatrices[m_pAnimVertices[i].vBlendIndex.z]);
		BoneMatrixFromIndex[3] = XMLoadFloat4x4(&m_BoneMatrices[m_pAnimVertices[i].vBlendIndex.w]);

		_matrix	BoneMatrix = BoneMatrixFromIndex[0] * m_pAnimVertices[i].vBlendWeight.x +
			BoneMatrixFromIndex[1] * m_pAnimVertices[i].vBlendWeight.y +
			BoneMatrixFromIndex[2] * m_pAnimVertices[i].vBlendWeight.z +
			BoneMatrixFromIndex[3] * fWeightW;

		vPosVerticeToVector = XMVector3TransformCoord(vPosVerticeToVector, BoneMatrix);

		_float3	vPosVertice;
		XMStoreFloat3(&vPosVertice, vPosVerticeToVector);

		m_pPosVertices.push_back(vPosVertice);
	}*/


	for (_int i = 0; i < m_iNumVertices; ++i)
	{
		//XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f)
		_float3 vPos = m_pAnimVertices[i].vPosition;
		if (m_pNvCloth_Mesh_Desc.m_bUseBoneMatrix)
		{
			_vector vPosWithModelTransform = XMVector3TransformCoord(XMLoadFloat3(&m_pAnimVertices[i].vPosition), XMLoadFloat4x4(&m_ModelTransform));
			XMStoreFloat3(&vPos, vPosWithModelTransform);
		}
		
		m_pPosVertices.push_back(vPos);
	}
	

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshContainer::Ready_IndexBuffer(shared_ptr<MESH_DATA> tMeshData)
{
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

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = tMeshData->pIndices.get();

	return S_OK;
}

HRESULT CMeshContainer::Ready_IndexBuffer_NonAnim_NvCloth(shared_ptr<MESH_DATA> tMeshData)
{
	

	for (_int i = 0; i < tMeshData->iNumFaces; ++i)
	{
		_vector vVertice = XMLoadFloat3(&tMeshData->pVertices[m_pIndices[i]._1].vPosition);

		for (_int j = 0; j < m_pModelVertices.size(); ++j)
		{
			_vector vTemp = XMLoadFloat3(&m_pModelVertices[j].vPosition);

			_float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
			if (DBL_EPSILON > fLength)
			{

				// Do. Somthing

				m_pIndices[i]._1 = j;
				break;
			}
		}

		vVertice = XMLoadFloat3(&tMeshData->pVertices[m_pIndices[i]._2].vPosition);

		for (_int j = 0; j < m_pModelVertices.size(); ++j)
		{
			_vector vTemp = XMLoadFloat3(&m_pModelVertices[j].vPosition);

			_float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
			if (DBL_EPSILON > fLength)
			{

				// Do. Somthing

				m_pIndices[i]._2 = j;
				break;
			}
		}

		vVertice = XMLoadFloat3(&tMeshData->pVertices[m_pIndices[i]._3].vPosition);

		for (_int j = 0; j < m_pModelVertices.size(); ++j)
		{
			_vector vTemp = XMLoadFloat3(&m_pModelVertices[j].vPosition);

			_float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
			if (DBL_EPSILON > fLength)
			{

				// Do. Somthing

				m_pIndices[i]._3 = j;
				break;
			}
		}

	}

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

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = &m_pIndices[0];

	return S_OK;
}

HRESULT CMeshContainer::Ready_IndexBuffer_Anim_NvCloth(shared_ptr<MESH_DATA> tMeshData)
{

	for (_int i = 0; i < tMeshData->iNumFaces; ++i)
	{
		_vector vVertice = XMLoadFloat3(&tMeshData->pAnimVertices[m_pIndices[i]._1].vPosition);

		for (_int j = 0; j < m_pAnimVertices.size(); ++j)
		{
			_vector vTemp = XMLoadFloat3(&m_pAnimVertices[j].vPosition);

			_float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
			if (DBL_EPSILON > fLength)
			{

				// Do. Somthing

				m_pIndices[i]._1 = j;
				break;
			}
		}

		vVertice = XMLoadFloat3(&tMeshData->pAnimVertices[m_pIndices[i]._2].vPosition);

		for (_int j = 0; j < m_pAnimVertices.size(); ++j)
		{
			_vector vTemp = XMLoadFloat3(&m_pAnimVertices[j].vPosition);

			_float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
			if (DBL_EPSILON > fLength)
			{

				// Do. Somthing

				m_pIndices[i]._2 = j;
				break;
			}
		}

		vVertice = XMLoadFloat3(&tMeshData->pAnimVertices[m_pIndices[i]._3].vPosition);

		for (_int j = 0; j < m_pAnimVertices.size(); ++j)
		{
			_vector vTemp = XMLoadFloat3(&m_pAnimVertices[j].vPosition);

			_float fLength = XMVector3Length(vTemp - vVertice).m128_f32[0];
			if (DBL_EPSILON > fLength)
			{

				// Do. Somthing

				m_pIndices[i]._3 = j;
				break;
			}
		}

	}

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

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = &m_pIndices[0];

	return S_OK;
}

_vector CMeshContainer::Software_Skinning(const _int In_iVtxAnimIndex)
{
	VTXANIM& AnimVertice = m_pAnimVertices[In_iVtxAnimIndex];

	return Software_Skinning(XMLoadFloat3(&m_pAnimVertices[In_iVtxAnimIndex].vPosition), In_iVtxAnimIndex);
}

_vector CMeshContainer::Software_Skinning(_fvector In_vPosition, const _int In_iVtxAnimIndex)
{
	VTXANIM& AnimVertice = m_pAnimVertices[In_iVtxAnimIndex];

	_vector vPosVerticeToVector = In_vPosition;

	vPosVerticeToVector = XMVectorSetW(vPosVerticeToVector, 1.f);

	if (m_iSwapBoneIndex < 0)
	{
		return vPosVerticeToVector;
	}

	_float	fWeightW = 1.f - (AnimVertice.vBlendWeight.x + AnimVertice.vBlendWeight.y + AnimVertice.vBlendWeight.z);

	_matrix BoneMatrixFromIndex[4];
	BoneMatrixFromIndex[0] = XMMatrixTranspose(XMLoadFloat4x4(&m_BoneMatrices[AnimVertice.vBlendIndex.x]));
	BoneMatrixFromIndex[1] = XMMatrixTranspose(XMLoadFloat4x4(&m_BoneMatrices[AnimVertice.vBlendIndex.y]));
	BoneMatrixFromIndex[2] = XMMatrixTranspose(XMLoadFloat4x4(&m_BoneMatrices[AnimVertice.vBlendIndex.z]));
	BoneMatrixFromIndex[3] = XMMatrixTranspose(XMLoadFloat4x4(&m_BoneMatrices[AnimVertice.vBlendIndex.w]));

	_matrix	BoneMatrix = BoneMatrixFromIndex[0] * AnimVertice.vBlendWeight.x +
		BoneMatrixFromIndex[1] * AnimVertice.vBlendWeight.y +
		BoneMatrixFromIndex[2] * AnimVertice.vBlendWeight.z +
		BoneMatrixFromIndex[3] * fWeightW;

	vPosVerticeToVector = XMVector3TransformCoord(vPosVerticeToVector, BoneMatrix);

	return vPosVerticeToVector;
}

void CMeshContainer::SetUp_BoneMatices(_fmatrix TransformationMatrix)
{
	if (m_iNumBones > 256)
		DEBUG_ASSERT;

	_uint		iIndex = 0;

	for (auto& pBone : m_pBoneNodes)
	{
		XMStoreFloat4x4(&m_BoneMatrices[iIndex++], XMMatrixTranspose(pBone.lock()->Get_OffsetMatrix() * pBone.lock()->Get_CombinedMatrix() * TransformationMatrix));
	}

	m_iSwapBoneIndex = 1;
}

HRESULT CMeshContainer::Bind_BoneMatices(weak_ptr<CShader> pShader, const char* pConstantBoneName)
{
	// 아직 뼈가 업데이트되지 않음.
	if (-1 == m_iSwapBoneIndex)
	{
		return E_FAIL;
	}

	HRESULT hr = pShader.lock()->Set_RawValue(pConstantBoneName, &m_BoneMatrices, sizeof(_float4x4) * 256);

	if (E_FAIL == hr)
	{
		DEBUG_ASSERT;
	}

	return hr;
}

void CMeshContainer::Set_NvCloth(const void* In_pNvClothMeshDesc)
{
	
	switch (m_pNvCloth_Mesh_Desc.eSimpleAttachType)
	{
	case SELECTION_TYPE::FORWARD:
	{
		m_pInvMasses = vector<_float>(m_iNumVertices, m_pNvCloth_Mesh_Desc.fSimpleInvMess);

		_int iAttachCount = (_int)(m_iNumVertices * m_pNvCloth_Mesh_Desc.fSimpleAttachRatio);

		for (_int i = 0; i < iAttachCount; ++i)
		{
			m_pInvMasses[i] = 0.f;
		}
	}
		break;

	case SELECTION_TYPE::REVERSE:
	{
		m_pInvMasses = vector<_float>(m_iNumVertices, m_pNvCloth_Mesh_Desc.fSimpleInvMess);

		_int iAttachCount = (_int)(m_iNumVertices * m_pNvCloth_Mesh_Desc.fSimpleAttachRatio);

		for (_int i = 0; i < iAttachCount; ++i)
		{
			m_pInvMasses[(m_iNumVertices - 1) - i] = 0.f;
		}
	}
		break;

	default:
	{
		m_pInvMasses = m_pNvCloth_Mesh_Desc.InvMesses;
	}
		break;
	}


	nv::cloth::ClothMeshDesc meshDesc;	
	//Fill meshDesc with data
	meshDesc.setToDefault();
	//meshDesc.flags &= ~MeshFlag::e16_BIT_INDICES;

	if (meshDesc.flags & MeshFlag::e16_BIT_INDICES)
		DEBUG_ASSERT;

	meshDesc.points.count = m_iNumVertices;
	meshDesc.points.stride = sizeof(_float3);
	meshDesc.points.data = &m_pPosVertices[0];

	
	
	meshDesc.triangles.count = m_pIndices.size();
	meshDesc.triangles.stride = sizeof(FACEINDICES32);
	meshDesc.triangles.data = &m_pIndices[0];
	//etc. for quads, triangles and invMasses

	meshDesc.invMasses.count = m_pInvMasses.size();
	meshDesc.invMasses.stride = sizeof(_float);
	meshDesc.invMasses.data = &m_pInvMasses[0];

	PxVec3 gravity(0.0f, -1.f, 0.0f);
	nv::cloth::Vector<int32_t>::Type phaseTypeInfo;

	m_pFabric = NvClothCookFabricFromMesh(GET_SINGLE(CNvCloth_Manager)->Get_Factory(), meshDesc, gravity, &phaseTypeInfo, false);
	

	_uint iFNumIndices = m_pFabric->getNumIndices();
	_uint iFNumParticle = m_pFabric->getNumParticles();

	
	m_pParticles.resize(m_pFabric->getNumParticles());

	//physx::PxMat44 transform = PxTransform(PxVec3(-2.f, 13.f, 0.f), PxQuat(PxPi / 6.f, PxVec3(1.f, 0.f, 0.f)));
	//physx::PxVec3 center = transform.transform(physx::PxVec3(0.0f, 0.0f, 0.0f));

	for (_int i = 0; i < m_iNumVertices; ++i)
	{
		// 터짐
		//m_pParticles[i].setZero();
		
		// 망토가 이상한 위치에 있음.
		//m_pParticles[i] = physx::PxVec4((m_pVertices[i] - center) * 0.85f + center, 1.f / 5.f);

		// 정점 위치에서 시작하면 어떻게 되지?
		// 똑같다.
		m_pParticles[i] = physx::PxVec4(SMath::Convert_PxVec3(m_pPosVertices[i]), m_pInvMasses[i]);

		//m_pParticles[i].w = 0.f;
		//particlesCopy[i].w = 1.f / 5.f;
	}

	//  and can be to 0 to lock the particle / make it static.
	m_pCloth = GET_SINGLE(CNvCloth_Manager)->Get_Factory()->createCloth(nv::cloth::Range<physx::PxVec4>(&m_pParticles[0], &m_pParticles[0] + m_pParticles.size()), *m_pFabric);

	_uint iCNumTriangles = m_pCloth->getNumTriangles();
	_uint iCNumVertices = m_pCloth->getNumParticles();


	//_vector vGravity = XMVectorSet(0.f, -9.81f, 0.f, 0.f);
	//XMVector3TransformCoord(vGravity, XMLoadFloat4x4(&m_ModelTransform));
	
	m_pCloth->setGravity(SMath::Convert_PxVec3(m_pNvCloth_Mesh_Desc.vGravity));
	//m_pCloth->setDamping(physx::PxVec3(0.1f, 0.1f, 0.1f));
	//m_pCloth->setSelfCollisionDistance(0.07f);

	m_pPhases = DBG_NEW nv::cloth::PhaseConfig[m_pFabric->getNumPhases()];
	for (int i = 0; i < m_pFabric->getNumPhases(); i++)
	{
		m_pPhases[i].mPhaseIndex = i; // Set index to the corresponding set (constraint group)

		//Give phases different configs depending on type
		switch (phaseTypeInfo[i])
		{
		case nv::cloth::ClothFabricPhaseType::eINVALID:
			//ERROR
			break;
		case nv::cloth::ClothFabricPhaseType::eVERTICAL:
			break;
		case nv::cloth::ClothFabricPhaseType::eHORIZONTAL:
			break;
		case nv::cloth::ClothFabricPhaseType::eBENDING:
			break;
		case nv::cloth::ClothFabricPhaseType::eSHEARING:
			break;
		}

		//For this example we give very phase the same config
		m_pPhases[i].mStiffness = 1.0f;
		m_pPhases[i].mStiffnessMultiplier = 1.f;
		m_pPhases[i].mCompressionLimit = 1.f;
		m_pPhases[i].mStretchLimit = 1.f;
	}
	m_pCloth->setPhaseConfig(nv::cloth::Range<nv::cloth::PhaseConfig>(m_pPhases, m_pPhases + m_pFabric->getNumPhases()));

	m_pCloth->setDamping(PxVec3(0.5f, 0.5f, 0.5f));
	m_pCloth->setFluidDensity(1.f);
	GET_SINGLE(CNvCloth_Manager)->Get_Solver()->addCloth(m_pCloth);

}

_size_t CMeshContainer::Get_NvVertexCount()
{
	return m_pPosVertices.size();
}

vector<_float>& CMeshContainer::Get_InvMesses()
{
	return m_pInvMasses;
}

void CMeshContainer::Update_NvClothVertices(ID3D11DeviceContext* pDeviceContext, _fmatrix In_WorldMatrix, _fvector In_Gravity)
{
	if (!m_pCloth)
		return;

	m_pCloth->setGravity(SMath::Convert_PxVec3(In_Gravity));

	/*_vector vPos = In_WorldMatrix.r[3];
	_vector vQuaternion = XMQuaternionRotationMatrix(SMath::Get_RotationMatrix(In_WorldMatrix));

	m_pCloth->setTranslation(SMath::Convert_PxVec3(vPos));
	m_pCloth->setRotation(SMath::Convert_PxQuat(vQuaternion));*/

	MappedRange<PxVec4> particle = m_pCloth->getCurrentParticles();
	_vector vTest = SMath::Convert_Vector(particle[0].getXYZ());

	if (!m_bSimulation)
	{
		m_bSimulation = true;
		m_pCloth->clearInertia();

		return;
	}

	if (!m_bTwoSimulation)
	{
		

		m_bTwoSimulation = true;

		

		//const CModel::NVCLOTH_MESH_DESC* pCustomDesc = (const CModel::NVCLOTH_MESH_DESC*)m_pNvCloth_Mesh_Desc;

		if (m_pNvCloth_Mesh_Desc.fDragCoefficient > DBL_EPSILON)
		{
			m_pCloth->setDragCoefficient(m_pNvCloth_Mesh_Desc.fDragCoefficient);
		}
		if (m_pNvCloth_Mesh_Desc.fLiftCoefficient > DBL_EPSILON)
		{
			m_pCloth->setLiftCoefficient(m_pNvCloth_Mesh_Desc.fLiftCoefficient);
		}
		
	}

	if (MODEL_TYPE::ANIM == m_pMeshData.lock()->eModelType)
	{
		Update_NvClothVertices_Anim(pDeviceContext, In_WorldMatrix);
	}
	else
	{
		Update_NvClothVertices_NonAnim(pDeviceContext, In_WorldMatrix);
	}

}

void CMeshContainer::Update_NvClothCollisionSpheres(Range<const physx::PxVec4> spheres, uint32_t first, uint32_t last)
{
	if (!m_pCloth)
		return;

	m_pCloth->setSpheres(spheres, first, last);
}

void CMeshContainer::Update_InvMesses()
{
	MappedRange<PxVec4> particle = m_pCloth->getCurrentParticles();

	for (_size_t i = 0; i < m_pInvMasses.size(); ++i)
	{
		particle[i].w = m_pInvMasses[i];
	}
}

void CMeshContainer::Update_NvClothVertices_Anim(ID3D11DeviceContext* pDeviceContext, _fmatrix In_WorldMatrix)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	MappedRange<PxVec4> particle = m_pCloth->getCurrentParticles();

	pDeviceContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	if (m_pNvCloth_Mesh_Desc.m_bUseBoneMatrix)
	{
		for (_int i = 0; i < m_iNumVertices; ++i)
		{
			_vector vNewPos = SMath::Convert_Vector(particle[i].getXYZ());

			if (particle[i].w <= DBL_EPSILON)
			{
				_vector vAttachPoint;

				vAttachPoint = Software_Skinning(i);

				vAttachPoint = XMVector3TransformCoord(vAttachPoint, In_WorldMatrix);
				particle[i] = PxVec4(SMath::Convert_PxVec3(vAttachPoint), particle[i].w);
				 
				vNewPos = vAttachPoint;
			}

			memcpy(&((VTXANIM*)SubResource.pData)[i], &m_pAnimVertices[i], sizeof(VTXANIM));
			
			//vNewPos = Software_Skinning(vNewPos, i);
			//vNewPos = XMVector3TransformCoord(vNewPos, In_WorldMatrix);
			

			XMStoreFloat3(&((VTXANIM*)SubResource.pData)[i].vPosition, vNewPos);
		}
	}
	else
	{
		for (_int i = 0; i < m_iNumVertices; ++i)
		{
			memcpy(&((VTXANIM*)SubResource.pData)[i], &m_pAnimVertices[i], sizeof(VTXANIM));
			_vector vNewPos = SMath::Convert_Vector(particle[i].getXYZ());
			XMStoreFloat3(&((VTXANIM*)SubResource.pData)[i].vPosition, vNewPos);

			if (particle[i].w < DBL_EPSILON)
			{
				_vector vAttachPoint;

				vAttachPoint = XMLoadFloat3(&m_pAnimVertices[i].vPosition);
				vAttachPoint = XMVectorSetW(vAttachPoint, 1.f);

				vAttachPoint = XMVector3TransformCoord(vAttachPoint, In_WorldMatrix);
				particle[i] = PxVec4(SMath::Convert_PxVec3(vAttachPoint), particle[i].w);
			}
		}
	}


	

	

	pDeviceContext->Unmap(m_pVB.Get(), 0);
}

void CMeshContainer::Update_NvClothVertices_NonAnim(ID3D11DeviceContext* pDeviceContext, _fmatrix In_WorldMatrix)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	MappedRange<PxVec4> particle = m_pCloth->getCurrentParticles();

	pDeviceContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	for (_int i = 0; i < m_iNumVertices; ++i) 
	{
		memcpy(&((VTXMODEL*)SubResource.pData)[i], &m_pModelVertices[i], sizeof(VTXMODEL));
		_vector vNewPos = SMath::Convert_Vector(particle[i].getXYZ());
		XMStoreFloat3(&((VTXMODEL*)SubResource.pData)[i].vPosition, vNewPos);

		if (particle[i].w < DBL_EPSILON)
		{
			_vector vAttachPoint = XMVector3TransformCoord(XMLoadFloat3(&m_pModelVertices[i].vPosition), In_WorldMatrix);
			particle[i] = PxVec4(SMath::Convert_PxVec3(vAttachPoint), particle[i].w);
		}
	}

	pDeviceContext->Unmap(m_pVB.Get(), 0);
}

void CMeshContainer::Free()
{
	if(m_pCloth)
		GET_SINGLE(CNvCloth_Manager)->Get_Solver()->removeCloth(m_pCloth);

	if(m_pFabric)
		m_pFabric->decRefCount();

	if (m_pCloth)
		Safe_Delete(m_pCloth);

	if(m_pPhases)
		Safe_Delete_Array(m_pPhases);


	/*if(m_pVertices)
		Safe_Delete_Array(m_pVertices);*/

	/*if (m_pInvMasses)
		Safe_Delete_Array(m_pInvMasses);*/

	/*if(m_pIndices)
		Safe_Delete_Array(m_pIndices);*/
}
