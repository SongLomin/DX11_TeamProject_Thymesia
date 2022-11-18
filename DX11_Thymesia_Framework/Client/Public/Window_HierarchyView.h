#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CWindow_HierarchyView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_HierarchyView)

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Call_Add_GameObject(const _hashcode& TypeHash, const char* TypeName);
	void Write_Json(json& Out_Json);
	void Load_FromJson(const json& In_Json);

private:
	void Call_Add_GameObject_Internal(const _hashcode& TypeHash, const char* TypeName);

public:
	FDelegate<GAMEOBJECT_DESC> CallBack_ListClick;

	list<GAMEOBJECT_DESC>	m_pGameObjects;
	list<GAMEOBJECT_DESC>	m_pSubGameObjects;

private:
	_uint	m_iPreSelectIndex = 0;

public:
	virtual void OnLevelLoad() override;

public:
	void Free();

	
};
END
