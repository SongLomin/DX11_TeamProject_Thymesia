#pragma once
#include "ClientComponent.h"


BEGIN(Client)


class CEasingTransform : public CClientComponent
{
    GAMECLASS_H(CEasingTransform)
    CLONE_H(CEasingTransform, CComponent)

public:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;

	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	_bool		Is_Lerping() { return m_bLerp; }
	void		Set_LerpFloat(_float In_fStart, _float In_fEnd, _float In_fTime, EASING_TYPE eType = EASING_TYPE::QUAD_IN);
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


END

