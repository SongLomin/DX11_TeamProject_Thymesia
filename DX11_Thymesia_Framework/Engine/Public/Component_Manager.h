#pragma once



/* 컴포넌트들의 원형을 가지고 있는다. */
//#include "Shader.h"
//#include "Texture.h"
#include "Renderer.h"
//#include "Transform.h"
//#include "VIBuffer_Rect.h"
//#include "VIBuffer_Cube.h"
//#include "VIBuffer_Terrain.h"


BEGIN(Engine)

class CComponent;

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

public:
	//HRESULT Reserve_Container(_uint iNumLevels);
	//HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	//class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);
public:
	//void Clear(_uint iLevelIndex);
	void Receive_EngineEventMessage(weak_ptr<CGameObject> pGameObject, const ENGINE_EVENT_TYPE In_eEngineEvent);

private:

	unordered_map<_hashcode, shared_ptr<CComponent>>				m_pPrototypes;
	typedef unordered_map<_hashcode, shared_ptr<CComponent>>		PROTOTYPES;

private:
	//class CComponent* Find_Components(const _tchar* pPrototypeTag);

public:
	void Free();

public: /* For Template Function*/
	template <typename T>
	shared_ptr<T> Clone_Component(void* pArg, weak_ptr<CGameObject> pOwner)
	{
		static_assert(is_base_of<CComponent, T>::value, "T isn't base of CComponent : CComponent_Manager");

		_hashcode Template_Class_HashCode = typeid(T).hash_code();

		weak_ptr<CComponent> pPrototype;
		/*Find_Components*/
		auto	iter = m_pPrototypes.find(Template_Class_HashCode);

		if (iter != m_pPrototypes.end())
		{
			pPrototype = iter->second;
		}

		if (0 == pPrototype.use_count())
		{
			/*Add_Prototype*/
			shared_ptr<CComponent> T_Instance = T::Create();

			m_pPrototypes.emplace(Template_Class_HashCode, T_Instance);


			T_Instance->Set_Owner(pOwner);
			T_Instance->m_CreatedLevel = pOwner.lock()->Get_CreatedLevel();

			return static_pointer_cast<T>(T_Instance->Clone(0, pArg));
		}

		pPrototype.lock()->Set_Owner(pOwner);
		pPrototype.lock()->m_CreatedLevel = pOwner.lock()->Get_CreatedLevel();

		return static_pointer_cast<T>(pPrototype.lock()->Clone(0, pArg));
	}
};

END