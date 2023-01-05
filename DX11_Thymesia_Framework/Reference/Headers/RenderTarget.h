#pragma once
#include "Base.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;

class CRenderTarget :
    public CBase
{
    GAMECLASS_H(CRenderTarget)

public:
	const ComPtr<ID3D11Texture2D>& Get_Texture() const {
		return m_pTexture;
	}

	const ComPtr<ID3D11ShaderResourceView>& Get_SRV() const {
		return m_pSRV;
	}

	const ComPtr<ID3D11RenderTargetView>& Get_RTV() const {
		return m_pRTV;
	}

	const ComPtr<ID3D11UnorderedAccessView>& Get_UAV() const {
		return m_pUAV;
	}

public:
	/* 1. ID3D11Texture2D 라는 객체를 선 생성. */
	/* 2. ID3D11RenderTargetView를 생성. */
	/* 3. ID3D11ShaderResourceView를 생성. */
	HRESULT Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, const _bool In_bUnorderedAccessView);
	HRESULT Clear(ID3D11DeviceContext* pDeviceContext = nullptr);

#ifdef _DEBUG
	HRESULT Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(weak_ptr<CShader> pShader, weak_ptr<CVIBuffer_Rect> pVIBuffer);
#endif // _DEBUG

private:
	_float4					m_vClearColor;
private:
	_float4x4				m_WorldMatrix;

#ifdef _DEBUG


#endif // _DEBUG

private:
	ComPtr<ID3D11Texture2D> m_pTexture;
	/* 렌더타겟으로 바인딩 할 수 있는 객체 .*/
	ComPtr<ID3D11RenderTargetView> m_pRTV;

	/* 셰이더 전역변수로 바인딩 될 수 있는 객체. */
	ComPtr<ID3D11ShaderResourceView> m_pSRV;

	ComPtr<ID3D11UnorderedAccessView>	m_pUAV;


public:
    static shared_ptr<CRenderTarget> Create(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, const _bool In_bUnorderedAccessView);

private:
    void Free();
};

END