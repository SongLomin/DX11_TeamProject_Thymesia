#include "GameObject_Thread.h"
#include "GameObject.h"
#include "Timer.h"


void CGameObject_Thread::Add_ReserveThreadObject(weak_ptr<CGameObject> pGameObject)
{
	m_ReservedThreadObjects.emplace_back(pGameObject);
}

void CGameObject_Thread::Bind_Works()
{
	shared_ptr<CThread_Manager> pThreadManager = GET_SINGLE(CThread_Manager);

	for (auto iter = m_ThreadObjects.begin(); iter != m_ThreadObjects.end();)
	{
		if (!(*iter).lock())
		{
			iter = m_ThreadObjects.erase(iter);
			continue;
		}
		else if ((*iter).lock()->Get_Enable())
		{
			pThreadManager->Enqueue_Job(bind(&CGameObject_Thread::Do, this, (*iter)));
		}
		++iter;
	}

}

void CGameObject_Thread::Add_ThreadObject(weak_ptr<CGameObject> pGameObject)
{
	m_ThreadObjects.emplace_back(pGameObject);
}

future<void> CGameObject_Thread::Async(const THREAD_TYPE In_eThread_Type)
{
	if (m_bEnable)
		return future<void>();

	m_bEnable = true;

	return async(launch::async, bind(&CGameObject_Thread::Loop, this, placeholders::_1), In_eThread_Type);
}

void CGameObject_Thread::Loop(const THREAD_TYPE In_eThread_Type)
{
	shared_ptr<CTimer> pTimer = CTimer::Create();
	_float fTimeDelta;
	_uint	iLoopIndex = -1;

	weak_ptr<CGameInstance> pGameInstance = GAMEINSTANCE;
	iLoopIndex = pGameInstance.lock()->Get_LoopIndex();

	shared_ptr<CThread_Manager> pThreadManager = GET_SINGLE(CThread_Manager);

	while (true)
	{
		if (!m_bEnable)
			break;

		if (pGameInstance.lock()->Get_LoopIndex() == iLoopIndex)
		{
			continue;
		}

		fTimeDelta = pTimer->Compute_Timer();


		for (auto iter = m_ThreadObjects.begin(); iter != m_ThreadObjects.end();)
		{
			if (!(*iter).lock())
			{
				iter = m_ThreadObjects.erase(iter);
			}
			else if ((*iter).lock()->Get_Enable())
			{
				switch (In_eThread_Type)
				{
				case THREAD_TYPE::TICK:
					(*iter).lock()->Tick(fTimeDelta);
					break;

				case THREAD_TYPE::LATETICK:
					(*iter).lock()->LateTick(fTimeDelta);
					break;

				case THREAD_TYPE::PRE_TICK:
					(*iter).lock()->Thread_PreTick(fTimeDelta);
					break;

				case THREAD_TYPE::PRE_LATETICK:
					(*iter).lock()->Thread_PreLateTick(fTimeDelta);
					break;

				case THREAD_TYPE::PRE_BEFORERENDER:
					(*iter).lock()->Thread_PreBeforeRender(fTimeDelta);
					break;

				case THREAD_TYPE::PRE_RENDER:
					(*iter).lock()->Thread_PreRender(fTimeDelta);
					break;

				default:
					break;
				}
				++iter;
			}
			else
			{
				++iter;
			}


		}

		for (auto& elem : m_ReservedThreadObjects)
		{
			m_ThreadObjects.push_back(elem);
		}


		m_ReservedThreadObjects.clear();

		iLoopIndex = pGameInstance.lock()->Get_LoopIndex();

	}

	pTimer.reset();

	for (auto& elem : m_ThreadObjects)
	{
		elem.reset();
	}

	m_ThreadObjects.clear();

	pGameInstance.reset();
}

void CGameObject_Thread::Do(weak_ptr<CGameObject> pGameObject)
{
	_float fTimeDelta = GAMEINSTANCE->Get_DeltaTime();

	switch (m_eThreadType)
	{
	case THREAD_TYPE::TICK:
		pGameObject.lock()->Tick(fTimeDelta);
		break;

	case THREAD_TYPE::LATETICK:
		pGameObject.lock()->LateTick(fTimeDelta);
		break;

	case THREAD_TYPE::PRE_TICK:
		pGameObject.lock()->Thread_PreTick(fTimeDelta);
		break;

	case THREAD_TYPE::PRE_LATETICK:
		pGameObject.lock()->Thread_PreLateTick(fTimeDelta);
		break;

	case THREAD_TYPE::PRE_BEFORERENDER:
		pGameObject.lock()->Thread_PreBeforeRender(fTimeDelta);
		break;

	case THREAD_TYPE::PRE_RENDER:
		pGameObject.lock()->Thread_PreRender(fTimeDelta);
		break;

	default:
		break;
	}

}

shared_ptr<CGameObject_Thread> CGameObject_Thread::Create(const THREAD_TYPE In_eThread_Type)
{
	shared_ptr<CGameObject_Thread> pInstance = make_shared<CGameObject_Thread>();
	pInstance->Initialize(nullptr);
	pInstance->m_eThreadType = In_eThread_Type;

	return pInstance;
}
