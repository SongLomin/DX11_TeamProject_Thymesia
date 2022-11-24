#pragma once
#include "Base.h"

BEGIN(Engine)

class CFrustum :
    public CBase
{
    DECLARE_SINGLETON(CFrustum)

public:
	/* 투영공간상의 절두체를 구성하는 여덟개 점을 모두 구한다. */
	HRESULT Initialize();

	/*월드 공간상의 여섯개 평면을 구한다. */
	void Update();

public:
	_bool isIn_Frustum_InWorldSpace(_fvector vWorldPoint, _float fRange = 0.f);
	_bool isIn_Frustum_InLocalSpace(_fvector vLocalPoint, _float fRange = 0.f);
	void Transform_ToLocalSpace(_fmatrix WorldMatrixInv);

private:
	_float4				m_vOriginalPoints[8];
	_vector				m_vWorldPoints[8];
	_float4				m_vWorldPlane[2][6];
	_float4				m_vLocalPlane[6];

	_int				iCurrentIndex = -1;

private:
	void Free();

};

END