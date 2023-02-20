#include "VIBuffer_Rect_Instance.h"
#include "GameInstance.h"

#include "SMath.h"

GAMECLASS_C(CVIBuffer_Rect_Instance)
CLONE_C(CVIBuffer_Rect_Instance, CComponent)

CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect_Instance::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::Initialize(void* pArg)
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXTEX* pVertices = DBG_NEW VTXTEX[m_iNumVertices];

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
		DEBUG_ASSERT;

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

	FACEINDICES16* pIndices = DBG_NEW FACEINDICES16[m_iNumPrimitive];
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
		DEBUG_ASSERT;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

void CVIBuffer_Rect_Instance::Start()
{
}

void CVIBuffer_Rect_Instance::Init_Particle(const _uint& In_Size)
{
	std::unique_lock<std::mutex> lock(m_JobMutex);

	if (0 == In_Size)
		return;

	int i = m_pVBInstance.Reset();

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

	VTXCOLORINSTANCE* pInstance = DBG_NEW VTXCOLORINSTANCE[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstance[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstance[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstance[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pInstance[i].vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
		pInstance[i].vColor = _float4(1.f, 0.f, 0.f, 1.f);
		pInstance[i].vSpriteTexUV = _float2(0.f, 0.f);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance)))
		DEBUG_ASSERT;

	Safe_Delete_Array(pInstance);
#pragma endregion

	lock.unlock();
}

HRESULT CVIBuffer_Rect_Instance::Render(ID3D11DeviceContext* pDeviceContext)
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

	pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	pDeviceContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	pDeviceContext->IASetPrimitiveTopology(m_eToplogy);

	/* 6 : 하나의 도형을 그리기위해 사용하는 인덱스의 갯수. 네모라서 여섯개.  */
	pDeviceContext->DrawIndexedInstanced(6, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Rect_Instance::Update(const vector<PARTICLE_DESC>& In_ParticleDescs, ID3D11DeviceContext* pDeviceContext, const _bool In_UseParentMatrix)
{
	std::unique_lock<std::mutex> lock(m_JobMutex);

	if (In_ParticleDescs.size() == 0 || 0 == m_iNumInstance)
		return;

	D3D11_MAPPED_SUBRESOURCE SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	/* D3D11_MAP_WRITE_NO_OVERWRITE : SubResource구조체가 받아온 pData에 유요한 값이 담겨잇는 형태로 얻어오낟. */
	/* D3D11_MAP_WRITE_DISCARD : SubResource구조체가 받아온 pData에 값이 초기화된 형태로 얻어오낟. */
	pDeviceContext->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	const PARTICLE_DESC* pParticleDesc = nullptr;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{	
		try
		{
			const PARTICLE_DESC& tParticleDecs = In_ParticleDescs.at(i);
			pParticleDesc = &In_ParticleDescs[i];
		}
		catch (const std::exception&)
		{
			void(0);
		}

		if (!pParticleDesc->bEnable)
		{
			//m_iNumInstance = i;
			//break;
			((VTXCOLORINSTANCE*)SubResource.pData)[i].vColor = _float4(0.f, 0.f, 0.f, 0.f);
			continue;
		}
		
		_float3 vCurrentScale = _float3(pParticleDesc->vCurrentScale.x, pParticleDesc->vCurrentScale.y, 1.f);
		_matrix WorldMatrix = SMath::Bake_WorldMatrix(vCurrentScale, pParticleDesc->vCurrentRotation, pParticleDesc->vCurrentTranslation);

		if (In_UseParentMatrix)
			WorldMatrix *= XMLoadFloat4x4(&pParticleDesc->ParentMatrix);

		WorldMatrix.r[3] += XMLoadFloat3(&pParticleDesc->vCurrentGravity);

		XMStoreFloat4(&((VTXCOLORINSTANCE*)SubResource.pData)[i].vRight, WorldMatrix.r[0]);
		XMStoreFloat4(&((VTXCOLORINSTANCE*)SubResource.pData)[i].vUp   , WorldMatrix.r[1]);
		XMStoreFloat4(&((VTXCOLORINSTANCE*)SubResource.pData)[i].vLook , WorldMatrix.r[2]);

		XMStoreFloat4(&((VTXCOLORINSTANCE*)SubResource.pData)[i].vTranslation,
			WorldMatrix.r[3]);

		((VTXCOLORINSTANCE*)SubResource.pData)[i].vColor = pParticleDesc->vCurrentColor;

		((VTXCOLORINSTANCE*)SubResource.pData)[i].vSpriteTexUV = pParticleDesc->vSpriteUV;
	}

	pDeviceContext->Unmap(m_pVBInstance.Get(), 0);

	lock.unlock();
}

void CVIBuffer_Rect_Instance::Free()
{

}
