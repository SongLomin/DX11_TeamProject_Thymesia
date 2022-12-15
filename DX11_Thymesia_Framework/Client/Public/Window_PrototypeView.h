#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CWindow_PrototypeView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_PrototypeView)

public:
	enum PROTOTYPE
	{
		PROTOTYPE_ACTOR = 0,
		PROTOTYPE_PROP,
		PROTOTYPE_EDIT,
		PROTOTYPE_UI,
		PROTOTYPE_CAMERA,
		PROTOTYPE_NEVIMESH,
		PROTOTYPE_END
	};

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
	FDelegate<_hashcode, const char*> CallBack_ListClick;

private:
	void Add_Prototypes();

private:

	vector<GAMEOBJECT_DESC> m_pPrototypes[(_uint)(PROTOTYPE::PROTOTYPE_END)];
	RAY						m_MouseRay;

public:
	void Free();
};
END
