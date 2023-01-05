#include "RenderTarget.h"
#include "GameInstance.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

GAMECLASS_C(CRenderTarget)

HRESULT CRenderTarget::Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, const _bool In_bUnorderedAccessView)
{
	m_vClearColor = vClearColor;


	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(DEVICE->CreateTexture2D(&TextureDesc, nullptr, m_pTexture.GetAddressOf())))
		return E_FAIL;

	if (FAILED(DEVICE->CreateRenderTargetView(m_pTexture.Get(), nullptr, &m_pRTV)))
		return E_FAIL;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pTexture.Get(), nullptr, &m_pSRV)))
		return E_FAIL;

	
	if (In_bUnorderedAccessView)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
		uavDesc.Format = eFormat;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		HRESULT hr = DEVICE->CreateUnorderedAccessView(m_pTexture.Get(), &uavDesc, &m_pUAV);

		int i = 0;
	}

    return S_OK;
}

HRESULT CRenderTarget::Clear(ID3D11DeviceContext* pDeviceContext)
{
	if (!pDeviceContext)
	{
		pDeviceContext = DEVICECONTEXT;
	}

	pDeviceContext->ClearRenderTargetView(m_pRTV.Get(), (_float*)&m_vClearColor);

    return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	DEVICECONTEXT->RSGetViewports(&iNumViewports, &ViewPortDesc);

	_matrix		WorldMatrix = XMMatrixIdentity();

	WorldMatrix.r[0] = XMVectorSet(fSizeX, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, fSizeY, 0.f, 0.f);
	WorldMatrix.r[3] = XMVectorSet(fX - (ViewPortDesc.Width * 0.5f), -fY + (ViewPortDesc.Height * 0.5f), 0.f, 1.f);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(WorldMatrix));

	return S_OK;
}

HRESULT CRenderTarget::Render_Debug(weak_ptr<CShader> pShader, weak_ptr<CVIBuffer_Rect> pVIBuffer)
{
	/* 직교투영을 위한 정보를 던진다. */
	if (FAILED(pShader.lock()->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pShader.lock()->Set_ShaderResourceView("g_Texture", m_pSRV)))
		return E_FAIL;

	if (FAILED(pShader.lock()->Begin(0, DEVICECONTEXT)))
		return E_FAIL;

	return pVIBuffer.lock()->Render(DEVICECONTEXT);
}



#endif // _DEBUG




shared_ptr<CRenderTarget> CRenderTarget::Create(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor, const _bool In_bUnorderedAccessView)
{
	shared_ptr<CRenderTarget> pInstance = make_shared<CRenderTarget>();

	if (FAILED(pInstance->Initialize(iWidth, iHeight, eFormat, vClearColor, In_bUnorderedAccessView)))
	{
		DEBUG_ASSERT;
	}

	return pInstance;
}

void CRenderTarget::Free()
{
	m_pTexture.Reset();
	m_pRTV.Reset();
	m_pSRV.Reset();

}
