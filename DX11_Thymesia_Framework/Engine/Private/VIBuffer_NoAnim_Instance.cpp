#include "VIBuffer_Model_Instance.h"
#include "GameInstance.h"
#include "ModelData.h"

GAMECLASS_C(CVIBuffer_Model_Instance)
CLONE_C(CVIBuffer_Model_Instance, CComponent)

HRESULT CVIBuffer_Model_Instance::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CVIBuffer_Model_Instance::Initialize(void* pArg)
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = 4;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.0f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.0f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.0f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.0f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);


#pragma endregion

#pragma region INDEXBUFFER


#pragma endregion
	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = 2;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

void CVIBuffer_Model_Instance::Start()
{
}

void CVIBuffer_Model_Instance::Init_NoAnimInstance(const char* In_szModelName, const MEMORY_TYPE In_eModelMemoryType, const _uint In_iNoAnimIndex)
{
	weak_ptr<MODEL_DATA> pModelData = GAMEINSTANCE->Get_ModelFromKey(In_szModelName, In_eModelMemoryType);

	m_pNoAnimData = pModelData.lock()->NoAnim_Datas[In_iNoAnimIndex];
	m_szName = pModelData.lock()->szModelFileName;

#pragma region VERTEXBUFFER

	m_iMaterialIndex = m_pNoAnimData.lock()->iMaterialIndex;

	HRESULT		hr = 0;

	if (MODEL_TYPE::NONANIM == tNoAnimData->eModelType)
		hr = Ready_VertexBuffer_NonAnim(tNoAnimData);
	else
		hr = Ready_VertexBuffer_Anim(tNoAnimData, pModel);

	if (FAILED(hr))
		DEBUG_ASSERT;
#pragma endregion

#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = tNoAnimData->iNumFaces;
	//m_iNumPrimitive = pAINoAnim->mNumFaces;
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
	m_SubResourceData.pSysMem = tNoAnimData->pIndices.get();

	if (FAILED(Create_IndexBuffer()))
		DEBUG_ASSERT;


}

void CVIBuffer_Model_Instance::Init_Particle(const _uint In_Size)
{
	if (0 == In_Size)
		return;

	m_pVBInstance.Reset();

#pragma region INSTANCEBUFFER
	m_iInstanceStride = sizeof(VTXCOLORINSTANCE);
	m_iNumInstance = In_Size;


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXCOLORINSTANCE* pInstance = new VTXCOLORINSTANCE[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstance[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstance[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstance[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pInstance[i].vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
		pInstance[i].vColor = _float4(1.f, 0.f, 0.f, 1.f);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance)))
		DEBUG_ASSERT;

	Safe_Delete_Array(pInstance);

#pragma endregion
}

HRESULT CVIBuffer_Model_Instance::Render()
{

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB.Get(),
		m_pVBInstance.Get()
	};

	_uint		iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint		iOffsets[] = {
		0,
		0
	};

	DEVICECONTEXT->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	DEVICECONTEXT->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	DEVICECONTEXT->IASetPrimitiveTopology(m_eToplogy);

	/* 6 : 하나의 도형을 그리기위해 사용하는 인덱스의 갯수. 네모라서 여섯개.  */
	DEVICECONTEXT->DrawIndexedInstanced(6, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Model_Instance::Update(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	/* D3D11_MAP_WRITE_NO_OVERWRITE : SubResource구조체가 받아온 pData에 유요한 값이 담겨잇는 형태로 얻어오낟. */
	/* D3D11_MAP_WRITE_DISCARD : SubResource구조체가 받아온 pData에 값이 초기화된 형태로 얻어오낟. */
	DEVICECONTEXT->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{

		// *(((VTXINSTANCE*)SubResource.pData) + i)
		((VTXCOLORINSTANCE*)SubResource.pData)[i].vTranslation.y -= m_pInstanceSpeeds[i] * fTimeDelta;

		if (0.0f >= ((VTXCOLORINSTANCE*)SubResource.pData)[i].vTranslation.y)
			((VTXCOLORINSTANCE*)SubResource.pData)[i].vTranslation.y = 10.f;

	}

	DEVICECONTEXT->Unmap(m_pVBInstance.Get(), 0);
}

void CVIBuffer_Model_Instance::Update(const vector<PARTICLE_DESC>& In_ParticleDescs)
{
	if (In_ParticleDescs.size() == 0 || 0 == m_iNumInstance)
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	/* D3D11_MAP_WRITE_NO_OVERWRITE : SubResource구조체가 받아온 pData에 유요한 값이 담겨잇는 형태로 얻어오낟. */
	/* D3D11_MAP_WRITE_DISCARD : SubResource구조체가 받아온 pData에 값이 초기화된 형태로 얻어오낟. */
	DEVICECONTEXT->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_vector PitchYawRoll = XMLoadFloat3(&In_ParticleDescs[i].vCurrentRotation);

		_matrix RotationMatrix = XMMatrixRotationRollPitchYawFromVector(PitchYawRoll);
		XMStoreFloat4(&((VTXCOLORINSTANCE*)SubResource.pData)[i].vRight, RotationMatrix.r[0] * In_ParticleDescs[i].vCurrentScale.x);
		XMStoreFloat4(&((VTXCOLORINSTANCE*)SubResource.pData)[i].vUp, RotationMatrix.r[1] * In_ParticleDescs[i].vCurrentScale.y);
		XMStoreFloat4(&((VTXCOLORINSTANCE*)SubResource.pData)[i].vLook, RotationMatrix.r[2] * In_ParticleDescs[i].vCurrentScale.z);

		XMStoreFloat4(&((VTXCOLORINSTANCE*)SubResource.pData)[i].vTranslation,
			XMLoadFloat3(&In_ParticleDescs[i].vCurrentTarnslation));

		((VTXCOLORINSTANCE*)SubResource.pData)[i].vTranslation.w = 1.f;

		//비활성화라면 색을 0준다.
		if (!In_ParticleDescs[i].bEnable)
		{
			((VTXCOLORINSTANCE*)SubResource.pData)[i].vColor = _float4(0.f, 0.f, 0.f, 0.f);
		}

		else
		{
			((VTXCOLORINSTANCE*)SubResource.pData)[i].vColor = In_ParticleDescs[i].vCurrentColor;
		}

		// *(((VTXINSTANCE*)SubResource.pData) + i)
		/*((VTXCOLORINSTANCE*)SubResource.pData)[i].vTranslation.y -= m_pInstanceSpeeds[i] * fTimeDelta;

		if (0.0f >= ((VTXCOLORINSTANCE*)SubResource.pData)[i].vTranslation.y)
			((VTXCOLORINSTANCE*)SubResource.pData)[i].vTranslation.y = 10.f;*/

	}

	DEVICECONTEXT->Unmap(m_pVBInstance.Get(), 0);

}

void CVIBuffer_Model_Instance::Free()
{

}
