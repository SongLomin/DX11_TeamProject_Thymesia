#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;

class CThread_Manager final : public CBase
{
	DECLARE_SINGLETON(CThread_Manager)

public:
	void Initialize(const _uint In_iNumLayer);
	
public:

	void Bind_EngineThread(const THREAD_TYPE In_eThread_Type, function<void(_float)> Function, _float fTimeDelta);
	void Wait_EngineThread(const THREAD_TYPE In_eThread_Type);

	void Clear_EngineThreads(const THREAD_TYPE In_eThread_Type);

public:
	


private:
	map<THREAD_TYPE, list<future<void>>>	m_EngineThreads;
	map<_uint, list<future<void>>>			m_CustomThreads;


public:
	void Free();
};

END