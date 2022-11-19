#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;

class CThread_Manager final : public CBase
{
	DECLARE_SINGLETON(CThread_Manager)

public:
	void Initialize(const _uint In_iNumLayer);
	void Bind_ThreadObject(const THREAD_TYPE In_eThread_Type, weak_ptr<CGameObject> pGameObject);

private:
	//void Loop(const THREAD_TYPE In_eThread_Type);

	void Clear_EngineThreads(const THREAD_TYPE In_eThread_Type);

public:
	


public:
	list<weak_ptr<CGameObject>>	m_ReservedThreadObjects[(_uint)THREAD_TYPE::TYPE_END];

	list<future<void>>				m_Threads;

	_bool								m_bDead = false;
	_uint							m_ThreadEnableFlag = 0;

public:
	virtual void OnDestroy() override;
	void Free();
};

END