#pragma once

#include "Engine_Defines.h"
#include "Json/Json_Utility.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase() = default;
	virtual ~CBase() = default;


public:
	//해당 객체의 활성화 상태를 바꿉니다. 활성화 상태가 변한다면 OnEnable / OnDisable 이벤트 함수를 호출합니다.
	_bool Set_Enable(_bool _Enable, void* _Arg = nullptr);
	bool Get_Enable();

	void Set_ThisInstance(weak_ptr<CBase> Other);

	virtual void Write_Json(json& Out_Json) {};
	virtual void Load_FromJson(const json& In_Json) {};

public: /* For Event Function */
	//타이머 이벤트 호출
	virtual void OnTimerEvent(const _uint _iEventIndex) {}

	//레벨 변경 시 호출
	virtual void OnLevelEnter() {}
	virtual void OnLevelExit() {}

	//객체의 상태가 활성화 상태로 변경될 때, 호출되는 이벤트입니다.
	virtual void OnEnable(void* _Arg = nullptr) {}

	//객체의 상태가 비활성화 상태로 변경될 때, 호출되는 이벤트입니다.
	virtual void OnDisable() {}

	//객체가 해제되기 전에 호출됩니다. Free보다 이전 시점에 호출됩니다.
	//Free는 스마트 포인터가 해제된 이후에 호출됩니다.
	virtual void OnDestroy() {}

	//이벤트 메세지입니다.
	virtual void OnEventMessage(_uint iArg) {}
	virtual void OnEventMessage(void* pArg) {}

private:
	virtual void OnEngineEventMessage(const ENGINE_EVENT_TYPE In_eEngineEvent) {};

protected:
	weak_ptr<CBase> m_this;

private:
	_bool			m_bEnable = true;

protected:
	void Free();

protected:
	template<typename T>
	static weak_ptr<T> Weak_Cast(weak_ptr<CBase> Instance)
	{
		return dynamic_pointer_cast<T>(Instance.lock());
	}

	template<typename T>
	static shared_ptr<T> Shared_Cast(shared_ptr<CBase> Instance)
	{
		return dynamic_pointer_cast<T>(Instance);
	}

	template<typename T>
	static weak_ptr<T> Weak_StaticCast(weak_ptr<CBase> Instance)
	{
#ifdef _DEBUG
		return dynamic_pointer_cast<T>(Instance.lock());
#else // _DEBUG
		return static_pointer_cast<T>(Instance.lock());
#endif // _DEBUG
	}

	template<typename T>
	static shared_ptr<T> Shared_StaticCast(shared_ptr<CBase> Instance)
	{
		return static_pointer_cast<T>(Instance);
	}


	template<typename T>
	static weak_ptr<T> Cast(weak_ptr<CBase> Instance)
	{
		return dynamic_pointer_cast<T>(Instance.lock());
	}

	template<typename T>
	static shared_ptr<T> Cast(shared_ptr<CBase> Instance)
	{
		return dynamic_pointer_cast<T>(Instance);
	}

	
};

END

