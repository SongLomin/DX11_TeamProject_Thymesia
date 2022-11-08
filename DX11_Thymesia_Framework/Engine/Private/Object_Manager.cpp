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
	


	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			for (auto& elem_GameObject : Pair.second)
			{
				//게임오브젝트가 활성화 상태면 LateTick을 돌린다.
				if (elem_GameObject->Get_Enable())
					elem_GameObject.get()->Tick(fTimeDelta);
			}
		}
	}
}

void CObject_Manager::LateTick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			for (auto& elem_GameObject : Pair.second)
			{
				//게임오브젝트가 활성화 상태면 LateTick을 돌린다.
				if (elem_GameObject->Get_Enable())
					elem_GameObject.get()->LateTick(fTimeDelta);
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
		Clear(i);
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
			if (elem_GameObject.get())
				elem_GameObject->OnDestroy();
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

void CObject_Manager::Register_ReservedObjects()
{
	for (auto& elem : m_ReservedObjects)
	{
		if(elem.pReservedObject)
			m_pLayers[elem.iLevelIndex][elem.hashcode].push_back(elem.pReservedObject);
	}

	m_ReservedObjects.clear();
}

void CObject_Manager::Free()
{

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

	m_pLayers[iLevelIndex][iTypeHash].push_back(pCloneObject);

	return pCloneObject;
}