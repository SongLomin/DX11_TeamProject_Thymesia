#pragma once
#include "ImGui_Window.h"


BEGIN(Client)

class CWindow_TextureEditerView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_TextureEditerView)

public:
	virtual HRESULT Initialize() override;
	virtual void	Start() override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:

private:
	void	Reset_TextureEditer();
	void	Reset_RGBs();
	_bool	Open_Texture();
	_bool	Save_Texture();
	
private:
	void	Render_RGBButton();
	

private:
	string szTextureFileName;
	tstring szTextureFilePath;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;

	_int m_RGBs[2][4];

	string szRGB[4]{ "Red", "Green", "Blue", "Alpha"};

private:
	virtual void OnDestroy() override;
	void Free();
};
END
