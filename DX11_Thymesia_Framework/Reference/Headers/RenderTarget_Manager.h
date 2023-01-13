#pragma once
#include "Base.h"
BEGIN(Engine)

class CRenderTarget;
class CShader;
class CVIBuffer_Rect;

class CRenderTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CRenderTarget_Manager)

public:
	ComPtr<ID3D11ShaderResourceView> Get_SRV(const _tchar* pTargetTag);
	list<const _tchar*> Get_AllSRVNames();

public:
	HRESULT Add_RenderTarget(const _tchar* pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, const _bool In_bUnorderedAccessView = false);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	HRESULT Begin_MRT(const _tchar* pMRTTag);
	HRESULT Begin_DeferredMRT(const _tchar* pMRTTag, ID3D11DeviceContext* pDeviceContext);

	HRESULT Begin_SingleRT(const _tchar* pMRTTag, const _uint& In_iIndex);
	HRESULT Begin_MRTWithNoneClear(const _tchar* pMRTTag);

	HRESULT Begin_MRTWithNoneClearWithIndex(const _tchar* pMRTTag, const _uint In_iIndex);
	HRESULT Begin_MRTWithNoneClearWithFlagIndex(const _tchar* pMRTTag, const _flag In_IndexFlag);
	HRESULT Begin_MRTWithDeferredContextNoneClearIndex(const _tchar* pMRTTag, const _uint In_iIndex, ID3D11DeviceContext* pDeviceContext);
	// 해당 인덱스 SRT만 클리어합니다.
	HRESULT Begin_MRTWithClearWithIndex(const _tchar* pMRTTag, const _uint In_iIndex);
	HRESULT Begin_MRTWithOriginalRenderTarget(const _tchar* pMRTTag);
	HRESULT End_MRT();
	HRESULT End_DeferredMRT(ID3D11DeviceContext* pDeviceContext);

	HRESULT Begin_ShadowMRT(const _tchar* pMRTTag);
	HRESULT End_ShadowMRT();

	HRESULT Begin_StaticShadowMRT(const _tchar* pMRTTag);
	HRESULT End_StaticShadowMRT();

	HRESULT Begin_AntiAliasingMRT(const _tchar* pMRTTag);
	HRESULT End_AntiAliasingMRT();

	HRESULT Begin_ExtractTextureMRT(const _tchar* pMRTTag);
	HRESULT End_ExtractTextureMRT();

	HRESULT Bake_ShadowDepthStencilView(_uint iWidth, _uint iHeight);
	HRESULT Bake_StaticShadowDepthStencilView(_uint iWidth, _uint iHeight);
	HRESULT Bake_AntiAliasingDepthStencilView(_uint iWidth, _uint iHeight);
	HRESULT Bake_ExtractTextureDepthStencilView(_uint iWidth, _uint iHeight);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _tchar* pMRTTag, weak_ptr<CShader> pShader, weak_ptr<CVIBuffer_Rect> pVIBuffer);
	HRESULT Render_DebugSRT(const _tchar* pTargetTag, weak_ptr<CShader> pShader, weak_ptr<CVIBuffer_Rect> pVIBuffer);
#endif // _DEBUG

private:
	/* 렌더타겟들을 모아놓는다. */
	unordered_map<tstring, shared_ptr<CRenderTarget>>			m_RenderTargets;

	/* 동시에 바인딩되어야하는 렌더타겟들을 그룹지어놓는다. */
	unordered_map<tstring, list<shared_ptr<CRenderTarget>>>				m_MRTs;

private:
	ComPtr<ID3D11RenderTargetView> m_pBackBufferView = nullptr;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;

	ComPtr<ID3D11RenderTargetView> m_pDeferredContextBackBufferView = nullptr;
	ComPtr<ID3D11DepthStencilView> m_pDeferredContextDepthStencilView = nullptr;

	ComPtr<ID3D11DepthStencilView> m_pShadowDepthStencilView = nullptr;
	ComPtr<ID3D11DepthStencilView> m_pStaticShadowDepthStencilView = nullptr;

	ComPtr<ID3D11DepthStencilView> m_pAntiAliasingDepthStencilView = nullptr;

	ComPtr<ID3D11DepthStencilView> m_pExtractTextureDepthStencilView = nullptr;
	D3D11_VIEWPORT					m_tExtractTextureViewPortDesc;


	D3D11_VIEWPORT					m_tOriginalViewPortDesc;
	D3D11_VIEWPORT					m_tShadowViewPortDesc;
	D3D11_VIEWPORT					m_tAntiAliasingViewPortDesc;


public:
	shared_ptr<CRenderTarget> Find_RenderTarget(const _tchar* pTargetTag);
	list<shared_ptr<CRenderTarget>>* Find_MRT(const _tchar* pMRTTag);

public:
	void Free();
};

END