#pragma once
#include "Base.h"
#include "GameObject_Thread.h"

BEGIN(Engine)

class CGameObject;

class CThread_Manager final : public CBase
{
	DECLARE_SINGLETON(CThread_Manager)


private:
	struct GAMEOBJECT_THREAD
	{
		future<void> Future;
		shared_ptr<CGameObject_Thread> pInstance;
	};

public:
	void Initialize(const _uint In_iNumLayer);
	void Bind_ThreadObject(const THREAD_TYPE In_eThread_Type, weak_ptr<CGameObject> pGameObject);
	void Bind_GameObjectWorks();

	_bool Check_JobDone();

private:

	void Clear_EngineThreads(const THREAD_TYPE In_eThread_Type);

	void WorkerThread();
	
	


public:

	GAMEOBJECT_THREAD			m_GameObject_Threads[(_uint)THREAD_TYPE::TYPE_END];

private:


	// 총 Worker 쓰레드의 개수.
	size_t num_threads_;
	// Worker 쓰레드를 보관하는 벡터.
	std::vector<std::thread> worker_threads_;
	// 할일들을 보관하는 job 큐.
	std::queue<std::function<void()>> jobs_;
	// 위의 job 큐를 위한 cv 와 m.
	std::condition_variable cv_job_q_;
	std::mutex m_job_q_;

	// 모든 쓰레드 종료
	bool stop_all;


public:
	virtual void OnDestroy() override;
	void Free();
	

public:
	template <class F, class... Args>
	std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(
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
