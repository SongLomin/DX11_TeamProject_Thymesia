#include "Thread_Manager.h"
#include "GameObject.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CThread_Manager)

void CThread_Manager::Initialize(const _uint In_iNumLayer)
{
	num_threads_ = In_iNumLayer;
	stop_all = false;


	worker_threads_.reserve(num_threads_);
	for (size_t i = 0; i < num_threads_; ++i) {
		worker_threads_.emplace_back([this]() { this->WorkerThread(); });
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

void CThread_Manager::Bind_GameObjectWorks()
{
	for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		if (m_GameObject_Threads[i].pInstance)
		{
			m_GameObject_Threads[i].pInstance->Bind_Works();
		}
	}

}

void CThread_Manager::Clear_EngineThreads(const THREAD_TYPE In_eThread_Type)
{	
	if (m_GameObject_Threads[(_uint)In_eThread_Type].pInstance)
	{
		m_GameObject_Threads[(_uint)In_eThread_Type].pInstance->Set_Enable(false);
	}

}

void CThread_Manager::WorkerThread()
{
	while (true) {
		std::unique_lock<std::mutex> lock(m_job_q_);
		cv_job_q_.wait(lock, [this]() { return !this->jobs_.empty() || stop_all; });
		if (stop_all && this->jobs_.empty()) {
			return;
		}

		// 맨 앞의 job 을 뺀다.
		std::function<void()> job = std::move(jobs_.front());
		jobs_.pop();
		lock.unlock();

		// 해당 job 을 수행한다 :)
		job();
	}
}

_bool CThread_Manager::Check_JobDone()
{
	return jobs_.empty();
}

void CThread_Manager::OnDestroy()
{

	for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		Clear_EngineThreads((THREAD_TYPE)i);
	}

	for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		if (m_GameObject_Threads->pInstance)
		{
			m_GameObject_Threads->Future.wait();
		}
	}

	for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		if (m_GameObject_Threads->pInstance)
		{
			m_GameObject_Threads->Future.get();
		}
	}

	stop_all = true;
	cv_job_q_.notify_all();

	for (auto& t : worker_threads_) {
		t.join();
	}

}

void CThread_Manager::Free()
{

}
