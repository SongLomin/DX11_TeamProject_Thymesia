#pragma once
#include "ImGui_Window.h"

BEGIN(Client)
class CWindow_RenderTarget_Dev :
	public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_RenderTarget_Dev)

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

	_bool		m_bOldSchool = true;
	_int		m_RenderTargetSize[2]{ 320,180 };
	vector<_int> m_iRenderTargetIndexs;
	vector<string> m_szSRVNames;

private:
	void Free();

};

END