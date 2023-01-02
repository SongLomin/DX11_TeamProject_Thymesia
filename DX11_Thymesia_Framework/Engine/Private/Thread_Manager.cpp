#include "Thread_Manager.h"
#include "GameObject.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CThread_Manager)

void CThread_Manager::Initialize(const _uint In_iNumLayer)
{
	num_threads_ = In_iNumLayer;
	stop_all = false;

	worker_jopdones = vector<_bool>(In_iNumLayer, false);

	worker_threads_.reserve(num_threads_);
	for (size_t i = 0; i < num_threads_; ++i) {
		//_bool* bCheck_JobDone = DBG_NEW _bool(true);
		worker_threads_.emplace_back([this, i]() { this->WorkerThread(i); });
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
		std::unique_lock<std::mutex> lock(m_job_q_);
		cv_job_q_.wait(lock, [this, iIndex, &bWait]() {
			bWait = !this->jobs_.empty() || stop_all;
			worker_jopdones[iIndex] = !bWait;
			return bWait;
			});
		if (stop_all && this->jobs_.empty()) {
			worker_jopdones[iIndex] = true;
			return;
		}

		worker_jopdones[iIndex] = false;
		// 맨 앞의 job 을 뺀다.
		std::function<void()> job = std::move(jobs_.front());
		jobs_.pop();
		lock.unlock();
		worker_jopdones[iIndex] = false;
		// 해당 job 을 수행한다 :)
		job();
	}
}

_bool CThread_Manager::Check_JobDone()
{
	cv_job_q_.notify_all();

	for (auto& elem : worker_jopdones)
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
	cv_job_q_.notify_all();



	Wait_JobDone("Wait For Release Thread. ");


	for (auto& t : worker_threads_) {
		t.join();
	}




	for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		Clear_EngineThreads((THREAD_TYPE)i);
	}

	/*for (auto& elem : worker_jopdones)
	{
		Safe_Delete(elem);
	}*/

	worker_jopdones.clear();

}

void CThread_Manager::Free()
{

}
