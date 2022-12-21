#pragma once
#include "CustomUI.h"
class CUI_LerpBar abstract : public CCustomUI
{


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

protected:
	_bool		Is_Lerping() { return m_bLerp; }
	void		Set_Lerp(_float In_fStart, _float In_fEnd, _float In_fTime, EASING_TYPE eType = EASING_TYPE::QUAD_IN);
	_float4		Get_Lerp();

private:
	//Lerp Variagbles
	_bool			m_bLerp;

	_float			m_fLerpAcc;
	_float			m_fLerpTick;
	_float			m_fLerpTime;
	
	_float4			m_vStart;
	_float4			m_vTarget;
	_float4			m_vLerped;

	EASING_TYPE		m_eLerpType;



};

