#pragma once
#include "Base.h"
#include "GameObject_Thread.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CThread_Manager final : public CBase
{
	DECLARE_SINGLETON(CThread_Manager)


private:
	struct GAMEOBJECT_THREAD
	{
		future<void> Future;
		shared_ptr<CGameObject_Thread> pInstance;
	};

public:
	_uint Get_NumThread() const { return m_iNumThreads; }

public:
	void Initialize(const _uint In_iNumLayer);
	void Bind_ThreadObject(const THREAD_TYPE In_eThread_Type, weak_ptr<CGameObject> pGameObject);
	void Bind_GameObjectWorks(const _flag In_ThreadTypeFlag);

	_bool Check_JobDone();
	void Wait_JobDone(const _char* In_szConsoleText = nullptr);

private:

	void Clear_EngineThreads(const THREAD_TYPE In_eThread_Type);

	void Work_Thread(_int iIndex);
	
	


public:

	GAMEOBJECT_THREAD			m_GameObject_Threads[(_uint)THREAD_TYPE::TYPE_END];

private:
	size_t m_iNumThreads;
	vector<thread> m_Worker_Threads;
	queue<function<void()>> m_Jobs;
	condition_variable m_CV;
	mutex m_JobMutex;
	vector<_bool> m_WorkerJobdones;

	bool stop_all;


public:
	virtual void OnDestroy() override;
	void Free();
	

public:
	template <class Func, class... Args>
	void Enqueue_Job(
		Func&& function, Args&&... args)
	{
		{
			lock_guard<mutex> lock(m_JobMutex);
			m_Jobs.push(bind(forward<Func>(function), forward<Args>(args)...));
		}
		m_CV.notify_one();

		return;
	}
};

END
