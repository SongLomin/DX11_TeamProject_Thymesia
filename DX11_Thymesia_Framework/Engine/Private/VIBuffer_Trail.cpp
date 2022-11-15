#include "VIBuffer_Trail.h"
#include "GameInstance.h"
#include "Transform.h"

GAMECLASS_C(CVIBuffer_Trail)
CLONE_C(CVIBuffer_Trail, CComponent)

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Trail::Initialize_Prototype()
{
	/*UINT ByteWidth;
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
	UINT StructureByteStride;*/


#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumVertexBuffers = 1; // 인스턴스가 아니니까 1

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];
	// 제로 메모리해도 됨. 어차피 갱신
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

	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = 2; // m_iNumVertices(정점) - 2가 삼각형 개수
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

	shared_ptr<FACEINDICES16[]> pIndices(new FACEINDICES16[m_iNumPrimitive]);

	//FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitive];
	//ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices.get();

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{
	return S_OK;
}

void CVIBuffer_Trail::Start()
{
}

void CVIBuffer_Trail::Tick(_float fTimeDelta)
{
    /*if (m_bTrailOn)
    {
        if (m_iVtxCount < m_iNumVertices)
        {
            D3D11_MAPPED_SUBRESOURCE      SubResource;

            DEVICECONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

            _float4 vHandPos = m_vLocalSwordLow;

            if (!m_pRefBone)
            {
                _float4x4 matWorld = m_pOwnerTransform.lock()->Get_WorldMatrix();
                vHandPos = vHandPos.MultiplyCoord(matWorld);
            }
            else
            {
                _matrix matRightHand = m_pRefBone->Get_CombinedMatrix();
                matRightHand = XMMatrixMultiply(matRightHand, m_TransformationMatrix.XMLoad());
                _float4x4 matBone;
                XMStoreFloat4x4(&matBone, matRightHand);
                _float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();

                vHandPos = vHandPos.MultiplyCoord(matBone);
                vHandPos = vHandPos.MultiplyCoord(matWorld);
            }


            ((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.x = vHandPos.x;
            ((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.y = vHandPos.y;
            ((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.z = vHandPos.z;


            vHandPos = m_vLocalSwordHigh;
            if (!m_pRefBone)
            {
                _float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();
                vHandPos = vHandPos.MultiplyCoord(matWorld);
            }
            else
            {
                _matrix matRightHand = m_pRefBone->Get_CombinedMatrix();
                matRightHand = XMMatrixMultiply(matRightHand, m_TransformationMatrix.XMLoad());
                _float4x4 matBone;
                XMStoreFloat4x4(&matBone, matRightHand);
                _float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();

                vHandPos = vHandPos.MultiplyCoord(matBone);
                vHandPos = vHandPos.MultiplyCoord(matWorld);
            }

            ((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.x = vHandPos.x;
            ((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.y = vHandPos.y;
            ((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.z = vHandPos.z;

            m_iVtxCount += 2;

            if (m_iVtxCount > m_iNumVertices)
                m_iVtxCount = m_iNumVertices;

            for (_uint i = 0; i < m_iVtxCount; i += 2)
            {
                ((VTXTEX*)SubResource.pData)[i].vTexUV = { (_float)i / ((_float)m_iVtxCount - 2), 1.f };
                ((VTXTEX*)SubResource.pData)[i + 1].vTexUV = { (_float)i / ((_float)m_iVtxCount - 2), 0.f };
            }

            DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);

        }
        else
        {
            _uint   iRemoveCount = 2;

            if (iRemoveCount % 2 > 0)
            {
                iRemoveCount -= 1;
            }

            if (iRemoveCount < 2)
                iRemoveCount = 2;

            if (m_iVtxCount >= 2)
                m_iVtxCount -= iRemoveCount;


            D3D11_MAPPED_SUBRESOURCE      SubResource;
            DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

            if (m_iVtxCount <= m_iNumVertices / 4)
            {
                for (_uint i = 2; i < m_iNumVertices; i += 2)
                {
                    ((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[0].vPosition;
                    ((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[1].vPosition;
                }
                m_iVtxCount = 0;

            }

            for (_uint i = 0; i < m_iVtxCount; i += 2)
            {
                ((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i].vPosition;
                ((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i + 1].vPosition;
            }
            DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);
        }
    }
    else
    {
        _uint   iRemoveCount = 2;

        if (iRemoveCount % 2 > 0)
        {
            iRemoveCount -= 1;
        }

        if (iRemoveCount < 2)
            iRemoveCount = 2;

        if (m_iVtxCount >= 2)
            m_iVtxCount -= iRemoveCount;


        D3D11_MAPPED_SUBRESOURCE      SubResource;
        DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

        if (m_iVtxCount <= m_iNumVertices / 4)
        {
            for (_uint i = 2; i < m_iNumVertices; i += 2)
            {
                ((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[0].vPosition;
                ((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[1].vPosition;
            }
            m_iVtxCount = 0;

        }


        for (_uint i = 0; i < m_iVtxCount; i += 2)
        {
            ((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i].vPosition;
            ((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i + 1].vPosition;
        }
        DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);
    }*/
}

//void CVIBuffer_Trail::Set_OwnerTransform(shared_ptr<CTransform> pTransform)
//{
//    m_pOwnerTransform = pTransform;
//}

//CVIBuffer_Trail * CVIBuffer_Trail::Create()
//{
//	CVIBuffer_Trail*		pInstance = new CVIBuffer_Trail();
//
//	if (FAILED(pInstance->Initialize_Prototype()))
//	{
//		MSG_BOX("Failed to Created : CBackGround");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//CComponent* CVIBuffer_Trail::Clone(void * pArg)
//{
//	CVIBuffer_Trail*		pInstance = new CVIBuffer_Trail(*this);
//
//	if (FAILED(pInstance->Initialize(pArg)))
//	{
//		MSG_BOX("Failed to Cloned : CVIBuffer_Trail");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

//void CVIBuffer_Trail::Free()
//{
//
//}
