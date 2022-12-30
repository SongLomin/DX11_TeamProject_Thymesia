#pragma once
#include "ImGui_Window.h"

BEGIN(Client)
class CWindow_Player_Dev :
	public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_Player_Dev)

public:


private:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Background_Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;


private:
	void Set_PlayerPosition(const _float3& In_vPosition);
	_vector Get_PlayerPosition();
	void Set_PlayerGravity(const _bool In_bGravity);

private:
	_float3	m_vPlayerPosition{ 0.f, 0.f, 0.f };


private:
	void Free();

};

END