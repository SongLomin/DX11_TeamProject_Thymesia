#pragma once

#include "Base.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent();
	CComponent(const CComponent& Prototype);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual void Start() PURE;
	virtual void Tick(_float fTimeDelta) {}
	virtual void LateTick(_float fTimeDelta) {}
	virtual HRESULT Render() { return S_OK; }

public:
	HRESULT	Set_Owner(weak_ptr<CGameObject> _pOwner);
	weak_ptr<CGameObject> Get_Owner();

protected:
	_bool						m_isCloned = false;
	weak_ptr<CGameObject>		m_pOwner;
	_uint						m_CreatedLevel;
	
public:
	virtual shared_ptr<CComponent> Clone(_uint iLevelIndex, void* pArg = nullptr) = 0;
	virtual void Free() override;

	friend class CComponent_Manager;
};

END