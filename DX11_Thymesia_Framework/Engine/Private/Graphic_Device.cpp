#include "..\public\Graphic_Device.h"

//#include <evr.h>

IMPLEMENT_SINGLETON(CGraphic_Device);

HRESULT CGraphic_Device::Ready_Graphic_Device(HWND hWnd, GRAPHICDESC::WINMODE WinMode, _uint iWinCX, _uint iWinCY)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL			FeatureLV;

	/* 그래픽 장치를 초기화한다. */
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iFlag, featureLevels, 1, D3D11_SDK_VERSION, &m_pDevice, &FeatureLV, &m_pDeviceContext)))
		return E_FAIL;

	assert(D3D_FEATURE_LEVEL_11_0 == FeatureLV);

	if (FAILED(Ready_SwapChain(hWnd, WinMode, iWinCX, iWinCY)))
		return E_FAIL;

	if (FAILED(Ready_BackBufferRenderTargetView()))
		return E_FAIL;

	if (FAILED(Ready_DepthStencilRenderTargetView(iWinCX, iWinCY)))
		return E_FAIL;

	/* 장치에 바인드해놓을 렌더타겟들과 뎁스스텐실뷰를 셋팅한다. */
	m_pDeviceContext->OMSetRenderTargets(1, m_pBackBufferRTV.GetAddressOf(), m_pDepthStencilView.Get());
	
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (FLOAT)iWinCX;
	ViewPortDesc.Height = (FLOAT)iWinCY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);

	GFSDK_SSAO_CustomHeap CustomHeap;
	CustomHeap.new_ = ::operator new;
	CustomHeap.delete_ = ::operator delete;

	GFSDK_SSAO_Status status;
	status = GFSDK_SSAO_CreateContext_D3D11(m_pDevice, &m_pAOContext, &CustomHeap);
	assert(status == GFSDK_SSAO_OK); // HBAO+ requires feature level 11_0 or above

	/*uint32_t resetToken = 0;
	ComPtr<IMFDXGIDeviceManager> manager;*/

	//m_hVideo = MCIWndCreate(hWnd, nullptr, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, L"../Bin/Video/Wildlife.wmv");

	// 동영상을 재생할 크기를 설정하여 윈도우 창으로 적용, 만약 설정하지 않을 경우 동영상의 원래 크기대로 출력
	//MoveWindow(m_hVideo, 0, 0, 1600, 900, FALSE);



	// 동영상 재생
	//MCIWndPlayFrom(m_hVideo, 3);
	//MCIWndPlay(m_hVideo);

	//if (FAILED(MFCreateDXGIDeviceManager(&resetToken, &manager)))
	//{
	//	DEBUG_ASSERT;
	//}

	//if (FAILED(manager->ResetDevice(m_pDevice, resetToken)))
	//{
	//	DEBUG_ASSERT;
	//}



	//// Create a media source for the video file
	//IMFSourceReader* pSourceReader = nullptr;
	//HRESULT hr = MFCreateSourceReaderFromURL(L"C:\\path\\to\\video.mp4", nullptr, &pSourceReader);
	//if (FAILED(hr)) {
	//	return hr;
	//}

	//IMFMediaType* pMediaType;
	//hr = MFCreateMediaType(&pMediaType);
	//
	//pMediaType->SetUnknown(MF_SOURCE_READER_D3D_MANAGER, manager.Get());
	//pMediaType->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, 1);
	//pMediaType->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, 1);



	//// Configure the media source to output video frames in a DXGI-compatible format
	//hr = pSourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, pMediaType);
	//if (FAILED(hr)) {
	//	return hr;
	//}

	//// Create a media sink for rendering the video frames
	//IMFVideoDisplayControl* pVideoDisplayControl = nullptr;
	//hr = MFCreateVideoRenderer(IID_PPV_ARGS(&pVideoDisplayControl));
	//if (FAILED(hr)) {
	//	return hr;
	//}

	//// Set the media sink's device handle
	//hr = pVideoDisplayControl->SetVideoWindow(hWnd);
	//if (FAILED(hr)) {
	//	return hr;
	//}
	///*hr = pVideoDisplayControl->Set(nullptr);
	//if (FAILED(hr)) {
	//	return hr;
	//}*/

	//RECT rect;
	//rect.left = 0;
	//rect.right = 1600;
	//rect.top = 0;
	//rect.bottom = 900;

	//hr = pVideoDisplayControl->SetVideoPosition(nullptr, &rect);
	//if (FAILED(hr)) {
	//	return hr;
	//}

	//// Start the media pipeline
	//hr = pSourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr, nullptr, nullptr, nullptr);
	//if (FAILED(hr)) {
	//	return hr;
	//}

	//BOOL bEOS = FALSE;
	//while (!bEOS) {
	//	DWORD streamIndex, flags;
	//	LONGLONG llTimestamp;
	//	IMFSample* pSample = nullptr;
	//	hr = pSourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &flags, &llTimestamp, &pSample);
	//	if (FAILED(hr)) {
	//		break;
	//	}

	//	if (flags & MF_SOURCE_READERF_ENDOFSTREAM) {
	//		bEOS = TRUE;
	//	}
	//	else {

	//		//pSample->ConvertToContiguousBuffer();
	//		// Process the video frame
	//		//hr = pVideoDisplayControl->ProcessFrame(pSample);
	//		if (FAILED(hr)) {
	//			break;
	//		}
	//	}
	//}

	/*ComPtr<IMFAttributes> mVideoAttributes;

	mVideoAttributes->SetUnknown(MF_SOURCE_READER_D3D_MANAGER, manager.Get());
	mVideoAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, 1);
	mVideoAttributes->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, 1);

	MFCreateSourceReaderFromURL(url.data(), mVideoAttributes.Get(), &mSourceReader);*/



	return S_OK;
}

