#include "..\Public\Object_Manager.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObject_Manager);

HRESULT CObject_Manager::Reserve_Container(_uint iNumLevels)
{
	if (0 != m_pLayers.size())
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	for (_uint i = 0; i < iNumLevels; i++)
		m_pLayers.push_back({});
	
	return S_OK;
}

void CObject_Manager::OnLevelEnter()
{
	Register_ReservedObjects();

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			for (auto& elem_GameObject : Pair.second)
			{
				elem_GameObject->OnLevelEnter();
			}
		}
	}
}

void CObject_Manager::OnLevelExit()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			for (auto& elem_GameObject : Pair.second)
			{
				elem_GameObject->OnLevelExit();
			}
		}
	}
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	CallBack_Start();
	CallBack_Start.Clear();

	_bool bThread;

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			for (auto& elem_GameObject : Pair.second)
			{
				bThread = elem_GameObject->Is_Thread(THREAD_TYPE::TICK);

				//게임오브젝트가 활성화 상태면 Tick을 돌린다.
				if (elem_GameObject->Get_Enable() && !bThread)
				{
					elem_GameObject->Tick(fTimeDelta);

				}
			}
		}
	}
}

void CObject_Manager::LateTick(_float fTimeDelta)
{
	_bool bThread;

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			for (auto& elem_GameObject : Pair.second)
			{
				bThread = elem_GameObject->Is_Thread(THREAD_TYPE::LATETICK);

				//게임오브젝트가 활성화 상태면 LateTick을 돌린다.
				if (elem_GameObject->Get_Enable() && !bThread)
				{
					elem_GameObject.get()->LateTick(fTimeDelta);
				}
			}
		}
	}
}

void CObject_Manager::Before_Render(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			for (auto& elem_GameObject : Pair.second)
			{
				//게임오브젝트가 활성화 상태면 LateTick을 돌린다.
				if (elem_GameObject->Get_Enable())
					elem_GameObject.get()->Before_Render(fTimeDelta);
			}
		}
	}
}

void CObject_Manager::After_Render()
{
	Remove_DeadObject();
	Register_ReservedObjects();
}

void CObject_Manager::OnDestroy()
{

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		Delete_GameObjectInstances(i);
	}

	m_Prototypes.clear();

}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		for (auto& elem_GameObject : Pair.second)
		{
			/*if (elem_GameObject.get())
				elem_GameObject->OnDestroy();*/

			elem_GameObject->Set_Dead();
		}
	}

	// m_pLayers[iLevelIndex].clear();
}

void CObject_Manager::Delete_GameObjectInstances(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		for (auto& elem_GameObject : Pair.second)
		{
			if (elem_GameObject.get())
				elem_GameObject->OnDestroy();

			elem_GameObject.reset();
		}
	}

	 m_pLayers[iLevelIndex].clear();
}

void CObject_Manager::Remove_DeadObject()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			for (auto iter = Pair.second.begin(); iter != Pair.second.end();)
			{
				if ((*iter)->Get_Dead())
				{
					(*iter)->OnDestroy();
					iter = Pair.second.erase(iter);
				}
				else
				{
					iter++;
				}
			}
		}
	}

}

shared_ptr<CGameObject> CObject_Manager::Find_Object(weak_ptr<CGameObject> pGameObject)
{
	return shared_ptr<CGameObject>();
}

void CObject_Manager::Register_ReservedObjects()
{
	for (auto& elem : m_ReservedObjects)
	{
		if (elem.pReservedObject)
		{
			m_pLayers[elem.iLevelIndex][elem.hashcode].push_back(elem.pReservedObject);
			CallBack_Start += bind(&CGameObject::Start, elem.pReservedObject);
		}
			
	}

	m_ReservedObjects.clear();
}

void CObject_Manager::Bind_Thread(shared_ptr<CGameObject> pGameObject)
{
	for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		if (pGameObject->Is_Thread((THREAD_TYPE)i))
		{
			GET_SINGLE(CThread_Manager)->Bind_ThreadObject((THREAD_TYPE)i, pGameObject);
		}
	}

}

void CObject_Manager::Free()
{

}

void CObject_Manager::OnEngineEventMessage(const ENGINE_EVENT_TYPE In_eEngineEvent)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			for (auto iter = Pair.second.begin(); iter != Pair.second.end();)
			{
				if ((*iter))
				{
					(*iter)->OnEngineEventMessage(In_eEngineEvent);
				}
				++iter;
			}
		}
	}
}

weak_ptr<CGameObject> CObject_Manager::Add_GameObject(size_t iTypeHash, _uint iLevelIndex, void* pArg)
{
	if (m_iNumLevels <= iLevelIndex)
	{
		//잘못된 레벨 인덱스
#ifdef _DEBUG
		assert(false);
#endif
		return weak_ptr<CGameObject>();
	}

	weak_ptr<CGameObject> pPrototype;

	auto iter = m_Prototypes.find(iTypeHash);

	if (iter != m_Prototypes.end())
	{
		pPrototype = (*iter).second;
	}

	if (0 == pPrototype.use_count())
	{
		return weak_ptr<CGameObject>();
	}


	if (0 == pPrototype.use_count())
		return weak_ptr<CGameObject>();

	shared_ptr<CGameObject> pCloneObject = pPrototype.lock()->Clone(iLevelIndex, pArg);

	/*for (_uint i = 0; i < (_uint)THREAD_TYPE::TYPE_END; ++i)
	{
		if (pCloneObject->Is_Thread((THREAD_TYPE)i))
		{
			GET_SINGLE(CThread_Manager)->Bind_ThreadObject((THREAD_TYPE)i, pCloneObject);
		}
	}*/

	m_ReservedObjects.push_back({ iTypeHash, iLevelIndex, pCloneObject });

	return pCloneObject;
}