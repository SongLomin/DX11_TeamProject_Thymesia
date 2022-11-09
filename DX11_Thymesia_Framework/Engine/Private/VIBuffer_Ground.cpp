#include "VIBuffer_Ground.h"
#include "GameInstance.h"

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

			m_VertexInfo.push_back(pVertices[iIndex]);
			m_VertexPositions.push_back(pVertices[iIndex].vPosition); // 나중에 지우기
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

void CVIBuffer_Ground::Start()
{
}

_bool CVIBuffer_Ground::Get_VertexPos(_uint _iIndex, _float3* _pOut)
{
	if (m_VertexPositions.size() <= _iIndex)
		return false;

	*_pOut = m_VertexPositions[_iIndex];

	return true;
}

void CVIBuffer_Ground::Update(_vector _vMousePos, _float _fRadious, _float _fPower, _uint _iMode)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	DEVICECONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint iCol = 0; iCol < m_iNumVerticesZ; ++iCol)
	{
		for (_uint iRow = 0; iRow < m_iNumVerticesX; ++iRow)
		{
			_ulong	iIndex = iCol * m_iNumVerticesX + iRow;

			_float3 vPos    = ((VTXNORTEX*)SubResource.pData)[iIndex].vPosition;
			_float  fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPos) - _vMousePos));

			if (_fRadious >= fLength)
			{
				if (0 == _iMode)
				{
					((VTXNORTEX*)SubResource.pData)[iIndex].vPosition.y += _fPower;
					m_VertexPositions[iIndex] = ((VTXNORTEX*)SubResource.pData)[iIndex].vPosition;
				}
				else if (1 == _iMode)
				{
					_float fLerpPower = _fPower;
					
					if (_fRadious == fLength)
						continue;
					else
						fLerpPower = (0 == fLength) ? (_fPower) : (_fPower * (fLength / _fRadious));


					((VTXNORTEX*)SubResource.pData)[iIndex].vPosition.y += fLerpPower;
					m_VertexPositions[iIndex] = ((VTXNORTEX*)SubResource.pData)[iIndex].vPosition;
				}
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

		_vector	vVec0 = XMLoadFloat3(&m_VertexPositions[iIndices.ix]);
		_vector	vVec1 = XMLoadFloat3(&m_VertexPositions[iIndices.iy]);
		_vector	vVec2 = XMLoadFloat3(&m_VertexPositions[iIndices.iz]);

		if (true == DirectX::TriangleTests::Intersects(vRayPos, vRayDir, vVec0, vVec1, vVec2, fDist))
		{
			vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * fDist;
			XMStoreFloat3(pOut, XMVector3TransformCoord(vPickedPos, _WorldMatrix));

			return true;
		}
	}

	return false;
}

void CVIBuffer_Ground::Free()
{
}
