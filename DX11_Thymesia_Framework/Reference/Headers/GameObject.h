#pragma once

#include "GameInstance.h"
#include "Base.h"
#include "Transform.h"

BEGIN(Engine)

class CComponent;
class CTransform;
class CGameInstance;
class CShader;
class CPhysXCollider;

class ENGINE_DLL CGameObject abstract : public CBase
{
	GAMECLASS_H(CGameObject)

protected:
	CGameObject(const CGameObject& Prototype);

public:
	_uint Get_GameObjectIndex();

	virtual _float Get_CamDistance() {
		return m_fCamDistance;
	}

	_uint Get_CreatedLevel() const {
		return m_CreatedLevel;
		} 

	shared_ptr<CTransform> Get_Transform() const noexcept {
		return m_pTransformCom.lock();
	}

public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual void Before_Render(_float fTimeDelta);
	virtual void Thread_PreTick(_float fTimeDelta);
	virtual void Thread_PreLateTick(_float fTimeDelta);
	virtual void Thread_PreBeforeRender(_float fTimeDelta);
	virtual void Thread_PreRender(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext); 
	virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext);

	virtual _bool IsPicking(const RAY& In_Ray, _float& Out_fRange) { return false; }

public:
	const unordered_map<_hashcode, list<shared_ptr<CComponent>>>& Get_AllComponents() const;

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

public:
	virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) {};
	virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) {};
	virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) {};

public:
	virtual void OnPhysXCollisionEnter(weak_ptr<CPhysXCollider> pOtherCollider) {};
	virtual void OnPhysXCollisionStay(weak_ptr<CPhysXCollider> pOtherCollider) {};
	virtual void OnPhysXCollisionExit(weak_ptr<CPhysXCollider> pOtherCollider) {};

protected:
	virtual void OnDestroy() override;

	//SetDead가 호출되는 시점에 발생하는 이벤트입니다.
	virtual void OnSetDead() {}

	//메모리풀링이 됐을 때 발생하는 이벤트입니다.
	virtual void OnMemoryPool() {}

protected:
	void Set_OwnerForMyComponents();

protected:
	weak_ptr<CGameObject>		m_thisToGameObject;
	_float						m_fCamDistance = 0.f;
	_uint						m_CreatedLevel;
	_uint						m_ThreadFlag = 0;
	_uint						m_UpatedFromThreadFlag = 0;


protected:
	unordered_map<_hashcode, list<shared_ptr<CComponent>>>	m_pComponents;
	weak_ptr<CTransform>							m_pTransformCom;

public:
	FDelegate<_float>								CallBack_Tick;
	FDelegate<_float>								CallBack_LateTick;
	FDelegate<>										CallBack_Render;
	FDelegate<weak_ptr<CShader>, const char*>		CallBack_Bind_SRV;

	void	Set_Dead();
	bool	Get_Dead() const { return m_bDead; }

	void	Use_Thread(const THREAD_TYPE In_Type);
	void	UnUse_Thread(const THREAD_TYPE In_Type);
	_bool	Is_Thread(const THREAD_TYPE In_Type);
	_bool	LastestDataFromThread(const THREAD_TYPE In_Type);

private:
	static _uint g_iGameObjectIndex;
	_uint m_iGameObjectIndex;
	bool m_bDead = false;

public: /* For Event Function */
	virtual void OnEventMessage(_uint iArg) override;

private:
	virtual void OnEngineEventMessage(const ENGINE_EVENT_TYPE In_eEngineEvent) override;


public:
	virtual shared_ptr<CGameObject> Clone(_uint iLevelIndex, void* pArg) = 0;
	void Free();

public: /* For Template Function */
	template <typename T>
	weak_ptr<T> Add_Component(void* pArg = nullptr)
	{
		
		shared_ptr<T> temp = GAMEINSTANCE->Clone_Component<T>(pArg, Weak_Cast<CGameObject>(m_this));

		if (nullptr == temp.get())
		{
			return shared_ptr<T>();
		}
		
		m_pComponents[typeid(T).hash_code()].push_back(temp);
		temp->Set_Owner(Cast<CGameObject>(m_this));

		//temp->OnEnable(pArg);
		return temp;
	}

	template <typename T>
	weak_ptr<T> Get_Component()
	{
		auto iter = m_pComponents.find(typeid(T).hash_code());

		//맞는 타입키가 없음.
		if (m_pComponents.end() == iter)
		{
			return weak_ptr<T>();
		}

		//맞는 타입키를 찾았는데 리스트가 비었음.
		if (iter->second.size() == 0)
		{
			return weak_ptr<T>();
		}

		return Cast<T>(weak_ptr<CComponent>(iter->second.front()));
	}

	template <typename T>
	list<weak_ptr<T>> Get_Components()
	{
		auto iter = m_pComponents.find(typeid(T).hash_code());

		//맞는 타입키가 없음.
		if (m_pComponents.end() == iter)
		{
			return list<weak_ptr<T>>();
		}

		//맞는 타입키를 찾았는데 리스트가 비었음.
		if (iter->second.size() == 0)
		{
			return list<weak_ptr<T>>();
		}

		list<weak_ptr<T>> pTcoms;

		for (auto& elem : iter->second)
		{
			pTcoms.push_back(Cast<T>(weak_ptr<CComponent>(elem)));
		}

		return pTcoms;
	}

	
	template <typename T>
	weak_ptr<T> Get_ComponentByType()
	{
		shared_ptr<T> TempCom;

		for (auto& elem_List : m_pComponents)
		{
			for (auto& elem : elem_List.second)
			{
				TempCom = Shared_Cast<T>(elem);

				if (TempCom.get())
				{
					return TempCom;
				}
			}
		}

		return weak_ptr<T>();
	}

	template <typename T>
	list<weak_ptr<T>> Get_ComponentsByType()
	{
		list<weak_ptr<T>> pTcoms;

		shared_ptr<T> TempCom;

		for (auto& elem_List : m_pComponents)
		{
			for (auto& elem : elem_List.second)
			{
				TempCom = Shared_Cast<T>(elem);

				if (TempCom.get())
				{
					pTcoms.push_back(TempCom);
				}
			}
		}

		return pTcoms;
	}

	template <typename T>
	void Remove_Components()
	{
		for (auto& elem : m_pComponents[typeid(T).hash_code()])
		{
			elem->OnDestroy();
		}

		m_pComponents[typeid(T).hash_code()].clear();
	}

	void Remove_Component(weak_ptr<CComponent> pComponent);

	friend CObject_Manager;
	friend CThread_Manager;
};

END