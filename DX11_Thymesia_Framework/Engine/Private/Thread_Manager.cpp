#include "Thread_Manager.h"
#include "GameObject.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CThread_Manager)


mutex g_Mutex;



void Loop(const THREAD_TYPE In_eThread_Type)
{


	shared_ptr<CTimer> pTimer = CTimer::Create();
	_float fTimeDelta;
	_uint	iLoopIndex = -1;

	weak_ptr<CGameInstance> pGameInstance = GAMEINSTANCE;
	iLoopIndex = pGameInstance.lock()->Get_LoopIndex();

	list<weak_ptr<CGameObject>>	ThreadObjects;

	_bool bDead = false;

	while (true)
	{
		std::scoped_lock(g_Mutex);

		bDead = GET_SINGLE(CThread_Manager)->m_bDead;

		if (bDead)
			break;

		if (pGameInstance.lock()->Get_LoopIndex() == iLoopIndex)
			continue;

		fTimeDelta = pTimer->Compute_Timer();

		//cout << "Thread! : " << (_uint)In_eThread_Type << endl;

		for (auto iter = ThreadObjects.begin(); iter != ThreadObjects.end();)
		{
			if (!(*iter).lock())
			{
				iter = ThreadObjects.erase(iter);
			}
			else
			{
				switch (In_eThread_Type)
				{
				case THREAD_TYPE::TICK:
					(*iter).lock()->Tick(fTimeDelta);
					break;

				case THREAD_TYPE::LATE_TICK:
					(*iter).lock()->LateTick(fTimeDelta);
					break;

				case THREAD_TYPE::BEFORE_RENDER:
					(*iter).lock()->Before_Render(fTimeDelta);
					break;

				default:
					break;
				}

				++iter;
			}
		}

		//GET_SINGLE(CThread_Manager)->m_ReservedThreadObjects[(_uint)In_eThread_Type];

		for (auto& elem : GET_SINGLE(CThread_Manager)->m_ReservedThreadObjects[(_uint)In_eThread_Type])
		{
			cout << "Thread [" << (_uint)In_eThread_Type << "] Push_Back" << endl;
			//ThreadObjects.push_back(elem);
		}


		//GET_SINGLE(CThread_Manager)->m_ReservedThreadObjects[In_eThread_Type].clear();

		//Update(In_eThread_Type, ThreadObjects, fTimeDelta);

		//GET_SINGLE(CThread_Manager)->Add_ThreadObject(In_eThread_Type, ThreadObjects);

		iLoopIndex = pGameInstance.lock()->Get_LoopIndex();

	}

	cout << "Thread [" << (_uint)In_eThread_Type << "] Break" << endl;

	pTimer.reset();

	for (auto& elem : ThreadObjects)
	{
		elem.reset();
	}

	ThreadObjects.clear();

	pGameInstance.reset();

	/*while (true)
	{

		if (m_bDead)
			break;

		cout << "Thread! : " << (_uint)In_eThread_Type << endl;


	}*/

}

void CThread_Manager::Initialize(const _uint In_iNumLayer)
{
	weak_ptr<CGameInstance> pGameInstance = GAMEINSTANCE;

	/*for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		function<void(const THREAD_TYPE)> function;
		function = bind(&CThread_Manager::Loop, this, placeholders::_1);

		m_Threads.push_back(async(launch::async, function, (THREAD_TYPE)i));
	}*/

	for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		m_Threads.push_back(async(launch::async, Loop, (THREAD_TYPE)i));
	}

	

}

void CThread_Manager::Update(const THREAD_TYPE In_eThread_Type, list<weak_ptr<CGameObject>>& In_List, _float fTimeDelta)
{
	for (auto iter = In_List.begin(); iter != In_List.end();)
	{
		if (!(*iter).lock())
		{
			iter = In_List.erase(iter);
		}
		else
		{
			switch (In_eThread_Type)
			{
			case THREAD_TYPE::TICK:
				(*iter).lock()->Tick(fTimeDelta);
				break;

			case THREAD_TYPE::LATE_TICK:
				(*iter).lock()->LateTick(fTimeDelta);
				break;

			case THREAD_TYPE::BEFORE_RENDER:
				(*iter).lock()->Before_Render(fTimeDelta);
				break;

			default:
				break;
			}

			++iter;
		}
	}

}

void CThread_Manager::Add_ThreadObject(const THREAD_TYPE In_eThread_Type, list<weak_ptr<CGameObject>>& In_List)
{
	

	for (auto& elem : GET_SINGLE(CThread_Manager)->m_ReservedThreadObjects[(_uint)In_eThread_Type])
	{
		In_List.push_back(elem);
	}

	GET_SINGLE(CThread_Manager)->m_ReservedThreadObjects[(_uint)In_eThread_Type].clear();
}

void CThread_Manager::Bind_ThreadObject(const THREAD_TYPE In_eThread_Type, weak_ptr<CGameObject> pGameObject)
{
	m_ReservedThreadObjects[(_uint)In_eThread_Type].emplace_back(pGameObject);
}

void CThread_Manager::Clear_EngineThreads(const THREAD_TYPE In_eThread_Type)
{
	
	for (auto& elem : m_ReservedThreadObjects[(_uint)In_eThread_Type])
	{
		elem.reset();
	}

	m_ReservedThreadObjects[(_uint)In_eThread_Type].clear();
}

void CThread_Manager::OnDestroy()
{
	m_bDead = true;

	for (auto& Thread : m_Threads)
	{
		Thread.wait();
	}

	for (auto& Thread : m_Threads)
	{
		Thread.get();
	}

	//WaitForSecon

	for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		Clear_EngineThreads((THREAD_TYPE)i);
	}

	//m_ReservedThreadObjects.clear();
	
	m_Threads.clear();
}

void CThread_Manager::Free()
{
	int t = 0;
}
