#include "SubThread_Pool.h"

GAMECLASS_C(CSubThread_Pool)

void CSubThread_Pool::Initialize(const _uint In_iSize)
{
	m_iNumThreads = In_iSize;
	stop_all = false;

	m_WorkerJopdones = vector<_bool>(In_iSize, false);

	m_Worker_Threads.reserve(m_iNumThreads);
	for (size_t i = 0; i < m_iNumThreads; ++i) {
		//_bool* bCheck_JobDone = DBG_NEW _bool(true);
		m_Worker_Threads.emplace_back([this, i]() { this->WorkerThread(i); });
	}

}

void CSubThread_Pool::WorkerThread(_int iIndex)
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

_bool CSubThread_Pool::Check_JobDone()
{
	m_CV.notify_all();

	for (auto& elem : m_WorkerJopdones)
	{
		if (!(elem))
			return false;
	}

	return true;
}

void CSubThread_Pool::Wait_JobDone(const _char* In_szConsoleText)
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

shared_ptr<CSubThread_Pool> CSubThread_Pool::Create(const _uint In_iSize)
{
	shared_ptr<CSubThread_Pool> pInstance = make_shared<CSubThread_Pool>();

	pInstance->Initialize(In_iSize);

	return pInstance;
}

void CSubThread_Pool::Free()
{
	stop_all = true;
	m_CV.notify_all();
	Wait_JobDone("Wait For Release SubThread. ");

	for (auto& t : m_Worker_Threads) {
		t.join();
	}
}
