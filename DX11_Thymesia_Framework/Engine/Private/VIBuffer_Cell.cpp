#include "..\Public\VIBuffer_Cell.h"

GAMECLASS_C(CVIBuffer_Cell)
CLONE_C(CVIBuffer_Cell, CComponent)

CVIBuffer_Cell::CVIBuffer_Cell(const CVIBuffer_Cell& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cell::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CVIBuffer_Cell::Initialize(void* pArg)
{
	return S_OK;
}

void CVIBuffer_Cell::Start()
{
}

void CVIBuffer_Cell::Init_VIBufferCell(const _float3* pVerticesPos)
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXCUBETEX);
	m_iNumVertices = 3;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXCUBETEX* pVertices = new VTXCUBETEX[m_iNumVertices];

	for (_uint i = 0; i < 3; ++i)
	{
		pVertices[i].vPosition = pVerticesPos[i];
		m_vVerticePositions[i] = pVerticesPos[i];
	}
	
	//TexUV의 x, y, z를 곱해서 0과 가까이 나오면 외곽선
	pVertices[0].vTexUV = _float3(1.f, 0.f, 0.f);
	pVertices[1].vTexUV = _float3(0.f, 1.f, 0.f);
	pVertices[2].vTexUV = _float3(0.f, 0.f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	__super::Create_VertexBuffer();

	Safe_Delete_Array(pVertices);
#pragma endregion

	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = 3;
	m_iNumIndices = 4;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	//m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = sizeof(_ushort) * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	_ushort x1 = pIndices[0];
	_ushort x2 = pIndices[1];
	_ushort x3 = pIndices[2];
	_ushort x4 = pIndices[3];

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	Create_IndexBuffer();
	Safe_Delete_Array(pIndices);
}

void CVIBuffer_Cell::Free()
{

}