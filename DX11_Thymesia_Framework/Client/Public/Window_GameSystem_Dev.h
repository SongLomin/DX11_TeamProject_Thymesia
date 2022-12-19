#pragma once
#include "ImGui_Window.h"

BEGIN(Client)
class CWindow_GameSystem_Dev :
	public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_GameSystem_Dev)

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

public:
	void Apply_CameraShaking();

private: /* For. Camera Shaking */
	_float3		m_vShakingDir{};
	_float		m_fShakingRatio{};
	_float		m_fShakingTime{};
	_float		m_fShakingFrequency{};
	_float		m_fShakingDecreaseRatio{};

private:
	void Free();

};

END