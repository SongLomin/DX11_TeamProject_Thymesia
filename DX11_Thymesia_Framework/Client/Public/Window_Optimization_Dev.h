#pragma once
#include "ImGui_Window.h"

BEGIN(Client)
class CWindow_Optimization_Dev :
	public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_Optimization_Dev)

public:
	_float Get_InstancingCullingRatio() const { return m_fInstancingCullingRatio; }

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
	_float m_fInstancingCullingRatio = 0.5f;

private:
	void Free();

};

END