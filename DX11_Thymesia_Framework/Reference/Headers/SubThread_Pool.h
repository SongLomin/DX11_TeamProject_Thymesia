#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSubThread_Pool :
    public CBase
{
    GAMECLASS_H(CSubThread_Pool);

public:
	static shared_ptr<CSubThread_Pool> Create(const _uint In_iSize);

private:
    void Initialize(const _uint In_iSize);
	void WorkerThread(_int iIndex);
public:
	_bool Check_JobDone();
	void Wait_JobDone(const _char* In_szConsoleText = nullptr);
	void Stop_All() { stop_all = true; }

private:
	// 총 Worker 쓰레드의 개수.
	size_t num_threads_;
	// Worker 쓰레드를 보관하는 벡터.
	vector<thread> worker_threads_;
	// 할일들을 보관하는 job 큐.
	queue<function<void()>> jobs_;
	// 위의 job 큐를 위한 cv 와 m.
	condition_variable cv_job_q_;
	mutex m_job_q_;
	vector<_bool> worker_jopdones;
	// 모든 쓰레드 종료
	bool stop_all;




private:
    void Free();

public:
	template <class F, class... Args>
	std::future<typename std::result_of<F(Args...)>::type> Enqueue_Job(
		F&& f, Args&&... args)
	{
		if (stop_all) {
			throw std::runtime_error("ThreadPool 사용 중지됨");
		}

		using return_type = typename std::invoke_result_t<F, Args...>;
		auto job = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		std::future<return_type> job_result_future = job->get_future();
		{
			std::lock_guard<std::mutex> lock(m_job_q_);
			jobs_.push([job]() { (*job)(); });
		}
		cv_job_q_.notify_one();

		return job_result_future;
	}
};

END