HRESULT CGraphic_Device::Clear_BackBuffer_View(_float4 vClearColor)
{
	m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV.Get(), (_float*)&vClearColor);
	
 	return S_OK;
}

HRESULT CGraphic_Device::Clear_DepthStencil_View()
{

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	//m_pDeviceContext->CopyResource(, );
	return S_OK;
}

void CGraphic_Device::SyncronizeDeferredContext(ID3D11DeviceContext* pDeferredDeviceContext)
{
	pDeferredDeviceContext->ClearState();

	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = nullptr;
	m_pDeviceContext->OMGetRenderTargets(1, &rtv, &dsv);
	pDeferredDeviceContext->OMSetRenderTargets(1, &rtv, dsv);

	UINT numViewports = 1;
	D3D11_VIEWPORT viewport;
	m_pDeviceContext->RSGetViewports(&numViewports, &viewport);
	pDeferredDeviceContext->RSSetViewports(1, &viewport);

	if(rtv)
		rtv->Release();
	if(dsv)
		dsv->Release();
}

HRESULT CGraphic_Device::Present()
{
	if (nullptr == m_pSwapChain.Get())
		return E_FAIL;

	return m_pSwapChain->Present(0, 0);	
}

ID3D11DeviceContext* CGraphic_Device::Get_DeviceContext()
{
	return m_pDeviceContext;
}


HRESULT CGraphic_Device::Ready_SwapChain(HWND hWnd, GRAPHICDESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY)
{
	ComPtr<IDXGIDevice>			pDevice;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDevice.GetAddressOf());

	ComPtr<IDXGIAdapter>		pAdapter;
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());

	ComPtr<IDXGIFactory>		pFactory;
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	DXGI_SWAP_CHAIN_DESC		SwapChain;
	ZeroMemory(&SwapChain, sizeof(DXGI_SWAP_CHAIN_DESC));
			
	SwapChain.BufferDesc.Width = iWinCX;
	SwapChain.BufferDesc.Height = iWinCY;
	SwapChain.BufferDesc.RefreshRate.Numerator = 60;
	SwapChain.BufferDesc.RefreshRate.Denominator = 1;
	SwapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapChain.SampleDesc.Quality = 0;
	SwapChain.SampleDesc.Count = 1;
	SwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	SwapChain.BufferCount = 1;
	SwapChain.OutputWindow = hWnd;	
	SwapChain.Windowed = eWinMode;
	SwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &SwapChain, m_pSwapChain.GetAddressOf())))
		return E_FAIL;

	//pFactory->Release();
	//pAdapter->Release();
	//pDevice->Release();

	return S_OK;
}


HRESULT CGraphic_Device::Ready_BackBufferRenderTargetView()
{


	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_pBackBufferTexture.GetAddressOf())))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pBackBufferTexture.Get(), nullptr, m_pBackBufferRTV.GetAddressOf())))
		return E_FAIL;	

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pBackBufferTexture.Get(), nullptr, m_pBackBufferSRV.GetAddressOf())))
		return E_FAIL;

	return S_OK;
}

HRESULT CGraphic_Device::Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY)
{

	
	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWinCX;
	TextureDesc.Height = iWinCY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	//TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, m_pDepthStencilTexture.GetAddressOf())))
		DEBUG_ASSERT;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture.Get(), &depthStencilViewDesc, m_pDepthStencilView.GetAddressOf())))
		DEBUG_ASSERT;


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pDepthStencilTexture.Get(), &shaderResourceViewDesc, m_pDepthStencilSRV.GetAddressOf())))
		DEBUG_ASSERT;

	return S_OK;
}

void CGraphic_Device::Free()
{
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_pDeviceContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

	m_pSwapChain.Reset();
	m_pDepthStencilView.Reset();
	m_pBackBufferRTV.Reset();

	m_pDeviceContext->Flush();
	m_pDeviceContext->Release();
	m_pAOContext->Release();
	m_pDevice->Release();
//
//#if defined(DEBUG) || defined(_DEBUG)
//	ID3D11Debug* d3dDebug;
//	HRESULT hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
//	if (SUCCEEDED(hr))
//	{
//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
//		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
//
//		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//
//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
//		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
//	}
//	if (d3dDebug != nullptr)            d3dDebug->Release();
//#endif


	
}
