#include "VIBuffer_Ground.h"
#include "GameInstance.h"
#include "ModelData.h"

GAMECLASS_C(CVIBuffer_Ground)
CLONE_C(CVIBuffer_Ground, CComponent)


CVIBuffer_Ground::CVIBuffer_Ground(const CVIBuffer_Ground& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Ground::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Ground::Initialize(void* pArg)
{
	return S_OK;
}

void CVIBuffer_Ground::Start()
{
}

_bool CVIBuffer_Ground::Get_VertexPos(_uint _iIndex, _float3* _pOut)
{
	if (m_VertexInfo.size() <= _iIndex)
		return false;

	*_pOut = m_VertexInfo[_iIndex].vPosition;

	return true;
}

_bool CVIBuffer_Ground::Get_Vertex(_uint _iIndex, VTXNORTEX* _pOut)
{
	if (m_VertexInfo.size() <= _iIndex)
		return false;

	*_pOut = m_VertexInfo[_iIndex];

	return true;
}

_bool CVIBuffer_Ground::Get_Indices(_uint _iIndex, _uint3* _pOut)
{
	if (m_Indices.size() <= _iIndex)
		return false;

	*_pOut = m_Indices[_iIndex];

	return true;
}

void CVIBuffer_Ground::Update(_vector _vMousePos, _float _fRadious, _float _fPower, _uint _iMode)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	DEVICECONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		_float3 vPos    = ((VTXNORTEX*)SubResource.pData)[i].vPosition;
		_float  fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPos) - _vMousePos));

		if (_fRadious >= fLength)
		{
			if (0 == _iMode)
			{
				((VTXNORTEX*)SubResource.pData)[i].vPosition.y += _fPower;
				m_VertexInfo[i].vPosition = ((VTXNORTEX*)SubResource.pData)[i].vPosition;
			}

			else if (1 == _iMode)
			{
				_float fLerpPower = _fPower * (1.f - pow((fLength / _fRadious), 2.f));

				((VTXNORTEX*)SubResource.pData)[i].vPosition.y += fLerpPower;
				m_VertexInfo[i].vPosition = ((VTXNORTEX*)SubResource.pData)[i].vPosition;
			}
		}		
	}

	DEVICECONTEXT->Unmap(m_pVB.Get(), 0);
}

_bool CVIBuffer_Ground::Compute_MousePos(RAY _Ray, _matrix _WorldMatrix, _float3* pOut)
{
	_matrix		WorldMatrix = XMMatrixInverse(nullptr, _WorldMatrix);

	_vector			vRayPos, vRayDir;

	vRayPos = XMVector3TransformCoord(XMLoadFloat4(&_Ray.vOrigin), WorldMatrix);
	vRayDir = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&_Ray.vDirection), WorldMatrix));

	_float		fDist;

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		_uint3		iIndices = m_Indices[i];
		_vector		vPickedPos;

		_vector	vVec0 = XMLoadFloat3(&m_VertexInfo[iIndices.ix].vPosition);
		_vector	vVec1 = XMLoadFloat3(&m_VertexInfo[iIndices.iy].vPosition);
		_vector	vVec2 = XMLoadFloat3(&m_VertexInfo[iIndices.iz].vPosition);

		if (true == DirectX::TriangleTests::Intersects(vRayPos, vRayDir, vVec0, vVec1, vVec2, fDist))
		{
			vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * fDist;
			XMStoreFloat3(pOut, XMVector3TransformCoord(vPickedPos, _WorldMatrix));

			return true;
		}
	}

	return false;
}

HRESULT CVIBuffer_Ground::Init_Mesh(shared_ptr<MESH_DATA> tMeshData)
{
	m_iStride			= sizeof(VTXNORTEX);
	m_iNumVertices		= tMeshData.get()->iNumVertices;
	m_iNumVertexBuffers = 1;

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i] = tMeshData.get()->pGroundVertices[i];

		m_VertexInfo.push_back(pVertices[i]);
	}

	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive		= tMeshData.get()->iNumFaces;
	m_iNumIndices		= m_iNumPrimitive * 3;
	m_eIndexFormat		= DXGI_FORMAT_R32_UINT;
	m_eToplogy			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i] = tMeshData.get()->pIndices[i];

		m_Indices.push_back(_uint3(pIndices[i]._1, pIndices[i]._2, pIndices[i]._3));
	}

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

HRESULT CVIBuffer_Ground::Init_Mesh(_float4 _vInfo)
{
	_float vInterval		= _vInfo.z;
	_float _iNumVerticesX	= (_uint)_vInfo.x;
	_float _iNumVerticesZ	= (_uint)_vInfo.y;

	m_iNumVerticesX = _iNumVerticesX;
	m_iNumVerticesZ = _iNumVerticesZ;

	/* 버텍스 버퍼 생성 */
	m_iStride			= sizeof(VTXNORTEX);
	m_iNumVertices		= _iNumVerticesX * _iNumVerticesZ;
	m_iNumVertexBuffers = 1;

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (_uint iCol = 0; iCol < _iNumVerticesZ; ++iCol)
	{
		for (_uint iRow = 0; iRow < _iNumVerticesX; ++iRow)
		{
			_ulong	iIndex = iCol * _iNumVerticesX + iRow;

			pVertices[iIndex].vPosition = _float3(_float(iRow * vInterval), 0.f, _float(iCol * vInterval));
			pVertices[iIndex].vTexUV    = _float2(iRow / (_iNumVerticesX - 1.f), iCol / (_iNumVerticesZ - 1.f));
			pVertices[iIndex].vNormal	= _float3(0.f, 1.f, 0.f);

			m_VertexInfo.push_back(pVertices[iIndex]);
		}
	}
	/* ------------------------------------------------------------------ */

	/* 인덱스 버퍼 생성 */
	m_iIndicesStride	= sizeof(FACEINDICES32);
	m_iNumPrimitive		= (_iNumVerticesX - 1) * (_iNumVerticesZ - 1) * 2;
	m_iNumIndices		= m_iNumPrimitive * 3;
	m_eIndexFormat		= DXGI_FORMAT_R32_UINT;
	m_eToplogy			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_ulong		dwNumFaces = 0;
	for (_uint iCol = 0; iCol < _iNumVerticesZ - 1; ++iCol)
	{
		for (_uint iRow = 0; iRow < _iNumVerticesX - 1; ++iRow)
		{
			_ulong	iIndex = iCol * _iNumVerticesX + iRow;

			_ulong  iIndices[] =
			{
				iIndex + _iNumVerticesX,
				iIndex + _iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[dwNumFaces] = { iIndices[0], iIndices[1], iIndices[2] };
			m_Indices.push_back(_uint3(pIndices[dwNumFaces]._1, pIndices[dwNumFaces]._2, pIndices[dwNumFaces]._3));
			++dwNumFaces;

			pIndices[dwNumFaces] = { iIndices[0], iIndices[2], iIndices[3] };
			m_Indices.push_back(_uint3(pIndices[dwNumFaces]._1, pIndices[dwNumFaces]._2, pIndices[dwNumFaces]._3));
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

void CVIBuffer_Ground::Free()
{
}
