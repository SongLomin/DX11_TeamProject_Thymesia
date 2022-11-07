#include "..\Public\Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObject_Manager);

//#define IMPLEMENT_SINGLETON(ClassName)									
//unique_ptr<CObject_Manager> CObject_Manager::m_pInstance;
//
//unique_ptr<CObject_Manager> CObject_Manager::Get_Instance()
//{																		
//	if (nullptr == m_pInstance.get())									
//		m_pInstance = make_unique<CObject_Manager>();
//																		
//	return m_pInstance;													
//}																		
//void CObject_Manager::Destroy_Instance()
//{																		
//	m_pInstance.reset();												
//}																		


//
//CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
//{
//	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
//
//	if (nullptr == pLayer)
//		return nullptr;
//
//	return pLayer->Get_Component(pComponentTag, iIndex);	
//}

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

//HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
//{
//	if (nullptr != Find_Prototype(pPrototypeTag))
//		return E_FAIL;
//
//	m_Prototypes.emplace(pPrototypeTag, pPrototype);
//
//	return S_OK;
//}
//
//HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void* pArg)
//{
//	CGameObject*		pPrototype = Find_Prototype(pPrototypeTag);
//	if (nullptr == pPrototype)
//		return E_FAIL;
//
//	CGameObject*		pCloneObject = pPrototype->Clone(pArg);
//	if (nullptr == pCloneObject)
//		return E_FAIL;
//
//	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
//
//	if (nullptr == pLayer)
//	{
//		pLayer = CLayer::Create();
//		pLayer->Add_GameObject(pCloneObject);
//
//		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
//	}
//	else
//		pLayer->Add_GameObject(pCloneObject);	
//
//	return S_OK;
//}

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

weak_ptr<CGameObject> CObject_Manager::Add_GameObject(size_t iTypeHash, _uint iLevelIndex, void* pArg, _bool _bMemoryPool)
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

	/*if (_bMemoryPool)
	{
		for (auto& elem : m_pLayers[iLevelIndex][HashCode])
		{
			if (!elem->Get_Enable())
			{
				elem->Set_Enable(true, pArg);

				return elem;
			}
		}
	}*/

	if (0 == pPrototype.use_count())
		return weak_ptr<CGameObject>();

	shared_ptr<CGameObject> pCloneObject = pPrototype.lock()->Clone(iLevelIndex, pArg);
	//pCloneObject->Set_OwnerForMyComponents();

	m_pLayers[iLevelIndex][iTypeHash].push_back(pCloneObject);

	/*if (pParent)
	{
		CTransform* pTransfromCom = pCloneObject->Get_Component<CTransform>();
		pTransfromCom->Set_Parent(pParent);
		pParent->Add_Child(pTransfromCom);
	}*/
	//pCloneObject->OnEnable(pArg);

	return pCloneObject;
}


//CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
//{
//	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));
//
//	if (iter == m_Prototypes.end())
//		return nullptr;
//
//	return iter->second;
//}
//
//CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
//{
//	if (iLevelIndex >= m_iNumLevels)
//		return nullptr;
//
//	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTag_Finder(pLayerTag));
//
//	if (iter == m_pLayers[iLevelIndex].end())
//		return nullptr;
//
//	return iter->second;	
//}

//void CObject_Manager::Free()
//{
//	for (_uint i = 0; i < m_iNumLevels; ++i)
//	{
//		/*for (auto& Pair : m_pLayers[i])
//		{
//			for (auto& elem_GameObject : Pair.second)
//			{
//				Safe_Release(elem_GameObject);
//			}
//			int i = 0;
//		}*/
//		m_pLayers[i].clear();
//	}
//
//	/*for (auto& Pair : m_Prototypes)
//		Safe_Release(Pair.second);*/
//
//	m_Prototypes.clear();
//	
//}
