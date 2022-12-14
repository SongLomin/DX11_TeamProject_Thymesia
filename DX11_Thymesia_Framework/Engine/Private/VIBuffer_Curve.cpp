#include "VIBuffer_Curve.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "Transform.h"
#include "BoneNode.h"
#include "ModelData.h"

GAMECLASS_C(CVIBuffer_Curve)
CLONE_C(CVIBuffer_Curve, CComponent)

CVIBuffer_Curve::CVIBuffer_Curve(const CVIBuffer_Curve& rhs)
    : CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Curve::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CVIBuffer_Curve::Initialize(void* pArg)
{
	// need pArg.
	if (!pArg)
	{
		DEBUG_ASSERT;
	}

	m_iNumInstance = *(_uint*)pArg;

#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXPOINT* pVertices = DBG_NEW VTXPOINT;

	pVertices->vPosition = _float3(0.f, 0.f, 0.f);
	pVertices->vPSize = _float2(1.0f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete(pVertices);

#pragma endregion

#pragma region INSTANCEBUFFER
	m_iInstanceStride = sizeof(VTXCURVE);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXCURVE* pInstance = DBG_NEW VTXCURVE[m_iNumInstance];

	_float	fRatio;
	_float	fNextRatio;
	for (_uint i = 0; i < m_iNumInstance - 1; ++i)
	{
		fRatio = (_float)i / (m_iNumInstance - 1);
		fNextRatio = (_float)(i + 1) / (m_iNumInstance - 1);
		pInstance[i].vRatio.x = fRatio;
		pInstance[i].vRatio.y = fNextRatio;
	}

	pInstance[m_iNumInstance - 1].vRatio.x = 1.f;
	pInstance[m_iNumInstance - 1].vRatio.y = 1.f;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstance);

#pragma region INDEXBUFFER	
	m_iIndicesStride = sizeof(_ushort);
	m_iNumPrimitive = m_iNumInstance;
	m_iNumIndices = m_iNumInstance;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_ushort* pIndices = DBG_NEW _ushort[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitive);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion





	return S_OK;
}

void CVIBuffer_Curve::Start()
{
}

HRESULT CVIBuffer_Curve::Render(ID3D11DeviceContext* pDeviceContext)
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
	DEVICECONTEXT->DrawIndexedInstanced(1, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

//void CVIBuffer_Curve::Update(_fvector In_vLeft, _fvector In_vLeftDir, _fvector In_vRight, _fvector in_vRightDir)
//{
//    if (!m_pVB.Get())
//        return;
//
//    _vector Point0 = In_vLeft - In_vLeftDir;
//    _vector Point3 = In_vRight + in_vRightDir;
//	_float	fRatio;
//
//    D3D11_MAPPED_SUBRESOURCE		SubResource;
//
//	DEVICECONTEXT->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
//
//	for (_uint i = 0; i < m_iNumInstance; ++i)
//	{
//		fRatio = (_float)i / (m_iNumInstance - 1);
//		// *(((VTXINSTANCE*)SubResource.pData) + i)
//		((VTXCURVE*)SubResource.pData)[i].fRatio = fRatio;
//	}
//
//	DEVICECONTEXT->Unmap(m_pVBInstance.Get(), 0);
//}

void CVIBuffer_Curve::Free()
{

}
