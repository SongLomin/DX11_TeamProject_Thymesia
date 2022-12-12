#pragma once
#include "ImGui_Window.h"

BEGIN(Client)
class CWindow_Level_Dev :
	public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_Level_Dev)

private:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private: /* For. LEVEL */
	

private:
	void Free();

};

END