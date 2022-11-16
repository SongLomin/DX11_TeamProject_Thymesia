#pragma once

/* 객체들의 원형을 관리하낟. */
/* 원형객체를 찾아서 복제하여 씬(레벨)별로 모아서 관린하다.*/

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
	HRESULT Reserve_Container(_uint iNumLevels);
	
	virtual void OnLevelEnter() override;
	virtual void OnLevelExit() override;

public:
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void Before_Render(_float fTimeDelta);
	void After_Render();

	virtual void OnDestroy() override;

public:
	void Clear(_uint iLevelIndex);
	void Remove_DeadObject();

private:
	void Register_ReservedObjects();

protected:
	void Free();

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
	weak_ptr<CGameObject> Add_GameObject(size_t iTypeHash, _uint iLevelIndex, void* pArg = nullptr);

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
	weak_ptr<T> Add_GameObject(_uint iLevelIndex, /*CTransform* pParent = nullptr,*/ void* pArg = nullptr)
	{
		static_assert(is_base_of<CGameObject, T>::value, "T Isn't base of CGameObject");

		if (m_iNumLevels <= iLevelIndex)
		{
			//잘못된 레벨 인덱스
#ifdef _DEBUG
			assert(false);
#endif
			return weak_ptr<T>();
		}

		weak_ptr<CGameObject> pPrototype;

		_hashcode HashCode = typeid(T).hash_code();

		auto iter = m_Prototypes.find(HashCode);

		if (iter != m_Prototypes.end())
		{
			pPrototype = (*iter).second;
		}

		if (0 == pPrototype.lock())
		{
			pPrototype = Add_Prototype<T>();
		}

		if (0 == pPrototype.use_count())
			return weak_ptr<T>();

		shared_ptr<CGameObject> pCloneObject = pPrototype.lock()->Clone(iLevelIndex, pArg);
		//pCloneObject->Set_OwnerForMyComponents();

		m_ReservedObjects.push_back({ HashCode, iLevelIndex, pCloneObject });

		return dynamic_pointer_cast<T>(pCloneObject);
	}

	template<typename T>
	list<weak_ptr<T>> Get_GameObjects(_uint iLevelIndex)
	{
		static_assert(is_base_of<CGameObject, T>::value, "T Isn't base of CGameObject");

		if (m_iNumLevels <= iLevelIndex)
		{
			//잘못된 레벨 인덱스
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
			//잘못된 레벨 인덱스
#ifdef _DEBUG
			assert(false);
#endif
			return weak_ptr<T>();
		}

		auto iter = m_pLayers[iLevelIndex].find(typeid(T).hash_code());

		// 생성된게 없으면 생성해서 내보낸다.
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

		// 전부 활성화 상태라면 생성해서 내보낸다.

		return Add_GameObject<T>(iLevelIndex);
	}

};

END