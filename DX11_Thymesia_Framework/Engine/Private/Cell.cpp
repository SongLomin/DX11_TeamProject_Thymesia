#include "..\Public\Cell.h"
#include "VIBuffer_Cell.h"
#include "GameObject.h"

GAMECLASS_C(CCell)

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	m_iIndex = iIndex;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	m_vNormal[LINE_AB] = _float3(m_vLine[LINE_AB].z * -1.f, 0.f, m_vLine[LINE_AB].x);
	m_vNormal[LINE_BC] = _float3(m_vLine[LINE_BC].z * -1.f, 0.f, m_vLine[LINE_BC].x);
	m_vNormal[LINE_CA] = _float3(m_vLine[LINE_CA].z * -1.f, 0.f, m_vLine[LINE_CA].x);

#ifdef _DEBUG
	m_pVIBuffer = m_pOwner.lock()->Add_Component<CVIBuffer_Cell>();
	m_pVIBuffer.lock()->Init_VIBufferCell(m_vPoints);
	if (nullptr == m_pVIBuffer.lock().get())
		DEBUG_ASSERT;
#endif // _DEBUG

	return S_OK;
}


/* 전달받은 두개의 점과 내 세갱릐 점중 같은 점이 두개냐?! */
_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}


	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex, _float3* Out_vNormal)
{
	/* 이 쎌의 세변에 대해서 나갔는지 안낙ㅆ다ㅣ;ㅓ 판단한다. */
	for (_uint i = 0; i < (_uint)LINE_END; ++i)
	{
		_vector	vDir = vPosition - XMLoadFloat3(&m_vPoints[i]);

		/* 바깥으로 낙ㅆ아어 */
		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&m_vNormal[i])))))
		{
			*pNeighborIndex = m_iNeighbor[i];
			XMStoreFloat3(Out_vNormal, XMVector3Normalize(XMLoadFloat3(&m_vLine[i])));

			return false;
		}
	}
	return true;
}




#ifdef _DEBUG
HRESULT CCell::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (nullptr == m_pVIBuffer.lock().get())
		DEBUG_ASSERT;

	m_pVIBuffer.lock()->Render(pDeviceContext);

	return S_OK;
}
#endif // _DEBUG

shared_ptr<CCell> CCell::Create(weak_ptr<CGameObject> pOwner, const _float3* pPoints, _int iIndex)
{
	shared_ptr<CCell> pInstance = make_shared<CCell>();

	pInstance->m_this = pInstance;
	pInstance->m_pOwner = pOwner;

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
	}

	return pInstance;
}

void CCell::Free()
{

}
