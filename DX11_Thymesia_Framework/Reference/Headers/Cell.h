#pragma once
#include "Base.h"
BEGIN(Engine)

class CVIBuffer_Cell;
class CGameObject;

class CCell final : public CBase
{
	GAMECLASS_H(CCell)

public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	

public:
	_vector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}

	_int Get_Index() const {
		return m_iIndex;
	}

	void Set_Neighbor(LINE eLine, _int iNeighborIndex) {
		m_iNeighbor[eLine] = iNeighborIndex;
	}

public:
	HRESULT Initialize(const _float3* pPoints, _int iIndex);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
	_bool isIn(_fvector vPosition, _int* pNeighborIndex, _float3* Out_vNormal);

#ifdef _DEBUG
public:
	HRESULT Render(ID3D11DeviceContext* pDeviceContext);
#endif // _DEBUG

private:
	weak_ptr<CGameObject>		m_pOwner;
	_uint						m_iIndex = 0;
	_float3						m_vPoints[POINT_END];
	_float3						m_vLine[LINE_END];
	_float3						m_vNormal[LINE_END];
	_int						m_iNeighbor[LINE_END] = { -1, -1, -1 };

#ifdef _DEBUG
	weak_ptr<CVIBuffer_Cell>	m_pVIBuffer;
#endif

public:
	static shared_ptr<CCell> Create(weak_ptr<CGameObject> pOwner, const _float3* pPoints, _int iIndex);
	void Free();
};

END