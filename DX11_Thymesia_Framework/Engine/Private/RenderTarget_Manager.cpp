#include "RenderTarget_Manager.h"
#include "GameInstance.h"
#include "RenderTarget.h"

IMPLEMENT_SINGLETON(CRenderTarget_Manager)

ComPtr<ID3D11ShaderResourceView> CRenderTarget_Manager::Get_SRV(const _tchar* pTargetTag)
{
	shared_ptr<CRenderTarget> pRenderTarget = Find_RenderTarget(pTargetTag);

	if (!pRenderTarget)
		return nullptr;

	return pRenderTarget->Get_SRV();
}

HRESULT CRenderTarget_Manager::Add_RenderTarget(const _tchar* pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr != Find_RenderTarget(pTargetTag))
		return E_FAIL;

	shared_ptr<CRenderTarget> pRenderTarget = CRenderTarget::Create(iWidth, iHeight, eFormat, vClearColor);
	if (!pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(pTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTarget_Manager::Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	shared_ptr<CRenderTarget> pRenderTarget = Find_RenderTarget(pTargetTag);
	if (!pRenderTarget)
		return E_FAIL;

	list<shared_ptr<CRenderTarget>>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<shared_ptr<CRenderTarget>>		MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(pMRTTag, MRTList);
	}
	else
	{
		pMRTList->push_back(pRenderTarget);
	}


	return S_OK;
}

HRESULT CRenderTarget_Manager::Begin_MRT(const _tchar* pMRTTag)
{
	list<shared_ptr<CRenderTarget>>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		DEBUG_ASSERT;

	if (pMRTList->size() >= 8)
		DEBUG_ASSERT;

	ID3D11ShaderResourceView* pSRVs[128] = { nullptr };

	DEVICECONTEXT->PSSetShaderResources(0, 128, pSRVs);

	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV().Get();
	}

	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	DEVICECONTEXT->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	/* 렌더타겟들을 장치에 바인딩한다. */
	DEVICECONTEXT->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView.Get());

	return S_OK;
}

HRESULT CRenderTarget_Manager::Begin_SingleRT(const _tchar* pMRTTag, const _uint& In_iIndex)
{
	list<shared_ptr<CRenderTarget>>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		DEBUG_ASSERT;

	if (pMRTList->size() >= 8)
		DEBUG_ASSERT;

	_uint iIndex = 0;
	shared_ptr<CRenderTarget> pSingleRT;

	for (auto& elem : *pMRTList)
	{
		if (In_iIndex == iIndex)
		{
			pSingleRT = elem;
		}
		
		++iIndex;
	}

	

	ID3D11ShaderResourceView* pSRVs[128] = { nullptr };

	DEVICECONTEXT->PSSetShaderResources(0, 128, pSRVs);

	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	pSingleRT->Clear();
	RTVs[0] = pSingleRT->Get_RTV().Get();
	iNumRTVs++;

	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	DEVICECONTEXT->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	/* 렌더타겟들을 장치에 바인딩한다. */
	DEVICECONTEXT->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView.Get());

	return S_OK;
}

HRESULT CRenderTarget_Manager::Begin_MRTWithNoneClear(const _tchar* pMRTTag)
{
	list<shared_ptr<CRenderTarget>>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		DEBUG_ASSERT;

	if (pMRTList->size() >= 8)
		DEBUG_ASSERT;

	ID3D11ShaderResourceView* pSRVs[128] = { nullptr };

	DEVICECONTEXT->PSSetShaderResources(0, 128, pSRVs);

	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV().Get();
	}

	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	DEVICECONTEXT->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	/* 렌더타겟들을 장치에 바인딩한다. */
	DEVICECONTEXT->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView.Get());

	return S_OK;
}

HRESULT CRenderTarget_Manager::Begin_MRTWithOriginalRenderTarget(const _tchar* pMRTTag)
{
	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	DEVICECONTEXT->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	list<shared_ptr<CRenderTarget>>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		DEBUG_ASSERT;

	if (pMRTList->size() >= 8)
		DEBUG_ASSERT;

	ID3D11ShaderResourceView* pSRVs[128] = { nullptr };

	DEVICECONTEXT->PSSetShaderResources(0, 128, pSRVs);

	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 1;

	RTVs[0] = m_pBackBufferView.Get();

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV().Get();
	}

	

	/* 렌더타겟들을 장치에 바인딩한다. */
	DEVICECONTEXT->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView.Get());

	return S_OK;
}

