#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Client)

class CImGui_Window abstract : public CBase
{

	friend class CImGui_Manager;

public:
	struct GAMEOBJECT_DESC
	{
		_hashcode				HashCode;
		string					TypeName;
		weak_ptr<CGameObject>	pInstance;
	};

public:
	struct ImGuiDESC
	{
		string				strName;
		ImGuiWindowFlags	eWindowFlags = ImGuiWindowFlags_None;
		ImVec2				vWindowSize = { 0.f, 0.f };

	};

public:
	CImGui_Window();
	virtual ~CImGui_Window();

public:
	_bool			Is_Enable() { return m_bEnable; }

public:
	virtual HRESULT	Initialize() PURE;
	virtual void	Start() PURE;
	virtual void	Tick(_float fTimeDelta);
	virtual void	Background_Tick(_float fTimeDelta) {}
	virtual HRESULT	Render(ID3D11DeviceContext* pDeviceContext) PURE;

public:
	virtual void OnLevelLoad() {};
	virtual void OnEventMessage(_uint iArg) {}

protected:
	_bool			m_bEnable = false;
	ImGuiDESC		m_tImGuiDESC;
	ImVec4			m_BackgroundColor = ImVec4(1.f, 1.f, 1.f, 1.f);
	_float			m_fTimeDelta = 0.f;
	_float			m_fWindowAhlpa = 1.f;

public:
	HRESULT			SetUp_ImGuiDESC(string _strName, const ImVec2& _vWindowSize, ImGuiWindowFlags _eWindowFlags);
	HRESULT			Begin();
	HRESULT			End();


};

END

