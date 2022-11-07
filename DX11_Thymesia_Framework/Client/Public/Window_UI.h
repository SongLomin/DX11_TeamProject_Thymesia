#pragma once
#include "ImGui_Window.h"


BEGIN(Client)
class CWindow_UI final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_UI)


public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Free() override;
};
END
