#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:
	HRESULT Ready_Graphic_Device(HWND hWnd, GRAPHICDESC::WINMODE WinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	void SyncronizeDeferredContext(ID3D11DeviceContext* pDeferredDeviceContext);
	HRESULT Present();

public:
	ID3D11Device* Get_Device() { return m_pDevice; }
	ID3D11DeviceContext* Get_DeviceContext();
	GFSDK_SSAO_Context_D3D11* Get_AOContext() { return m_pAOContext; }

public:
	const ComPtr<ID3D11Texture2D>& Get_Texture() const {
		return m_pBackBufferTexture;
	}

	const ComPtr<ID3D11ShaderResourceView>& Get_SRV() const {
		return m_pBackBufferSRV;
	}

	const ComPtr<ID3D11RenderTargetView>& Get_RTV() const {
		return m_pBackBufferRTV;
	}

	const ComPtr<ID3D11ShaderResourceView>& Get_DepthStencilSRV() const {
		return m_pDepthStencilSRV;
	}


private:	
	std::mutex m_JobMutex;
	/* 메모리 할당. (정점버퍼, 인덱스버퍼, 텍스쳐로드) */
	ID3D11Device*			m_pDevice;

	/* 바인딩작업. 기능실행 (정점버퍼를 SetStreamSource(), SetIndices(), SetTransform(), SetTexture() */
	ID3D11DeviceContext*	m_pDeviceContext;

	ComPtr<IDXGISwapChain>			m_pSwapChain;	

	// ComPtr<ID3D11ShaderResourceView>
	ComPtr<ID3D11Texture2D>				m_pBackBufferTexture;
	ComPtr<ID3D11RenderTargetView>		m_pBackBufferRTV;
	ComPtr<ID3D11ShaderResourceView>	m_pBackBufferSRV;
	ComPtr<ID3D11DepthStencilView>		m_pDepthStencilView;
	ComPtr<ID3D11Texture2D>				m_pDepthStencilTexture;
	ComPtr<ID3D11ShaderResourceView>	m_pDepthStencilSRV;

private:
	GFSDK_SSAO_Context_D3D11*			m_pAOContext;

private:
	HWND		m_hVideo;

private:
	HRESULT Ready_SwapChain(HWND hWnd, GRAPHICDESC::WINMODE WinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

public:
	void Free();
};

END