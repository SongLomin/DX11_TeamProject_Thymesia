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

_bool CVIBuffer_Ground::Get_Vertex(_uint _iIndex, VTXGROUND* _pOut)
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

	_int2	iPickIndex  = { _int(_vMousePos.m128_f32[0] / m_fInterval), _int(_vMousePos.m128_f32[2] / m_fInterval) };
	_int	iRoundIndx  = (_int)(_fRadious / m_fInterval);

	_int2	iBeginIndex, iEndIndex;
	iBeginIndex.x = (0 > iPickIndex.x - iRoundIndx) ? (0) : (iPickIndex.x - iRoundIndx);
	iBeginIndex.y = (0 > iPickIndex.y - iRoundIndx) ? (0) : (iPickIndex.y - iRoundIndx);
	 														
	iEndIndex.x   = ((_int)m_iNumVerticesX < iPickIndex.x + iRoundIndx) ? (m_iNumVerticesX) : (iPickIndex.x + iRoundIndx);
	iEndIndex.y   = ((_int)m_iNumVerticesZ < iPickIndex.y + iRoundIndx) ? (m_iNumVerticesZ) : (iPickIndex.y + iRoundIndx);

	for (_uint iZ(iBeginIndex.y); iZ < (_uint)iEndIndex.y; ++iZ)
	{
		for (_uint iX(iBeginIndex.x); iX < (_uint)iEndIndex.x; ++iX)
		{
			_ulong	iIndex = iZ * m_iNumVerticesX + iX;

			_float3 vPos    = ((VTXGROUND*)SubResource.pData)[iIndex].vPosition;
			_float  fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPos) - _vMousePos));
		
			switch (_iMode)
			{
				case 0: 
				{
					if (_fRadious < fLength)
						continue;

					((VTXGROUND*)SubResource.pData)[iIndex].vPosition.y += _fPower;
					m_VertexInfo[iIndex].vPosition = ((VTXGROUND*)SubResource.pData)[iIndex].vPosition;
				}
				break;

				case 1:
				{
					if (_fRadious < fLength)
						continue;

					_float fLerpPower = _fPower * (1.f - pow((fLength / _fRadious), 2.f));

					((VTXGROUND*)SubResource.pData)[iIndex].vPosition.y += fLerpPower;
					m_VertexInfo[iIndex].vPosition = ((VTXGROUND*)SubResource.pData)[iIndex].vPosition;
				}
				break;

				case 2:
				{
					((VTXGROUND*)SubResource.pData)[iIndex].vPosition.y = _fPower;
					m_VertexInfo[iIndex].vPosition = ((VTXGROUND*)SubResource.pData)[iIndex].vPosition;
				}
				break;
			}		
		}
	}

	for (_uint iZ(iBeginIndex.y); iZ < (_uint)iEndIndex.y; ++iZ)
	{
		for (_uint iX(iBeginIndex.x); iX < (_uint)iEndIndex.x; ++iX)
		{
			_ulong	iIndex = iZ * m_iNumVerticesX + iX;

			_long  iAdjacency[] =
			{
				_long(iIndex + m_iNumVerticesX),	// 위
				_long(iIndex + 1),					// 오른쪽
				_long(iIndex - m_iNumVerticesX),	// 아래
				_long(iIndex - 1)					// 왼쪽
			};
			
			if (0 == iX)
				iAdjacency[3] = -1;

			if (m_iNumVerticesX - 1 == iX)
				iAdjacency[1] = -1;

			if (0 == iZ)
				iAdjacency[2] = -1;

			if (m_iNumVerticesZ - 1 == iZ)
				iAdjacency[0] = -1;

			_float3 vNorm = m_VertexInfo[iIndex].vNormal;

			// 노말 벡터 계산
			_vector vComputeNorm = XMVectorSet(0.f, 0.f, 0.f, 0.f);

			for (_uint i = 0; i < 4; ++i)
			{
				_uint iNext = (3 == i) ? (0) : (i + 1);

				if (0 > iAdjacency[i] || 0 > iAdjacency[iNext])
					continue;

				_vector vLine_no1 = XMLoadFloat3(&m_VertexInfo[iAdjacency[i]].vPosition)     - XMLoadFloat3(&m_VertexInfo[iIndex].vPosition);
				_vector vLine_no2 = XMLoadFloat3(&m_VertexInfo[iAdjacency[iNext]].vPosition) - XMLoadFloat3(&m_VertexInfo[iIndex].vPosition);
				_vector vLingNorm = XMVector3Normalize(XMVector3Cross(vLine_no1, vLine_no2));

				vComputeNorm = XMVector3Normalize(vComputeNorm + vLingNorm);
			}

			XMStoreFloat3(&vNorm, vComputeNorm);

			m_VertexInfo[iIndex].vNormal = vNorm;
			((VTXGROUND*)SubResource.pData)[iIndex].vNormal = vNorm;

			if (0 > iAdjacency[1])
				continue;

			// 탄젠트 벡터 계산
			_float3 vTempTangent;
			XMStoreFloat3(&vTempTangent, XMVector3Normalize(XMLoadFloat3(&m_VertexInfo[iAdjacency[1]].vPosition) - XMLoadFloat3(&m_VertexInfo[iIndex].vPosition)));
			
			m_VertexInfo[iIndex].vTangent = vTempTangent;
			((VTXGROUND*)SubResource.pData)[iIndex].vTangent = vTempTangent;
		}
	}

	DEVICECONTEXT->Unmap(m_pVB.Get(), 0);
}