HRESULT CRenderTarget_Manager::End_MRT()
{

	DEVICECONTEXT->OMSetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.Get());

	m_pBackBufferView.Reset();
	m_pDepthStencilView.Reset();

	return S_OK;
}

HRESULT CRenderTarget_Manager::Begin_ShadowMRT(const _tchar* pMRTTag)
{
	list<shared_ptr<CRenderTarget>>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		DEBUG_ASSERT;

	if (pMRTList->size() >= 8)
		DEBUG_ASSERT;

	ID3D11ShaderResourceView* pSRVs[8] = { nullptr };

	DEVICECONTEXT->PSSetShaderResources(0, 8, pSRVs);

	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV().Get();
	}


	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	DEVICECONTEXT->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	DEVICECONTEXT->ClearDepthStencilView(m_pShadowDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	/* 렌더타겟들을 장치에 바인딩한다. */
	DEVICECONTEXT->OMSetRenderTargets(iNumRTVs, RTVs, m_pShadowDepthStencilView.Get());

	_uint iViewPortCount = 1;

	DEVICECONTEXT->RSGetViewports(&iViewPortCount, &m_tOriginalViewPortDesc);
	DEVICECONTEXT->RSSetViewports(1, &m_tShadowViewPortDesc);
	return S_OK;
}

HRESULT CRenderTarget_Manager::End_ShadowMRT()
{
	DEVICECONTEXT->OMSetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.Get());
	DEVICECONTEXT->RSSetViewports(1, &m_tOriginalViewPortDesc);

	m_pBackBufferView.Reset();
	m_pDepthStencilView.Reset();

	return S_OK;
}

HRESULT CRenderTarget_Manager::Bake_ShadowDepthStencilView(_uint iWidth, _uint iHeight)
{
	ComPtr<ID3D11Texture2D>		pDepthStencilTexture;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(DEVICE->CreateTexture2D(&TextureDesc, nullptr, pDepthStencilTexture.GetAddressOf())))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	if (FAILED(DEVICE->CreateDepthStencilView(pDepthStencilTexture.Get(), nullptr, m_pShadowDepthStencilView.GetAddressOf())))
		return E_FAIL;
	
	ZeroMemory(&m_tShadowViewPortDesc, sizeof(D3D11_VIEWPORT));
	m_tShadowViewPortDesc.TopLeftX = 0;
	m_tShadowViewPortDesc.TopLeftY = 0;
	m_tShadowViewPortDesc.Width = (FLOAT)iWidth;
	m_tShadowViewPortDesc.Height = (FLOAT)iHeight;
	m_tShadowViewPortDesc.MinDepth = 0.f;
	m_tShadowViewPortDesc.MaxDepth = 1.f;

	

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderTarget_Manager::Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	shared_ptr<CRenderTarget> pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		DEBUG_ASSERT;

	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CRenderTarget_Manager::Render_Debug(const _tchar* pMRTTag, weak_ptr<CShader> pShader, weak_ptr<CVIBuffer_Rect> pVIBuffer)
{
	list<shared_ptr<CRenderTarget>>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		DEBUG_ASSERT;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render_Debug(pShader, pVIBuffer);
	}

	return S_OK;
}
#endif // _DEBUG




shared_ptr<CRenderTarget> CRenderTarget_Manager::Find_RenderTarget(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_RenderTargets.begin(), m_RenderTargets.end(), CTag_Finder(pTargetTag));

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second; 
}

list<shared_ptr<CRenderTarget>>* CRenderTarget_Manager::Find_MRT(const _tchar* pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), CTag_Finder(pMRTTag));

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CRenderTarget_Manager::Free()
{
	for (auto& elem : m_RenderTargets)
	{
		elem.second.reset();
	}

	for (auto& elem_List : m_MRTs)
	{
		for (auto& elem : elem_List.second)
		{
			elem.reset();
		}

		elem_List.second.clear();
	}

	m_RenderTargets.clear();
	m_MRTs.clear();


	m_pBackBufferView.Reset();
	m_pDepthStencilView.Reset();
}