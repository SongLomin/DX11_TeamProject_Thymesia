#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
END

BEGIN(Client)
class CWindow_ComponentView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_ComponentView)

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
	void Call_Click_GameObject(GAMEOBJECT_DESC GameObjectDesc);

private:
	void Draw_Components();
	void Init_Components();

private:
	void TransformComponent_PickingAction(weak_ptr<Engine::CTransform> _pTransform);
	void View_FreeCamera(weak_ptr<Engine::CTransform> In_pTransform);

private:
	GAMEOBJECT_DESC m_tPickedGameObjectDesc;
	_char m_szModelKey[MAX_PATH];

private: /* For. ModelComponent */
	vector<string>	m_AllModelKeys;
	_int			m_CurrentModelIndex = 0;

public:
	void Free();
};
END
