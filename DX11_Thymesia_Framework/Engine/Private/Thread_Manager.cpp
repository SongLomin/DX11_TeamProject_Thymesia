#include "Thread_Manager.h"
#include "GameObject.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CThread_Manager)

void CThread_Manager::Initialize(const _uint In_iNumLayer)
{
	m_iNumThreads = In_iNumLayer;
	stop_all = false;

	m_WorkerJopdones = vector<_bool>(In_iNumLayer, false);

	m_Worker_Threads.reserve(m_iNumThreads);
	for (size_t i = 0; i < m_iNumThreads; ++i) {
		//_bool* bCheck_JobDone = DBG_NEW _bool(true);
		m_Worker_Threads.emplace_back([this, i]() { this->WorkerThread(i); });
	}
}


void CThread_Manager::Bind_ThreadObject(const THREAD_TYPE In_eThread_Type, weak_ptr<CGameObject> pGameObject)
{
	if (!m_GameObject_Threads[(_uint)In_eThread_Type].pInstance)
	{
		m_GameObject_Threads[(_uint)In_eThread_Type].pInstance = CGameObject_Thread::Create(In_eThread_Type);
		m_GameObject_Threads[(_uint)In_eThread_Type].pInstance->Add_ThreadObject(pGameObject);
		//m_GameObject_Threads[(_uint)In_eThread_Type].pInstance->Add_ReserveThreadObject(pGameObject);
		//m_GameObject_Threads[(_uint)In_eThread_Type].Future = m_GameObject_Threads[(_uint)In_eThread_Type].pInstance->Async(In_eThread_Type);
	}
	else
	{
		m_GameObject_Threads[(_uint)In_eThread_Type].pInstance->Add_ThreadObject(pGameObject);
	}
}

void CThread_Manager::Bind_GameObjectWorks(const _flag In_ThreadTypeFlag)
{
	for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		if (In_ThreadTypeFlag & ((_flag)1 << (_flag)i))
		{
			if (m_GameObject_Threads[i].pInstance)
			{
				m_GameObject_Threads[i].pInstance->Bind_Works();
			}
		}
	}

}

void CThread_Manager::Clear_EngineThreads(const THREAD_TYPE In_eThread_Type)
{	
	if (m_GameObject_Threads[(_uint)In_eThread_Type].pInstance)
	{
		m_GameObject_Threads[(_uint)In_eThread_Type].pInstance->Set_Enable(false);
		m_GameObject_Threads[(_uint)In_eThread_Type].pInstance.reset();
	}

}

void CThread_Manager::WorkerThread(_int iIndex)
{
	_bool bWait = false;

	while (true) {
		std::unique_lock<std::mutex> lock(m_JobMutex);
		m_CV.wait(lock, [this, iIndex, &bWait]() {
			bWait = !this->m_Jobs.empty() || stop_all;
			m_WorkerJopdones[iIndex] = !bWait;
			return bWait;
			});
		if (stop_all && this->m_Jobs.empty()) {
			m_WorkerJopdones[iIndex] = true;
			return;
		}

		m_WorkerJopdones[iIndex] = false;
		// 맨 앞의 job 을 뺀다.
		std::function<void()> job = std::move(m_Jobs.front());
		m_Jobs.pop();
		lock.unlock();
		m_WorkerJopdones[iIndex] = false;
		// 해당 job 을 수행한다 :)
		job();
	}
}

_bool CThread_Manager::Check_JobDone()
{
	m_CV.notify_all();

	for (auto& elem : m_WorkerJopdones)
	{
		if (!(elem))
			return false;
	}

	return true;
}

void CThread_Manager::Wait_JobDone(const _char* In_szConsoleText)
{

	while (!Check_JobDone())
	{
#ifdef _DEBUG
		if (In_szConsoleText)
		{
			cout << In_szConsoleText << endl;
		}
#endif // _DEBUG

		continue;
	}

	while (!Check_JobDone())
	{
#ifdef _DEBUG
		if (In_szConsoleText)
		{
			cout << In_szConsoleText << endl;
		}
#endif // _DEBUG

		continue;
	}

}

void CThread_Manager::OnDestroy()
{

	

	stop_all = true;
	m_CV.notify_all();



	Wait_JobDone("Wait For Release Thread. ");


	for (auto& t : m_Worker_Threads) {
		t.join();
	}




	for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		Clear_EngineThreads((THREAD_TYPE)i);
	}

	/*for (auto& elem : m_WorkerJopdones)
	{
		Safe_Delete(elem);
	}*/

	m_WorkerJopdones.clear();

}

void CThread_Manager::Free()
{

}
