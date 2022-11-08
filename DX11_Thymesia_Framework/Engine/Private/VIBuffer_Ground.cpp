#include "VIBuffer_Ground.h"

GAMECLASS_C(CVIBuffer_Ground)
CLONE_C(CVIBuffer_Ground, CComponent)


CVIBuffer_Ground::CVIBuffer_Ground(const CVIBuffer_Ground& rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{
}

HRESULT CVIBuffer_Ground::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Ground::Initialize(void* pArg)
{
	if (!pArg)
		return E_FAIL;

	_float4 vInfo = *(_float4*)pArg;

	/*
		x, y : 가로, 세로 수 
		z    : 간격
	*/


	_float vInterval	= vInfo.z;
	m_iNumVerticesX		= (_uint)vInfo.x;
	m_iNumVerticesZ		= (_uint)vInfo.y;

	/* 버텍스 버퍼 생성 */
	m_iStride			= sizeof(VTXNORTEX);
	m_iNumVertices		= m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (_uint iCol = 0; iCol < m_iNumVerticesZ; ++iCol)
	{
		for (_uint iRow = 0; iRow < m_iNumVerticesX; ++iRow)
		{
			_ulong	iIndex = iCol * m_iNumVerticesX + iRow;

			pVertices[iIndex].vPosition = _float3(_float(iRow * vInterval), 0.f, _float(iCol * vInterval));
			pVertices[iIndex].vTexUV    = _float2(iRow / (m_iNumVerticesX - 1.f), iCol / (m_iNumVerticesZ - 1.f));
			pVertices[iIndex].vNormal	= _float3(0.f, 1.f, 0.f);
		}
	}
	/* ------------------------------------------------------------------ */

	/* 인덱스 버퍼 생성 */
	m_iIndicesStride	= sizeof(FACEINDICES32);
	m_iNumPrimitive		= (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndices		= m_iNumPrimitive * 3;
	m_eIndexFormat		= DXGI_FORMAT_R32_UINT;
	m_eToplogy			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	
	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_ulong		dwNumFaces = 0;
	for (_uint iCol = 0; iCol < m_iNumVerticesZ - 1; ++iCol)
	{
		for (_uint iRow = 0; iRow < m_iNumVerticesX - 1; ++iRow)
		{
			_ulong	iIndex = iCol * m_iNumVerticesX + iRow;

			_ulong  iIndices[] =
			{
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[dwNumFaces] = { iIndices[0], iIndices[1], iIndices[2] };
			++dwNumFaces;

			pIndices[dwNumFaces] = { iIndices[0], iIndices[2], iIndices[3] };
			++dwNumFaces;
		}
	}

	/* ------------------------------------------------------------------ */

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth				= m_iStride * m_iNumVertices;
	m_BufferDesc.Usage					= D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride	= m_iStride;
	m_BufferDesc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags				= 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	/* ------------------------------------------------------------------ */

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth				= m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage					= D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride	= 0;
	m_BufferDesc.CPUAccessFlags		    = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags				= 0;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	/* ------------------------------------------------------------------ */

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

	return S_OK;
}

void CVIBuffer_Ground::Start()
{
}

void CVIBuffer_Ground::Free()
{
}
