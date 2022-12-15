#include "..\Public\VIBuffer.h"
#include "GameInstance.h"

GAMECLASS_C(CVIBuffer)

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iIndicesStride(rhs.m_iIndicesStride)
	, m_iNumIndices(rhs.m_iNumIndices)
	, m_iNumVertexBuffers(rhs.m_iNumVertexBuffers)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_eToplogy(rhs.m_eToplogy)
{


}


HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (nullptr == pDeviceContext)
		return E_FAIL;

	// Is VB null
	if (!m_pVB.Get())
	{
		DEBUG_ASSERT;
	}

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB.Get(),
	};

	_uint		iStrides[] = {
		m_iStride,
	};

	_uint		iOffsets[] = {
		0,
	};

	pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	pDeviceContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	pDeviceContext->IASetPrimitiveTopology(m_eToplogy);

	pDeviceContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}


HRESULT CVIBuffer::Create_VertexBuffer()
{

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pVB.GetAddressOf())))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pIB.GetAddressOf())))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	m_pIB.Reset();
	m_pVB.Reset();
}
