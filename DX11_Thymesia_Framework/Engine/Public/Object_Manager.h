#pragma once

/* ��ü���� ������ �����ϳ�. */
/* ������ü�� ã�Ƽ� �����Ͽ� ��(����)���� ��Ƽ� �����ϴ�.*/

#include "Base.h"

BEGIN(Engine)

class CGameObject;

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
	
private:
	struct RESERVED_OBEJECT_DESC
	{
		_hashcode	hashcode;
		_uint		iLevelIndex;
		shared_ptr<CGameObject> pReservedObject;
	};

public:
	//class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);

public:
	HRESULT Reserve_Container(_uint iNumLevels);
	
	virtual void OnLevelEnter() override;
	virtual void OnLevelExit() override;

public:
	/* ������ü�� �߰��Ѵ�. */
	//HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);

	/* ������ü�����Ӥ� �����Ͽ� �纻��ü�� �߰��Ѵ�. */
	//HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);

	

	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void After_Render();

	virtual void OnDestroy() override;

public:
	void Clear(_uint iLevelIndex);
	void Remove_DeadObject();

private:
	void Register_ReservedObjects();

protected:
	virtual void Free() override;

private:
	map<_hashcode, class shared_ptr<CGameObject>>			m_Prototypes;
	typedef map<_hashcode, class shared_ptr<CGameObject>>	PROTOTYPES;

private:
	vector<map<_hashcode, list<shared_ptr<CGameObject>>>>				m_pLayers;
	typedef map<_hashcode, list<shared_ptr<CGameObject>>>	LAYERS;

	_uint					m_iNumLevels = 0;

private:
	list<RESERVED_OBEJECT_DESC>								m_ReservedObjects;

public:
	FDelegate<> CallBack_Start;
	
public:
	weak_ptr<CGameObject> Add_GameObject(size_t iTypeHash, _uint iLevelIndex, /*CTransform * pParent = nullptr,*/ void* pArg = nullptr, _bool _bMemoryPool = false);

public: /* For Template Function */
	template <typename T>
	weak_ptr<T> Add_Prototype()
	{
		static_assert(is_base_of<CGameObject, T>::value, "T Isn't base of CGameObject");

		auto iter = m_Prototypes.find(typeid(T).hash_code());

		if (m_Prototypes.end() == iter)
		{
			shared_ptr<T> pInstance = T::Create();

			m_Prototypes.emplace(typeid(T).hash_code(), pInstance);

			return pInstance;
		}

		return Weak_Cast<T>((*iter).second);
	}

	template <typename T>
	weak_ptr<T> Add_GameObject(_uint iLevelIndex, /*CTransform* pParent = nullptr,*/ void* pArg = nullptr, _bool _bMemoryPool = false)
	{
		static_assert(is_base_of<CGameObject, T>::value, "T Isn't base of CGameObject");

		if (m_iNumLevels <= iLevelIndex)
		{
			//�߸��� ���� �ε���
#ifdef _DEBUG
			assert(false);
#endif
			return weak_ptr<T>();
		}

		weak_ptr<CGameObject> pPrototype;

		_hashcode HashCode = typeid(T).hash_code();

		auto iter = m_Prototypes.find(HashCode);
		//auto iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder_c_str(typeid(T).name()));

		if (iter != m_Prototypes.end())
		{
			pPrototype = (*iter).second;
		}

		if (0 == pPrototype.lock())
		{
			pPrototype = Add_Prototype<T>();
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
			return weak_ptr<T>();

		shared_ptr<CGameObject> pCloneObject = pPrototype.lock()->Clone(iLevelIndex, pArg);
		//pCloneObject->Set_OwnerForMyComponents();

		m_ReservedObjects.push_back({ HashCode, iLevelIndex, pCloneObject });

		//m_pLayers[iLevelIndex][HashCode].push_back(pCloneObject);

		/*if (pParent)
		{
			CTransform* pTransfromCom = pCloneObject->Get_Component<CTransform>();
			pTransfromCom->Set_Parent(pParent);
			pParent->Add_Child(pTransfromCom);
		}*/
		//pCloneObject->OnEnable(pArg);

		return dynamic_pointer_cast<T>(pCloneObject);
	}

	template<typename T>
	list<weak_ptr<T>> Get_GameObjects(_uint iLevelIndex)
	{
		static_assert(is_base_of<CGameObject, T>::value, "T Isn't base of CGameObject");

		if (m_iNumLevels <= iLevelIndex)
		{
			//�߸��� ���� �ε���
#ifdef _DEBUG
			assert(false);
#endif
			return list<weak_ptr<T>>();
		}

		auto iter = m_pLayers[iLevelIndex].find(typeid(T).hash_code());

		if (m_pLayers[iLevelIndex].end() == iter)
		{
			return list<weak_ptr<T>>();
		}

		list<weak_ptr<T>> GameObjects;

		for (auto& elem : iter->second)
		{
			GameObjects.push_back(Shared_Cast<T>(elem));
		}

		return GameObjects;

	}

	template<typename T>
	weak_ptr<T> Get_GameObject_UseMemoryPool(_uint iLevelIndex)
	{
		static_assert(is_base_of<CGameObject, T>::value, "T Isn't base of CGameObject");

		if (m_iNumLevels <= iLevelIndex)
		{
			//�߸��� ���� �ε���
#ifdef _DEBUG
			assert(false);
#endif
			return weak_ptr<T>();
		}

		auto iter = m_pLayers[iLevelIndex].find(typeid(T).hash_code());

		// �����Ȱ� ������ �����ؼ� ��������.
		if (m_pLayers[iLevelIndex].end() == iter)
		{
			return Add_GameObject<T>(iLevelIndex);
		}

		for (auto& elem : iter->second)
		{
			if (!elem->Get_Enable())
			{
				return Shared_Cast<T>(elem);
			}
		}

		// ���� Ȱ��ȭ ���¶�� �����ؼ� ��������.

		return Add_GameObject<T>(iLevelIndex);
	}

//	template <typename T>
//	CGameObject* Get_ParticleSystem(_uint iLevelIndex, const _tchar* pLayerTag, CTransform* pParent = nullptr, void* pArg = nullptr)
//	{
//		static_assert(is_base_of<CParticleSystem, T>::value, "T Isn't base of CParticleSystem");
//
//		if (m_iNumLevels <= iLevelIndex)
//		{
//			//�߸��� ���� �ε���
//#ifdef _DEBUG
//			assert(false);
//#endif
//			return nullptr;
//		}
//
//		CGameObject* pPrototype = nullptr;
//
//		auto iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder_c_str(typeid(T).name()));
//
//		if (iter != m_Prototypes.end())
//		{
//			pPrototype = (*iter).second;
//		}
//
//		if (nullptr == pPrototype)
//		{
//			pPrototype = Add_Prototype<T>();
//		}
//
//		for (auto& elem : m_pLayers[iLevelIndex][pLayerTag])
//		{
//			elem->Set_Enable(true, pArg);
//
//			return elem;
//		}
//
//
//		CGameObject* pCloneObject = pPrototype->Clone(pArg);
//
//		pCloneObject->Set_Internal_Tag(pLayerTag);
//
//		if (nullptr == pCloneObject)
//			return nullptr;
//
//		m_pLayers[iLevelIndex][pLayerTag].push_back(pCloneObject);
//
//		if (pParent)
//		{
//			CTransform* pTransfromCom = pCloneObject->Get_Component<CTransform>();
//			pTransfromCom->Set_Parent(pParent);
//			pParent->Add_Child(pTransfromCom);
//		}
//		pCloneObject->OnEnable();
//
//		return pCloneObject;
//	}

};

END