#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
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
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
	void Call_Add_GameObject(const _hashcode& TypeHash, const char* TypeName);
	void Write_Json(json& Out_Json);
	void Write_Json_ObjectListLayer();
	void Load_FromJson(const json& In_Json);

private:
	void Call_Add_GameObject_Internal(const _hashcode& TypeHash, const char* TypeName);
	void Picking_Obj();

public:
	typedef map<_hashcode, vector<GAMEOBJECT_DESC>>		OBJ_GROUP;

	FDelegate<GAMEOBJECT_DESC>		CallBack_ListClick;

	vector<GAMEOBJECT_DESC>			m_pGameObjects;
	vector<GAMEOBJECT_DESC>			m_pSubGameObjects;
	OBJ_GROUP						m_pObjGroup;

	_uint							m_iPreSelectIndex = 0;
	_bool							m_RenderMSG_BOX   = true;

public:
	virtual void OnLevelLoad() override;

public:
	virtual void OnDestroy() override;
	void Free();
};
END
