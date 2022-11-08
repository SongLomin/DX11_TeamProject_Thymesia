#pragma once
#include "Component.h"


BEGIN(Engine)

class CTransform;

class ENGINE_DLL CRigidBody : public CComponent
{
	GAMECLASS_H(CRigidBody);
	CLONE_H(CRigidBody, CComponent);

	struct FORCE
	{
		_float3	vDir;
		_float	fPower;
	};

	enum FORCE_MODE
	{
		MODE_FORCE = 0,
		MODE_IMPURSE,
		FORCE_MODE_END
	};


public:
	CRigidBody(const CRigidBody& rhs);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	void Add_Force(_vector In_vDir, const _float& In_fPower, const FORCE_MODE& In_ForceMode = MODE_FORCE);

private:
	GETSET(_float, fMass)
	GETSET(_float, fDrag)
	GETSET(_float, fMaxSpeed)
	GETSET(_bool, bUseGravity)

private:
	list<FORCE> m_Forces[FORCE_MODE_END];
	weak_ptr<CTransform> m_pMyTransform;

public:
	virtual void OnDestroy() override;

public:
	void Free();
};

END