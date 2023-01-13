#include "SubThread_Pool.h"

GAMECLASS_C(CSubThread_Pool)

void CSubThread_Pool::Initialize(const _uint In_iSize)
{
	num_threads_ = In_iSize;
	stop_all = false;

	worker_jopdones = vector<_bool>(In_iSize, false);

	worker_threads_.reserve(num_threads_);
	for (size_t i = 0; i < num_threads_; ++i) {
		//_bool* bCheck_JobDone = DBG_NEW _bool(true);
		worker_threads_.emplace_back([this, i]() { this->WorkerThread(i); });
	}

}

void CSubThread_Pool::WorkerThread(_int iIndex)
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

_bool CSubThread_Pool::Check_JobDone()
{
	cv_job_q_.notify_all();

	for (auto& elem : worker_jopdones)
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
	cv_job_q_.notify_all();
	Wait_JobDone("Wait For Release SubThread. ");

	for (auto& t : worker_threads_) {
		t.join();
	}
}
