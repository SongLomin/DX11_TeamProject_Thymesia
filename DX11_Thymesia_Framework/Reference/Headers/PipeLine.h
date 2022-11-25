#pragma once
#include "Base.h" 

BEGIN(Engine)

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)

public:
	enum TRANSFORMSTATE { D3DTS_WORLD, D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

public:
	void Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformState);

	_matrix Get_Transform(TRANSFORMSTATE eState) {
		return XMLoadFloat4x4(&m_TransformState[eState]);
	}

	const _float4x4* Get_Transform_float4x4(TRANSFORMSTATE eState) {
		return &m_TransformState[eState];
	}

	const _float4x4* Get_Transform_TP(TRANSFORMSTATE eState) {
		return &m_TransformState_TP[eState];
	}

	_float4 Get_CamPosition() const {
		return m_vCamPosition;
	}


public:
	void Tick();

	_vector Get_LinearVelocity();
	_float4x4 Get_PreViewMatrix();
	_vector Get_AngularVelocity();

private:
	_float4x4			m_TransformState[D3DTS_END];
	_float4x4			m_TransformState_TP[D3DTS_END];
	_float4				m_vCamPosition;

	_float4				m_vPreCamPosition;
	_float4				m_vPreCamQuaternion;
	_float4x4			m_PreViewMatrix;
public:
	void Free();
};

END