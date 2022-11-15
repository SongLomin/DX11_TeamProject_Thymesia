#include "VIBuffer_Trail.h"
#include "GameInstance.h"

#include "Transform.h"
#include "BoneNode.h"
#include "ModelData.h"

GAMECLASS_C(CVIBuffer_Trail)
CLONE_C(CVIBuffer_Trail, CComponent)

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Trail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{
    if (!pArg)
        return E_FAIL;

	ZeroMemory(&m_pTrailDesc, sizeof(TRAIL_DESC));
	m_pTrailDesc = *(TRAIL_DESC*)pArg;

	/* 버텍스 버퍼 생성 */
    m_iStride           = sizeof(VTXTEX);
    m_iNumVertices      = 2 * m_pTrailDesc.iMaxCnt + 2;
    m_iNumVertexBuffers = 1;

	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXTEX) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; i += 2)
    {
        pVertices[i].vPosition     = m_pTrailDesc.vPos_0;
        pVertices[i + 1].vPosition = m_pTrailDesc.vPos_1;

        pVertices[i].vTexUV        = _float2(1.f, 0.f);
        pVertices[i + 1].vTexUV    = _float2(1.f, 1.f);
    }

	pVertices[0].vTexUV = _float2(0.f, 0.f);
	pVertices[1].vTexUV = _float2(0.f, 1.f);

	/* 인덱스 버퍼 생성 */
	m_iIndicesStride = sizeof(FACEINDICES16);
    m_iNumPrimitive  = m_pTrailDesc.iMaxCnt * 2;
    m_iNumIndices    = 3 * m_iNumPrimitive;
    m_eIndexFormat   = DXGI_FORMAT_R16_UINT;
    m_eToplogy       = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	 FACEINDICES16* pIndices = new FACEINDICES16[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumIndices);

    for (_uint i = 0; i < m_iNumPrimitive; i += 2)
    {
        pIndices[i]     = { _ushort(i), _ushort(i + 2), _ushort(i + 3) };
        pIndices[i + 1] = { _ushort(i), _ushort(i + 3), _ushort(i + 1) };
    }

	 /* ------------------------------------------------------------------ */

    ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
    m_BufferDesc.ByteWidth             = m_iStride * m_iNumVertices;
    m_BufferDesc.Usage                 = D3D11_USAGE_DYNAMIC;
    m_BufferDesc.BindFlags             = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.StructureByteStride   = m_iStride;
    m_BufferDesc.CPUAccessFlags        = D3D11_CPU_ACCESS_WRITE;
    m_BufferDesc.MiscFlags             = 0;

    ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

    if (FAILED(__super::Create_VertexBuffer()))
        return E_FAIL;

    /* ------------------------------------------------------------------ */

    ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
    m_BufferDesc.ByteWidth             = m_iIndicesStride * m_iNumPrimitive;
    m_BufferDesc.Usage                 = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags             = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.StructureByteStride   = 0;
    m_BufferDesc.CPUAccessFlags        = 0;
    m_BufferDesc.MiscFlags             = 0;

    ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    m_SubResourceData.pSysMem = pIndices;

    if (FAILED(__super::Create_IndexBuffer()))
        return E_FAIL;

    /* ------------------------------------------------------------------ */

    Safe_Delete_Array(pVertices);
    Safe_Delete_Array(pIndices);

	return S_OK;
}

void CVIBuffer_Trail::Start()
{
}

void CVIBuffer_Trail::Update(_float _fTimeDelta, weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data)
{
	if (!m_pVB.Get())
		return;

    _matrix		ParentMatrix
        = _pOwnerBoneNode.lock()->Get_OffsetMatrix()
        * _pOwnerBoneNode.lock()->Get_CombinedMatrix()
        * XMLoadFloat4x4(&_pOwnerModel_Data.lock()->TransformMatrix)
        * _pOwnerTransform.lock()->Get_WorldMatrix();

    ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
    ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
    ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);

	D3D11_MAPPED_SUBRESOURCE		tSubResource;

	DEVICECONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &tSubResource);

	if (m_iVtxCnt >= m_iNumVertices)
    {
        _uint iRemoveCnt = m_iVtxCnt / 4;
        m_iVtxCnt -= iRemoveCnt;

        for (_uint i = 0; i < m_iVtxCnt; i += 2)
        {
            ((VTXTEX*)tSubResource.pData)[i].vPosition     = ((VTXTEX*)tSubResource.pData)[iRemoveCnt + i].vPosition;
            ((VTXTEX*)tSubResource.pData)[i + 1].vPosition = ((VTXTEX*)tSubResource.pData)[iRemoveCnt + i + 1].vPosition;
        }
    }
    else
    {
        _vector vPos[2] =
        {
            XMVectorSetW(XMLoadFloat3(&m_pTrailDesc.vPos_0), 1.f),
            XMVectorSetW(XMLoadFloat3(&m_pTrailDesc.vPos_1), 1.f),
        };

        vPos[0] = XMVector3TransformCoord(vPos[0], ParentMatrix);
        vPos[1] = XMVector3TransformCoord(vPos[1], ParentMatrix);

        if (0 == m_iVtxCnt)
        {
            for (_uint i = 0; i < m_iNumVertices; i += 2)
            {
                XMStoreFloat3(&((VTXTEX*)tSubResource.pData)[m_iVtxCnt].vPosition    , vPos[0]);
                XMStoreFloat3(&((VTXTEX*)tSubResource.pData)[m_iVtxCnt + 1].vPosition, vPos[1]);
            }
        }
        else
        {
            XMStoreFloat3(&((VTXTEX*)tSubResource.pData)[m_iVtxCnt].vPosition    , vPos[0]);
            XMStoreFloat3(&((VTXTEX*)tSubResource.pData)[m_iVtxCnt + 1].vPosition, vPos[1]);
        }

        for (_uint i = 0; i < m_iVtxCnt; i += 2)
        {
            ((VTXTEX*)tSubResource.pData)[i].vTexUV     = _float2(_float(i) / _float(m_iVtxCnt - 2.f), 0.f);
            ((VTXTEX*)tSubResource.pData)[i + 1].vTexUV = _float2(_float(i) / _float(m_iVtxCnt - 2.f), 1.f);
        }

        if (0 == m_iVtxCnt || 0.005f < m_fAccTime)
        {
            m_iVtxCnt += 2;
            m_fAccTime = 0.f;
        }
    }

	DEVICECONTEXT->Unmap(m_pVB.Get(), 0);
}

void CVIBuffer_Trail::Free()
{

}