_bool CVIBuffer_Ground::Compute_MousePos(RAY _Ray, _matrix _WorldMatrix, _float3* pOut)
{
	_matrix		WorldMatrix = XMMatrixInverse(nullptr, _WorldMatrix);
	_vector		vRayPos, vRayDir;

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
			XMStoreFloat3(pOut, vPickedPos);

			return true;
		}
	}

	return false;
}

_bool CVIBuffer_Ground::Compute_MouseRatio(RAY _Ray, _matrix _WorldMatrix, _float2* pOut)
{
	_matrix		WorldMatrix = XMMatrixInverse(nullptr, _WorldMatrix);

	_vector			vRayPos, vRayDir;

	vRayPos = XMVector3TransformCoord(XMLoadFloat4(&_Ray.vOrigin), WorldMatrix);
	vRayDir = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&_Ray.vDirection), WorldMatrix));

	_float		fDist;
	_float2		vMaxRange	= { m_iNumVerticesX * m_fInterval, m_iNumVerticesZ * m_fInterval };

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

			pOut->x = vPickedPos.m128_f32[0] / vMaxRange.x;
			pOut->y = vPickedPos.m128_f32[2] / vMaxRange.y;

			return true;
		}
	}

	return false;
}

_bool CVIBuffer_Ground::Compute_IsInTerrain(_uint _iNumVertexX, _uint _iNumVertexZ, _float _fInterval, _fvector _vTargetPos, _fmatrix _WorldMatrix)
{
	_float vRangeX = _iNumVertexX * _fInterval;
	_float vRangeZ = _iNumVertexZ * _fInterval;

	_matrix WorldMatrixInv(XMMatrixInverse(nullptr, _WorldMatrix));
	_float3 vLocalPos;
	XMStoreFloat3(&vLocalPos, XMVector3TransformCoord(_vTargetPos, WorldMatrixInv));

	if ((0.f < vLocalPos.x && vRangeX > vLocalPos.x) &&
		(0.f < vLocalPos.z && vRangeZ > vLocalPos.z))
	{
		return true;
	}

	return false;
}

HRESULT CVIBuffer_Ground::Init_Mesh(shared_ptr<MESH_DATA> tMeshData, D3D11_PRIMITIVE_TOPOLOGY In_eToplogy)
{
	m_iStride			= sizeof(VTXGROUND);
	m_iNumVertices		= tMeshData.get()->iNumVertices;
	m_iNumVertexBuffers = 1;

	VTXGROUND* pVertices = new VTXGROUND[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXGROUND) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i] = tMeshData.get()->pGroundVertices[i];

		m_VertexInfo.push_back(pVertices[i]);
	}

	m_iIndicesStride	= sizeof(FACEINDICES32);
	m_iNumPrimitive		= tMeshData.get()->iNumFaces;
	m_iNumIndices		= m_iNumPrimitive * 3;
	m_eIndexFormat		= DXGI_FORMAT_R32_UINT;
	m_eToplogy			= In_eToplogy;

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i] = tMeshData.get()->pIndices[i];

		m_Indices.push_back(_uint3(pIndices[i]._1, pIndices[i]._2, pIndices[i]._3));
	}

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth				= m_iStride * m_iNumVertices;
	m_BufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride	= m_iStride;
	m_BufferDesc.CPUAccessFlags			= 0;
	m_BufferDesc.MiscFlags				= 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	/* ------------------------------------------------------------------ */

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth				= m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride	= 0;
	m_BufferDesc.CPUAccessFlags		    = 0;
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

HRESULT CVIBuffer_Ground::Init_Mesh(shared_ptr<MESH_DATA> tMeshData, _uint _iNumVerticesX, _uint _iNumVerticesZ, _float _fInterval)
{
	m_iNumVerticesX = _iNumVerticesX;
	m_iNumVerticesZ = _iNumVerticesZ;
	m_fInterval		= _fInterval;

	/* --------------------------------------------------------- */

	m_iStride			= sizeof(VTXGROUND);
	m_iNumVertices		= tMeshData.get()->iNumVertices;
	m_iNumVertexBuffers = 1;

	VTXGROUND* pVertices = new VTXGROUND[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXGROUND) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i] = tMeshData.get()->pGroundVertices[i];

		m_VertexInfo.push_back(pVertices[i]);
	}

	m_iIndicesStride	= sizeof(FACEINDICES32);
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
	m_fInterval			= _vInfo.z;
	m_iNumVerticesX		= (_uint)_vInfo.x;
	m_iNumVerticesZ		= (_uint)_vInfo.y;

	/* 버텍스 버퍼 생성 */
	m_iStride			= sizeof(VTXGROUND);
	m_iNumVertices		= m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;

	VTXGROUND* pVertices = new VTXGROUND[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXGROUND) * m_iNumVertices);

	for (_uint iCol = 0; iCol < m_iNumVerticesZ; ++iCol)
	{
		for (_uint iRow = 0; iRow < m_iNumVerticesX; ++iRow)
		{
			_ulong	iIndex = iCol * m_iNumVerticesX + iRow;

			pVertices[iIndex].vPosition = _float3(_float(iRow * m_fInterval), 0.f, _float(iCol * m_fInterval));
			pVertices[iIndex].vTexUV    = _float2(iRow / (m_iNumVerticesX - 1.f), iCol / (m_iNumVerticesZ - 1.f));
			pVertices[iIndex].vNormal	= _float3(0.f, 1.f, 0.f);
			pVertices[iIndex].vTangent  = _float3(1.f, 0.f, 0.f);

			m_VertexInfo.push_back(pVertices[iIndex]);
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

void CVIBuffer_Ground::Free()
{
}
