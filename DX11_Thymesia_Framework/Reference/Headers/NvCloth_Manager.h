#pragma once
#include "Base.h"
#include "CustomContextManagerCallback.h"
#include "CustomPxAllocatorCallback.h"
#include "CustomPxErrorCallback.h"
#include "CustomPxAssertHandler.h"

BEGIN(Engine)

class CNvCloth_Manager final : public CBase
{
	DECLARE_SINGLETON(CNvCloth_Manager)

public:
	Solver* Get_Solver();
	Factory* Get_Factory();

public:
	

public:
	HRESULT	Initialize();
	void	Tick(_float fTimeDelta);

private:
	CCustomContextManagerCallback* m_pGraphicsContextManager{};
	CCustomPxAllocatorCallback* m_pAllocatorCallback{};
	CCustomPxErrorCallback* m_pErrorCallback{};
	CCustomPxAssertHandler* m_pAssertHander{};
	Factory* m_pFactory{};
	Solver* m_pSolver{};

public:
	virtual void OnDestroy() override;

private:
	void Free();
};

END