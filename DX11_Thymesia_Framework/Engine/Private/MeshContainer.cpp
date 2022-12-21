#include "MeshContainer.h"
#include "ModelData.h"
#include "BoneNode.h"
#include "Model.h"
#include "Shader.h"
#include "NvCloth_Manager.h"

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

HRESULT CMeshContainer::Init_Mesh(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel, const _bool In_bNvCloth)
{
	m_pMeshData = tMeshData;
	m_szName    = tMeshData->szName;

#pragma region VERTEXBUFFER

	m_iMaterialIndex = tMeshData->iMaterialIndex;

	HRESULT		hr = 0;

	if (MODEL_TYPE::NONANIM == tMeshData->eModelType)
		hr = Ready_VertexBuffer_NonAnim(tMeshData, In_bNvCloth);
	else
		hr = Ready_VertexBuffer_Anim(tMeshData, pModel, In_bNvCloth);

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

	if (In_bNvCloth)
	{
		Set_NvCloth();
	}

	return S_OK;
}

HRESULT CMeshContainer::Ready_VertexBuffer_NonAnim(shared_ptr<MESH_DATA> tMeshData, const _bool In_bNvCloth)
{
	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = tMeshData->iNumVertices;
	//m_iNumVertices = pAIMesh.mNumVertices;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;

	if (In_bNvCloth)
	{
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.CPUAccessFlags = 0;
	}
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.MiscFlags = 0;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = tMeshData->pVertices.get();
	//m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	//Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Ready_VertexBuffer_Anim(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel, const _bool In_bNvCloth)
{
	m_iStride = sizeof(VTXANIM);
	m_iNumVertices = tMeshData->iNumVertices;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	
	if (In_bNvCloth)
	{
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.CPUAccessFlags = 0;
	}
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
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

void CMeshContainer::SetUp_BoneMatices(_fmatrix TransformationMatrix)
{
	if (m_iNumBones > 256)
		return;

	_int iUpdateBoneIndex;

	if (-1 == m_iSwapBoneIndex)
	{
		iUpdateBoneIndex = 0;
	}
	else
	{
		iUpdateBoneIndex = 1 - m_iSwapBoneIndex;
	}
	

	_uint		iIndex = 0;

	for (auto& pBone : m_pBoneNodes)
	{
		XMStoreFloat4x4(&m_BoneMatrices[iUpdateBoneIndex][iIndex++], XMMatrixTranspose(pBone.lock()->Get_OffsetMatrix() * pBone.lock()->Get_CombinedMatrix() * TransformationMatrix));
	}

	m_iSwapBoneIndex = iUpdateBoneIndex;
}

HRESULT CMeshContainer::Bind_BoneMatices(weak_ptr<CShader> pShader, const char* pConstantBoneName)
{
	// 아직 뼈가 업데이트되지 않음.
	if (-1 == m_iSwapBoneIndex)
	{
		return E_FAIL;
	}

	HRESULT hr = pShader.lock()->Set_RawValue(pConstantBoneName, &m_BoneMatrices[m_iSwapBoneIndex], sizeof(_float4x4) * 256);

	if (E_FAIL == hr)
	{
		DEBUG_ASSERT;
	}

	return hr;
}

void CMeshContainer::Set_NvCloth()
{
	PxU32 iNumVertices;
	PxU32 iNumFaces;


	nv::cloth::ClothMeshDesc meshDesc;

	iNumVertices = m_pMeshData.lock()->iNumVertices;
	iNumFaces = m_pMeshData.lock()->iNumFaces;
	m_pVertices = DBG_NEW PxVec3[iNumVertices];
	m_pIndices = DBG_NEW FACEINDICES32[iNumFaces];

	SMath::Convert_PxVec3FromMeshDataWithTransformMatrix(m_pVertices, m_pMeshData, XMMatrixIdentity());

	//Fill meshDesc with data
	meshDesc.setToDefault();
	//meshDesc.flags &= ~MeshFlag::e16_BIT_INDICES;

	meshDesc.points.count = iNumVertices;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = m_pVertices;

	for (PxU32 i = 0; i < iNumFaces; ++i)
	{
		memcpy(&m_pIndices[i], &m_pMeshData.lock()->pIndices[i], sizeof(FACEINDICES32));
	}
	
	meshDesc.triangles.count = iNumFaces;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	meshDesc.triangles.data = m_pIndices;
	//etc. for quads, triangles and invMasses

	PxVec3 gravity(0.0f, -9.8f, 0.0f);
	nv::cloth::Vector<int32_t>::Type phaseTypeInfo;

	m_pFabric = NvClothCookFabricFromMesh(GET_SINGLE(CNvCloth_Manager)->Get_Factory(), meshDesc, gravity, &phaseTypeInfo, false);

	

	
	m_pParticles.resize(m_pFabric->getNumParticles());

	physx::PxMat44 transform = PxTransform(PxVec3(-2.f, 13.f, 0.f), PxQuat(PxPi / 6.f, PxVec3(1.f, 0.f, 0.f)));
	physx::PxVec3 center = transform.transform(physx::PxVec3(0.0f, 0.0f, 0.0f));

	for (_int i = 0; i < iNumVertices; ++i)
	{
		// 터짐
		//m_pParticles[i].setZero();
		
		// 망토가 이상한 위치에 있음.
		//m_pParticles[i] = physx::PxVec4((m_pVertices[i] - center) * 0.85f + center, 1.f / 5.f);

		// 정점 위치에서 시작하면 어떻게 되지?
		// 똑같다.
		m_pParticles[i] = physx::PxVec4(m_pVertices[i], 0.f);

		//m_pParticles[i].w = 0.f;
		//particlesCopy[i].w = 1.f / 5.f;
	}

	//  and can be to 0 to lock the particle / make it static.
	m_pCloth = GET_SINGLE(CNvCloth_Manager)->Get_Factory()->createCloth(nv::cloth::Range<physx::PxVec4>(&m_pParticles[0], &m_pParticles[0] + m_pParticles.size()), *m_pFabric);
	m_pCloth->setGravity(PxVec3(0.f, 9.81f, 0.f));
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
		m_pPhases[i].mStiffnessMultiplier = 1.0f;
		m_pPhases[i].mCompressionLimit = 1.0f;
		m_pPhases[i].mStretchLimit = 1.0f;
	}
	m_pCloth->setPhaseConfig(nv::cloth::Range<nv::cloth::PhaseConfig>(m_pPhases, m_pPhases + m_pFabric->getNumPhases()));
	
	

	GET_SINGLE(CNvCloth_Manager)->Get_Solver()->addCloth(m_pCloth);

}

void CMeshContainer::Update_NvClothVertices(ID3D11DeviceContext* pDeviceContext)
{
	if (!m_pCloth)
		return;

	if (MODEL_TYPE::ANIM == m_pMeshData.lock()->eModelType)
	{
		Update_NvClothVertices_Anim(pDeviceContext);
	}
	else
	{
		Update_NvClothVertices_NonAnim(pDeviceContext);
	}

}

void CMeshContainer::Update_NvClothVertices_NonAnim(ID3D11DeviceContext* pDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	MappedRange<PxVec4> particle = m_pCloth->getCurrentParticles();

	pDeviceContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	for (_int i = 0; i < m_iNumVertices; ++i)
	{
		XMStoreFloat3(&((VTXMODEL*)SubResource.pData)[i].vPosition, SMath::Convert_Vector(particle[i].getXYZ()));
	}

	pDeviceContext->Unmap(m_pVB.Get(), 0);
}

void CMeshContainer::Update_NvClothVertices_Anim(ID3D11DeviceContext* pDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	MappedRange<PxVec4> particle = m_pCloth->getCurrentParticles();
	MappedRange<PxVec4> preparticle = m_pCloth->getPreviousParticles();
	//m_pCloth->get

	pDeviceContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	_vector vPos;

	//Print_Vector(SMath::Convert_Vector(particle[0]));

	for (_int i = 0; i < m_iNumVertices; ++i)
	{
		/*m_pParticles[i].x += particle[i].x - preparticle[i].x;
		m_pParticles[i].y += particle[i].y - preparticle[i].y;
		m_pParticles[i].z += particle[i].z - preparticle[i].z;

		vPos = XMLoadFloat3(&m_pMeshData.lock()->pAnimVertices[i].vPosition);
		vPos += SMath::Convert_Vector(m_pParticles[i]);

		XMStoreFloat3(&((VTXANIM*)SubResource.pData)[i].vPosition, vPos);*/

		XMStoreFloat3(&((VTXANIM*)SubResource.pData)[i].vPosition, SMath::Convert_Vector(particle[i].getXYZ()));
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


	if(m_pVertices)
		Safe_Delete_Array(m_pVertices);

	if(m_pIndices)
		Safe_Delete_Array(m_pIndices);
}
