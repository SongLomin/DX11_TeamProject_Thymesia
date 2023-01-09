#pragma once
#include "Component.h"


BEGIN(Engine)

class CNavigation;

class ENGINE_DLL CTransform final : public CComponent
{
	GAMECLASS_H(CTransform)
	CLONE_H(CTransform, CComponent)


//public:
//	typedef struct tagTransformDesc
//	{
//		_float			fSpeedPerSec; 5.f
//		_float			fRotationPerSec; XMConvertToRadians(90.0f);
//	}TRANSFORMDESC;

public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_TRANSLATION, STATE_END };
public:
	CTransform(const CTransform& rhs);

public:
	_vector Get_State(STATE eState) const {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	_matrix Get_UnScaledWorldMatrix() const;

	void Set_WorldMatrix(_fmatrix In_WorldMatrix) {
		XMStoreFloat4x4(&m_WorldMatrix, In_WorldMatrix);
	}

	void Set_State(STATE eState, _fvector vState)
	{
		_float4		vData;
		XMStoreFloat4(&vData, vState);
		memcpy(&m_WorldMatrix.m[eState], &vData, sizeof(_float4));
	}

	_vector	Get_Position() const { return Get_State(STATE_TRANSLATION); }

public:/*
	void Set_TransformDesc(const TRANSFORMDESC * pTransformDesc) {
		m_TransformDesc = *pTransformDesc;
	}*/

	void Set_Scaled(_float3 vScale);
	_float3 Get_Scaled();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;

public:
	HRESULT Set_ShaderResource(weak_ptr<class CShader> pShader, const char* pConstantName);

public:
	HRESULT Go_Direction(_fvector vDirection, _float fTimeDelta);
	HRESULT Go_Straight(_float fTimeDelta, weak_ptr<CNavigation> pNaviCom = weak_ptr<CNavigation>());
	HRESULT Go_Backward(_float fTimeDelta);
	HRESULT Go_Left(_float fTimeDelta);
	HRESULT Go_Right(_float fTimeDelta);
	HRESULT Go_Up(_float fTimeDelta);
	HRESULT Go_Down(_float fTimeDelta);

	// TODO : for effect tool << movement by world axis >>
	HRESULT Go_Straight_WorldAxis(_float fTimeDelta);
	HRESULT Go_Backward_WorldAxis(_float fTimeDelta);
	HRESULT Go_Left_WorldAxis(_float fTimeDelta);
	HRESULT Go_Right_WorldAxis(_float fTimeDelta);
	HRESULT Go_Up_WorldAxis(_float fTimeDelta);
	HRESULT Go_Down_WorldAxis(_float fTimeDelta);


	HRESULT	Set_Position(_fvector vPos, weak_ptr<CNavigation> pNaviCom = weak_ptr<CNavigation>());
	HRESULT Add_Position(_fvector vPos, weak_ptr<CNavigation> pNaviCom = weak_ptr<CNavigation>());
	HRESULT Add_PositionWithRotation(_fvector vPos, weak_ptr<CNavigation> pNaviCom = weak_ptr<CNavigation>(), const _bool& In_bReverse = false );

	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	void Rotation_Add(_fvector vAxis, _float fRadian);
	void Rotation_Quaternion(_fvector vQuaternion);
	void Rotation_PitchYawRoll(_float3 vRadian);
	void LookAt(_fvector vTargetPos);
	void LookAt2D(_fvector vTargetPos);
	void LookAt_Right(_fvector vTargetRight);
	void Set_Look(_fvector vLook);
	void Set_Look2D(_fvector vLook);
	void Chase(_fvector vTargetPos, _float fTimeDelta);

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

protected:
	virtual void OnDestroy() override;

private:
	_float4x4				m_WorldMatrix;

public:
	void Free();
};

END