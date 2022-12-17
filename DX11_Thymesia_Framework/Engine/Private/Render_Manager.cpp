#include "Render_Manager.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "Easing_Utillity.h"


IMPLEMENT_SINGLETON(CRender_Manager)

ID3D11DeviceContext* CRender_Manager::Get_BeforeRenderContext()
{
	std::unique_lock<std::mutex> lock(m_job_q_);

	if (m_pBeforeRenderSleepContexts.empty())
	{
		Emplace_SleepContext(1);
	}

	ID3D11DeviceContext* pContext = m_pBeforeRenderSleepContexts.back();
	m_pBeforeRenderSleepContexts.pop_back();

	lock.unlock();

	return pContext;
	// 나중에 이펙트_렉트나 이펙트 메쉬, 맵언맵 등등 시간이 오래 걸리는 곳에 보내준다.
	// 맵언맵이 끝나면 다시 돌려받을 것.
	// 해당 Late_Tick은 쓰레드로 할 것.
}

void CRender_Manager::Release_BeforeRenderContext(ID3D11DeviceContext* pDeviceContext)
{
	std::unique_lock<std::mutex> lock(m_job_q_);

	m_pBeforeRenderContexts.emplace_back(pDeviceContext);

	lock.unlock();
}

void CRender_Manager::Execute_BeforeRenderCommandList()
{
	std::unique_lock<std::mutex> lock(m_job_q_);

	ID3D11CommandList* pCommandList = nullptr;

	for (auto& elem : m_pBeforeRenderContexts)
	{
		elem->FinishCommandList(false, &pCommandList);

		if (pCommandList)
		{
			DEVICECONTEXT->ExecuteCommandList(pCommandList, true);
			pCommandList->Release();
			pCommandList = nullptr; 
		}
	}

	m_pBeforeRenderSleepContexts.insert(m_pBeforeRenderSleepContexts.begin(), m_pBeforeRenderContexts.begin(), m_pBeforeRenderContexts.end());
	m_pBeforeRenderContexts.clear();
	lock.unlock();
}

HRESULT CRender_Manager::Initialize()
{

	/* 렌더타겟들을 추가한다. */

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	DEVICECONTEXT->RSGetViewports(&iNumViewports, &ViewPortDesc);

	for (_int i = 0; i < (_int)DEFERRED_END; ++i)
	{
		DEVICE->CreateDeferredContext(0, m_pDeferredContext[i].GetAddressOf());
		GET_SINGLE(CGraphic_Device)->SyncronizeDeferredContext(m_pDeferredContext[i].Get());
	}
	
	GET_SINGLE(CFont_Manager)->Init_DeviceContext(m_pDeferredContext[DEFERRED_UI]);

	//D3D11_QUERY_DESC queryDesc;
	//queryDesc.MiscFlags;
	//queryDesc.Query = ;
	//ID3D11Query* pQuery = DEVICE->CreateQuery();

	//m_pDeferredContext->Begin();
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);
		
	/*For. Target_AntiAliasing*/
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_AntiAliasing"),
		(_uint)ViewPortDesc.Width*2.f, (_uint)ViewPortDesc.Height*2.f, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 128.f / 255.f, 1.f, 1.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Bake_AntiAliasingDepthStencilView((_uint)ViewPortDesc.Width*2.f, (_uint)ViewPortDesc.Height*2.f)))
		DEBUG_ASSERT;

	/* For.Target_Diffuse */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Diffuse"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	/* For.Target_Normal*/
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Normal"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 1.f, 0.f, 1.f))))
		DEBUG_ASSERT;

	/* For.Target_Depth */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Depth"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f,  0.f, 1.f))))
		DEBUG_ASSERT;

	/* For.Target_Shade */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Shade"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		DEBUG_ASSERT;
	/* For.Target_PBR */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_PBR"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;
	///* For.Target_Emissive*/
	//if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Emissive"),
	//	(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
	//	DEBUG_ASSERT;

	/* For.Target_Specular */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Specular"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	/*For.Target_Fog*/
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Fog"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	/* For.Target_ShaderFlag */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_ShaderFlag"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	/* For.Target_ShadowDepth */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_ShadowDepth"), 
		(_uint)ViewPortDesc.Width * 5, (_uint)ViewPortDesc.Height * 5, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		DEBUG_ASSERT;

	pRenderTargetManager->Bake_ShadowDepthStencilView((_uint)ViewPortDesc.Width * 5, (_uint)ViewPortDesc.Height * 5);

	/* For.Target_StaticShadowDepth */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_StaticShadowDepth"),
		(_uint)ViewPortDesc.Width * 5, (_uint)ViewPortDesc.Height * 5, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		DEBUG_ASSERT;

	pRenderTargetManager->Bake_StaticShadowDepthStencilView((_uint)ViewPortDesc.Width * 5, (_uint)ViewPortDesc.Height * 5);


	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_ExtractBloom"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_ExtractGlow"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_OriginalEffect"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_BlurXForGlow"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_BlurXForBloom"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_BlurForBloom"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_BlurForGlow"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_CopyOriginalRender"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_BlurForEffect"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_ExtractOutLine"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_BlurOutLine"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_BlurOutLineIntensity"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_ViewShadow"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_PublicBlur"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Distortion"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Ambient"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	/*if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_BlurShadow"),
		(_uint)ViewPortDesc.Width * 5, (_uint)ViewPortDesc.Height * 5, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		DEBUG_ASSERT;*/

	//if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Glow"), (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 0.f, 1.f))))
	//	DEBUG_ASSERT;

	/* For.Target_PostEffect */
	/*if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_PostEffect"), (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		DEBUG_ASSERT;*/

	/* For.MRT_Deferred */
	/* 조명연산이 필요한 객체들을 그릴 때, 추후 빛연산에 필요한 정보를 저장받기위한 렌더타겟들 (Diffuse + Normal ) */
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_ShaderFlag"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_PBR"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_ExtractBloom"))))
		DEBUG_ASSERT;


 
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_ExtractEffect"), TEXT("Target_OriginalEffect"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_ExtractEffect"), TEXT("Target_ExtractBloom"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_ExtractEffect"), TEXT("Target_ExtractGlow"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_BlurXForGlow"), TEXT("Target_BlurXForGlow"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_BlurXForBloom"), TEXT("Target_BlurXForBloom"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_BlurForBloom"), TEXT("Target_BlurForBloom"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_BlurForGlow"), TEXT("Target_BlurForGlow"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_BlurEffect"), TEXT("Target_BlurForEffect"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_ExtractOutLine"), TEXT("Target_ExtractOutLine"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_BlurOutLine"), TEXT("Target_BlurOutLine"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_BlurOutLine"), TEXT("Target_BlurOutLineIntensity"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_ViewShadow"), TEXT("Target_ViewShadow"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_PublicBlur"), TEXT("Target_PublicBlur"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Distortion"), TEXT("Target_Distortion"))))
		DEBUG_ASSERT;

	if(FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_AntiAliasing"),TEXT("Target_AntiAliasing"))))
		DEBUG_ASSERT;

	//if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Glow"), TEXT("Target_Glow"))))
	//	DEBUG_ASSERT;


	/* For.MRT_LightAcc */
	/* 조명연산한 결과를 저장해놓기위한 타겟 (Shade + Specular) */
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Ambient"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Fog"), TEXT("Target_Fog"))))
		DEBUG_ASSERT;

	/* For.MRT_ShadowDepth*/
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_ShadowDepth"), TEXT("Target_ShadowDepth"))))
		DEBUG_ASSERT;

	/* For.MRT_StaticShadowDepth*/
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_StaticShadowDepth"), TEXT("Target_StaticShadowDepth"))))
		DEBUG_ASSERT;


	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_CopyOriginalRender"), TEXT("Target_CopyOriginalRender"))))
		DEBUG_ASSERT;


	_float fSize = 150.f;
	_float fHalf = fSize * 0.5f;

#ifdef _DEBUG	
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_Diffuse"), fHalf, fHalf, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_Normal"), fHalf, fHalf + fSize, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_Depth"), fHalf, fHalf + fSize * 2, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_ShaderFlag"), fHalf, fHalf + fSize * 3, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_ExtractOutLine"), fHalf, fHalf + fSize * 4, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_BlurOutLine"), fHalf, fHalf + fSize * 5, fSize, fSize)))
		DEBUG_ASSERT;


	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_Shade"), fHalf + fSize, fHalf, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_Specular"), fHalf + fSize, fHalf + fSize, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_CopyOriginalRender"), fHalf + fSize, fHalf + fSize * 2, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_ViewShadow"), fHalf + fSize, fHalf + fSize * 3, fSize, fSize)))
		DEBUG_ASSERT;


	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_ShadowDepth"), ViewPortDesc.Width - fHalf, fHalf, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_ExtractBloom"), ViewPortDesc.Width - fHalf, fHalf + fSize, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_BlurForBloom"), ViewPortDesc.Width - fHalf, fHalf + fSize * 2.f, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_PBR"), ViewPortDesc.Width - fHalf, fHalf + fSize * 4.f, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_Ambient"), ViewPortDesc.Width - fHalf, fHalf + fSize * 5.f, fSize, fSize)))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_ExtractGlow"), ViewPortDesc.Width - fHalf - fSize, fHalf, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_BlurForGlow"), ViewPortDesc.Width - fHalf - fSize, fHalf + fSize, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_OriginalEffect"), ViewPortDesc.Width - fHalf - fSize, fHalf + fSize * 2.f, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_BlurForEffect"), ViewPortDesc.Width - fHalf - fSize, fHalf + fSize * 3.f, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_AntiAliasing"), ViewPortDesc.Width - fHalf - fSize, fHalf + fSize * 5.f, fSize, fSize)))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_StaticShadowDepth"), ViewPortDesc.Width - fHalf -fSize*2.f, fHalf, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_Distortion"), ViewPortDesc.Width - fHalf - fSize * 2.f, fHalf +fSize, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_Fog"), ViewPortDesc.Width - fHalf - fSize * 3.f, fHalf, fSize, fSize)))
		DEBUG_ASSERT;


	/*if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_BlurShadow"), ViewPortDesc.Width - 300, 700, 200, 200)))
		DEBUG_ASSERT;*/

	
	//if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_Glow"), ViewPortDesc.Width - 300, 700, 200, 200)))
	//	DEBUG_ASSERT;

#endif // _DEBUG

	_matrix			WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height, 0.f, 0.f);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(WorldMatrix));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(ViewPortDesc.Width, ViewPortDesc.Height, 0.f, 1.f)));

	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width*2.f, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height*2.f, 0.f, 0.f);
	XMStoreFloat4x4(&m_AntiAliasingWorldMatrix, XMMatrixTranspose(WorldMatrix));
	XMStoreFloat4x4(&m_AntiAliasingProjMatrixTranspose, XMMatrixTranspose(XMMatrixOrthographicLH(ViewPortDesc.Width*2.f, ViewPortDesc.Height*2.f, 0.f, 1.f)));


	m_pShader = CShader::Create();
	GAMEINSTANCE->Load_Shader(TEXT("Shader_Deferred"), TEXT("../Bin/Shaderfiles/Shader_Deferred.hlsl"));
	m_pShader->Set_ShaderInfo(TEXT("Shader_Deferred"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);

	m_pXBlurShader = CShader::Create();
	GAMEINSTANCE->Load_Shader(TEXT("Shader_XBlur"), TEXT("../Bin/Shaderfiles/Shader_XBlur.hlsl"));
	m_pXBlurShader->Set_ShaderInfo(TEXT("Shader_XBlur"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);

	m_pOutLineShader = CShader::Create();
	GAMEINSTANCE->Load_Shader(TEXT("Shader_OutLine"), TEXT("../Bin/Shaderfiles/Shader_OutLine.hlsl"));
	m_pOutLineShader->Set_ShaderInfo(TEXT("Shader_OutLine"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);
	
	m_pDistortionShader = CShader::Create();
	GAMEINSTANCE->Load_Shader(TEXT("Shader_Distortion"), TEXT("../Bin/Shaderfiles/Shader_DistortionBlend.hlsl"));
	m_pDistortionShader->Set_ShaderInfo(TEXT("Shader_Distortion"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);

	m_pPostProcessingShader = CShader::Create();
	GAMEINSTANCE->Load_Shader(TEXT("Shader_PostProcessing"), TEXT("../Bin/Shaderfiles/Shader_PostProcessing.hlsl"));
	m_pPostProcessingShader->Set_ShaderInfo(TEXT("Shader_PostProcessing"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);


	m_pVIBuffer = CVIBuffer_Rect::Create();
	
	GAMEINSTANCE->Load_Textures("IrradianceMap", TEXT("../Bin/Resources/Textures/IrradianceMap/IrradianceMap%d.dds"));
	m_pIrradianceTextureCom = CTexture::Create();
	m_pIrradianceTextureCom->Use_Texture("IrradianceMap");

	GAMEINSTANCE->Load_Textures("BRDF", TEXT("../Bin/Resources/Textures/BRDF/brdf%d.png"));
	m_pBRDFLUTTextureCom = CTexture::Create();
	m_pBRDFLUTTextureCom->Use_Texture("BRDF");



	return S_OK;
}

HRESULT CRender_Manager::Add_RenderGroup(RENDERGROUP eGroup, weak_ptr<CGameObject> pGameObject)
{
	if ((_uint)eGroup >= (_uint)RENDERGROUP::RENDER_END ||
		pGameObject.lock().get() == nullptr)
		DEBUG_ASSERT;

	m_RenderObjects[(_uint)eGroup].push_back(pGameObject);

	//Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRender_Manager::Draw_RenderGroup()
{
	

	GET_SINGLE(CGraphic_Device)->SyncronizeDeferredContext(m_pDeferredContext[DEFERRED_EFFECT].Get());	
	GET_SINGLE(CGraphic_Device)->SyncronizeDeferredContext(m_pDeferredContext[DEFERRED_UI].Get());


	while (!GET_SINGLE(CThread_Manager)->Check_JobDone())
	{
		cout << "Wait For Object Jobs." << endl;
		continue;
	}

	Execute_BeforeRenderCommandList();

	//std::list<std::future<HRESULT>> futures;
	
	//futures.emplace_back(GET_SINGLE(CThread_Manager)->EnqueueJob(bind(&CRender_Manager::Render_Effect, this)));
	// futures.emplace_back(GET_SINGLE(CThread_Manager)->EnqueueJob(bind(&CRender_Manager::Render_UI, this)));

	//future<HRESULT> DrawUIThread = async(launch::async, bind(&CRender_Manager::Render_UI, this));
	//future<HRESULT> DrawEffectThread = async(launch::async, bind(&CRender_Manager::Render_Effect, this));

	// HRESULT hr;
	ID3D11CommandList* pCommandList = nullptr;

	/*if (FAILED(Render_UI()))
		DEBUG_ASSERT;*/

	if (FAILED(Render_Priority()))
		DEBUG_ASSERT;
	if (FAILED(Render_ShadowDepth()))
		DEBUG_ASSERT;
	if (FAILED(Render_NonAlphaBlend()))
		DEBUG_ASSERT;

	if (FAILED(Extract_OutLine()))
		DEBUG_ASSERT;

	if (FAILED(Blur_OutLine()))
		DEBUG_ASSERT;

	if (FAILED(Bake_ViewShadow()))
		DEBUG_ASSERT;

	if (FAILED(Render_Lights()))
		DEBUG_ASSERT;

	if (FAILED(Bake_Fog()))
		DEBUG_ASSERT;


	///////SSR 넣기

	if (FAILED(Render_Blend()))
		DEBUG_ASSERT;
	//////여기다가 화면 블룸

	if (FAILED(Blend_OutLine()))
		DEBUG_ASSERT;


	if (FAILED(Render_NonLight()))
		DEBUG_ASSERT;
	
	
	Render_Effect();
	
	//hr = DrawEffectThread.get();

	

	/*m_pDeferredContext[DEFERRED_EFFECT]->FinishCommandList(false, &pCommandList);

	if (pCommandList)
	{
		DEVICECONTEXT->ExecuteCommandList(pCommandList, true);
		pCommandList->Release();
		pCommandList = nullptr;
	}*/

	if (SUCCEEDED(Extract_Distortion()))
	{
		if (FAILED(Blend_Distortion()))
			DEBUG_ASSERT;
	}
	
	if (FAILED(Render_Font()))
		DEBUG_ASSERT;

	if (FAILED(Blur_ExtractBloom()))
		DEBUG_ASSERT;

	if (FAILED(ReBlur_ExtractBloom()))
		DEBUG_ASSERT;

	if (FAILED(ReBlur_ExtractBloom()))
		DEBUG_ASSERT;

	if (FAILED(Blend_Bloom()))
		DEBUG_ASSERT;


	if (FAILED(PostProcessing()))
		DEBUG_ASSERT;

	if (FAILED(AntiAliasing()))
		DEBUG_ASSERT;


	
	/*hr = futures.front().get();
	while (!GET_SINGLE(CThread_Manager)->Check_JobDone())
	{
		cout << "Wait For Context Jobs." << endl;
		continue;
	}*/
	
	/*m_pDeferredContext[DEFERRED_UI]->FinishCommandList(false, &pCommandList);

	if (pCommandList)
	{
		DEVICECONTEXT->ExecuteCommandList(pCommandList, true);
		pCommandList->Release();
		pCommandList = nullptr;
	}*/
	
	/*std::future<HRESULT> UIFuture = GET_SINGLE(CThread_Manager)->Enqueue_Job(bind(&CRender_Manager::Render_UI, this));
	UIFuture.get();

	m_pDeferredContext[DEFERRED_UI]->FinishCommandList(false, &pCommandList);

	if (pCommandList)
	{
		DEVICECONTEXT->ExecuteCommandList(pCommandList, true);
		pCommandList->Release();
		pCommandList = nullptr;
	}*/

	if (FAILED(Render_UI()))
		DEBUG_ASSERT;

	if (FAILED(Render_Final()))
		DEBUG_ASSERT;

#ifdef _DEBUG

	if (FAILED(Render_Debug()))
		DEBUG_ASSERT;
#endif 

	return S_OK;
  }

HRESULT CRender_Manager::Set_MotionBlur(const _float In_fBlurScale)
{
	m_fMotionBlurStrength = In_fBlurScale;
	m_fMotionBlurStrengthAcc = In_fBlurScale;

	return S_OK;
}

HRESULT CRender_Manager::Add_MotionBlur(const _float In_fBlurScale)
{
	m_fMotionBlurStrengthAcc += In_fBlurScale;
	m_fMotionBlurStrengthAcc = min(1.f,max(0.f, m_fMotionBlurStrengthAcc));

	return S_OK;
}

HRESULT CRender_Manager::Set_Chromatic(const _float In_fChormaticStrangth)
{
	m_fChromaticStrengthAcc = In_fChormaticStrangth;
	m_fChromaticStrangth = In_fChormaticStrangth;

	return S_OK;
}

HRESULT CRender_Manager::Add_Chromatic(const _float In_fChormaticStrangth)
{
	m_fChromaticStrengthAcc += In_fChormaticStrangth;
	m_fChromaticStrengthAcc = max(0.f, m_fChromaticStrengthAcc);

	return S_OK;
}

HRESULT CRender_Manager::Set_RadialBlur(const _float In_fRadialBlurStength, _float3 In_vBlurWorldPosition)
{
	m_fRadialBlurStrength = In_fRadialBlurStength;
	m_fRadialBlurStrengthAcc = In_fRadialBlurStength;
	m_vRadialBlurWorldPos = In_vBlurWorldPosition;

	return S_OK;
}

HRESULT CRender_Manager::Add_RadialBlur(const _float In_fRadialBlurStrength)
{
	m_fRadialBlurStrengthAcc += In_fRadialBlurStrength;
	m_fRadialBlurStrengthAcc = max(0.f, m_fRadialBlurStrengthAcc);

	return S_OK;
}

HRESULT CRender_Manager::Set_FogDesc(const _float4 In_vFogColor, const _float In_fFogRange)
{
	m_vFogColor = In_vFogColor;
	m_fFogRange = In_fFogRange;

	return S_OK;
}

HRESULT CRender_Manager::Set_Contrast(const _float In_fContrast)
{
	m_fContrastValue = In_fContrast;

	return S_OK;
}
HRESULT CRender_Manager::Set_Saturation(const _float In_fSaturation)
{
	m_fSaturation = In_fSaturation;

	return S_OK;
}


HRESULT CRender_Manager::Set_LiftGammaGain(const _float4 In_vLift, const _float4 In_vGamma, const _float4 In_vGain)
{
	m_LiftGammaGainDesc.vLift = In_vLift;
	m_LiftGammaGainDesc.vGamma = In_vGamma;
	m_LiftGammaGainDesc.vGain = In_vGain;

	return S_OK;
}

HRESULT CRender_Manager::Set_GrayScale(const _float In_fGrayScale)
{
	m_fGrayScale = In_fGrayScale;

	return S_OK;
}

HRESULT CRender_Manager::Set_Exposure(const _float In_fExposure)
{
	m_fExposure = In_fExposure;

	return S_OK;
}


HRESULT CRender_Manager::Set_ShadowLight(_fvector In_vEye, _fvector In_vLookAt)
{
	XMStoreFloat3(&m_vShadowLightEye, In_vEye);
	XMStoreFloat3(&m_vShadowLightLookAt, In_vLookAt);

	return S_OK;
}

HRESULT CRender_Manager::Set_DynamicShadowLight(_fvector In_vEye, _fvector In_vLookAt)
{
	XMStoreFloat3(&m_vDynamicShadowLightEye, In_vEye);
	XMStoreFloat3(&m_vDynamicShadowLightLookAt, In_vLookAt);

	return S_OK;
}


HRESULT CRender_Manager::Render_Priority()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;


	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_PRIORITY])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render(pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_PRIORITY].clear();

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_PRIORITY])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render(pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRender_Manager::Render_ShadowDepth()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	if (FAILED(GET_SINGLE(CRenderTarget_Manager)->Begin_ShadowMRT(TEXT("MRT_ShadowDepth"))))
		DEBUG_ASSERT;

	
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	DEVICECONTEXT->RSGetViewports(&iNumViewports, &ViewPortDesc);

	_float fFovy = XMConvertToRadians(60.f);
	_float fAspect = (_float)ViewPortDesc.Width / ViewPortDesc.Height;
	_float fNear = 0.2f;
	_float fFar = 300.f;


	_vector		vLightEye = XMLoadFloat3(&m_vDynamicShadowLightEye);
	_vector		vLightAt = XMLoadFloat3(&m_vDynamicShadowLightLookAt);

	/*_vector		vLightEye = { -iHeight, iHeight, -iHeight };
	_vector		vLightAt = { 0.f, 0.f, 0.f };*/
	_vector		vLightUp = {0.f, 1.f, 0.f};

	_matrix LightViewMatrix = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	LightViewMatrix = XMMatrixTranspose(LightViewMatrix);

	XMStoreFloat4x4(&m_DynamicLightViewMatrixTranspose, LightViewMatrix);

	_matrix LightProjMatrix = XMMatrixPerspectiveFovLH(fFovy, fAspect, fNear, fFar);
	LightProjMatrix = XMMatrixTranspose(LightProjMatrix);

	XMStoreFloat4x4(&m_DynamicLightProjMatrixTranspose, LightProjMatrix);


	//m_pShader->Set_RawValue("g_ViewMatrix", &LightViewMatrix, sizeof(_float4x4));
	//m_pShader->Set_RawValue("g_ProjMatrix", &LightProjMatrix, sizeof(_float4x4));

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_SHADOWDEPTH])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render_ShadowDepth(LightViewMatrix, LightProjMatrix, pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_SHADOWDEPTH].clear();


	if (FAILED(GET_SINGLE(CRenderTarget_Manager)->End_ShadowMRT()))
		DEBUG_ASSERT;

	if (m_bFirst || !m_RenderObjects[(_uint)RENDERGROUP::RENDER_STATICSHADOWDEPTH].empty())
	{
		m_bFirst = false;
		if (FAILED(GET_SINGLE(CRenderTarget_Manager)->Begin_StaticShadowMRT(TEXT("MRT_StaticShadowDepth"))))
			DEBUG_ASSERT;

		vLightEye = XMLoadFloat3(&m_vShadowLightEye);
		vLightAt = XMLoadFloat3(&m_vShadowLightLookAt);

		/*_vector		vLightEye = { -iHeight, iHeight, -iHeight };
		_vector		vLightAt = { 0.f, 0.f, 0.f };*/
		_vector		vLightUp = { 0.f, 1.f, 0.f };

		LightViewMatrix = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
		LightViewMatrix = XMMatrixTranspose(LightViewMatrix);

		XMStoreFloat4x4(&m_LightViewMatrixTranspose, LightViewMatrix);

		LightProjMatrix = XMMatrixPerspectiveFovLH(fFovy, fAspect, fNear, fFar);
		LightProjMatrix = XMMatrixTranspose(LightProjMatrix);

		XMStoreFloat4x4(&m_LightProjMatrixTranspose, LightProjMatrix);

		for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_STATICSHADOWDEPTH])
		{
			if (pGameObject.lock())
				pGameObject.lock()->Render_ShadowDepth(LightViewMatrix, LightProjMatrix, pDeviceContext);
		}

		m_RenderObjects[(_uint)RENDERGROUP::RENDER_STATICSHADOWDEPTH].clear();

		if (FAILED(GET_SINGLE(CRenderTarget_Manager)->End_StaticShadowMRT()))
			DEBUG_ASSERT;
	}
	

	return S_OK;
}

HRESULT CRender_Manager::Render_NonAlphaBlend()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	if (FAILED(GET_SINGLE(CRenderTarget_Manager)->Begin_MRT(TEXT("MRT_Deferred"))))
		DEBUG_ASSERT;

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHABLEND])
	{
		if (pGameObject.lock())
		{
			pGameObject.lock()->Render(pDeviceContext);
			pGameObject.reset();
		}
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHABLEND].clear();

	if (FAILED(GET_SINGLE(CRenderTarget_Manager)->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;
}

HRESULT CRender_Manager::Render_Lights()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	/* 셰이드 타겟을 장치에 바인드한다. */
	if (FAILED(pRenderTargetManager->Begin_MRT( TEXT("MRT_LightAcc"))))
		DEBUG_ASSERT;

	/* 모든 빛은 이 노멀텍스쳐(타겟)과 연산이 이뤄지면 되기때문에.
	모든 빛마다 각각 던질피룡가 없다. */
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DiffuseTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Diffuse")))))
		DEBUG_ASSERT;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_NormalTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Normal")))))
		DEBUG_ASSERT;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Depth")))))
		DEBUG_ASSERT;
	//if (FAILED(m_pShader->Set_ShaderResourceView("g_LightFlagTexture", pRenderTargetManager->Get_SRV(TEXT("Target_LightFlag")))))
	//	DEBUG_ASSERT;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_ORMTexture", pRenderTargetManager->Get_SRV(TEXT("Target_PBR")))))
		DEBUG_ASSERT;

	if (FAILED(m_pIrradianceTextureCom->Set_ShaderResourceView(m_pShader, "g_IrradianceTexture", 0)))
		DEBUG_ASSERT;

	if (FAILED(m_pBRDFLUTTextureCom->Set_ShaderResourceView(m_pShader, "g_BRDFTexture", 0)))
		DEBUG_ASSERT;

	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	shared_ptr<CPipeLine> pPipeLine = GET_SINGLE(CPipeLine);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ))));

	m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));

	m_pShader->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4));

	GET_SINGLE(CLight_Manager)->Render_Lights(m_pShader, m_pVIBuffer, pDeviceContext);

	/*m_pShader->Begin(12, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);
	*/////IBL돌기

	if (FAILED(pRenderTargetManager->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;
}


HRESULT CRender_Manager::Bake_Fog()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	/* 셰이드 타겟을 장치에 바인드한다. */
	if (FAILED(pRenderTargetManager->Begin_MRT(TEXT("MRT_Fog"))))
		DEBUG_ASSERT;

	/* 모든 빛은 이 노멀텍스쳐(타겟)과 연산이 이뤄지면 되기때문에.
	모든 빛마다 각각 던질피룡가 없다. */
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Depth")))))
		DEBUG_ASSERT;

	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	shared_ptr<CPipeLine> pPipeLine = GET_SINGLE(CPipeLine);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ))));

	m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));

	m_pShader->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4));
	m_pShader->Set_RawValue("g_fFogRange", &m_fFogRange, sizeof(_float));
	m_pShader->Set_RawValue("g_vFogColor", &m_vFogColor, sizeof(_float4));


	m_pShader->Begin(11, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	if (FAILED(pRenderTargetManager->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;
}

HRESULT CRender_Manager::Bake_ViewShadow()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	if (FAILED(pRenderTargetManager->Begin_MRT(TEXT("MRT_ViewShadow"))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_ShadowDepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ShadowDepth")))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_StaticShadowDepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_StaticShadowDepth")))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_DepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Depth")))))
		DEBUG_ASSERT;

	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Set_RawValue("g_LightViewMatrix", &m_LightViewMatrixTranspose, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_LightProjMatrix", &m_LightProjMatrixTranspose, sizeof(_float4x4));

	m_pShader->Set_RawValue("g_DynamicLightViewMatrix", &m_DynamicLightViewMatrixTranspose, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_DynamicLightProjMatrix", &m_DynamicLightProjMatrixTranspose, sizeof(_float4x4));

	shared_ptr<CPipeLine> pPipeLine = GET_SINGLE(CPipeLine);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ))));

	m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));

	m_pShader->Begin(10, pDeviceContext);

	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	Blur(1.f, TEXT("MRT_ViewShadow"), TEXT("Target_ViewShadow"));
	Blur(1.f, TEXT("MRT_ViewShadow"), TEXT("Target_ViewShadow"));


	return S_OK;
}

HRESULT CRender_Manager::Render_Blend()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	if (FAILED(m_pShader->Set_ShaderResourceView("g_DiffuseTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Diffuse")))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_ShadeTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Shade")))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_SpecularTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Specular")))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_ShadowDepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ShadowDepth")))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_DepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Depth")))))
		DEBUG_ASSERT;

	//if (FAILED(m_pShader->Set_ShaderResourceView("g_LightFlagTexture", pRenderTargetManager->Get_SRV(TEXT("Target_LightFlag")))))
	//	DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_ViewShadow", pRenderTargetManager->Get_SRV(TEXT("Target_ViewShadow")))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_FogTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Fog")))))
		DEBUG_ASSERT;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_AmbientTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Ambient")))))
		DEBUG_ASSERT;


	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_LightViewMatrix", &m_LightViewMatrixTranspose, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_LightProjMatrix", &m_LightProjMatrixTranspose, sizeof(_float4x4));


	shared_ptr<CPipeLine> pPipeLine = GET_SINGLE(CPipeLine);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ))));

	m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4));

	m_pShader->Begin(3, pDeviceContext);

	/* 사각형 버퍼를 백버퍼위에 그려낸다. */
	m_pVIBuffer->Render(pDeviceContext);

	GET_SINGLE(CCollision_Manager)->Render_Collider();

	return S_OK;
}

HRESULT CRender_Manager::Render_Effect()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	if (SUCCEEDED(Render_NonAlphaEffect(pDeviceContext)))
	{
		if (FAILED(Blur_ExtractGlow(3.f, pDeviceContext)))
			DEBUG_ASSERT;
		//if (FAILED(ReBlur_ExtractGlow(3.f, pDeviceContext)))
		//	DEBUG_ASSERT;
		if (FAILED(Blend_Glow(pDeviceContext)))
			DEBUG_ASSERT;
	}

	if (SUCCEEDED(Render_AlphaBlend(pDeviceContext)))
	{
		if (FAILED(Blur_ExtractGlow(3.f, pDeviceContext)))
			DEBUG_ASSERT;
		//if (FAILED(ReBlur_ExtractGlow(3.f, pDeviceContext)))
		//	DEBUG_ASSERT;
		if (FAILED(Blend_Glow(pDeviceContext)))
			DEBUG_ASSERT;
	}

	return S_OK;
}

HRESULT CRender_Manager::Render_NonLight()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONLIGHT])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render(pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRender_Manager::Render_NonAlphaEffect(ID3D11DeviceContext* pDeviceContext)
{
	for (auto iter = m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHA_EFFECT].begin(); iter != m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHA_EFFECT].end();)
	{
		if (!(*iter).lock())
		{
			iter = m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHA_EFFECT].erase(iter);
		}
		else
		{
			++iter;
		}
	}

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	

	pRenderTargetManager->Begin_MRTWithNoneClearWithIndex(TEXT("MRT_ExtractEffect"), 1);

	if (m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHA_EFFECT].empty())
	{
		pRenderTargetManager->End_MRT();

		return E_FAIL;
	}

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHA_EFFECT])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render(pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHA_EFFECT].clear();

	if (FAILED(pRenderTargetManager->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;
}

HRESULT CRender_Manager::Render_AlphaBlend(ID3D11DeviceContext* pDeviceContext)
{

	for (auto iter = m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHABLEND].begin(); iter != m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHABLEND].end();)
	{
		if (!(*iter).lock())
		{
			iter = m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHABLEND].erase(iter);
		}
		else
		{
			++iter;
		}
	}

	m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHABLEND].sort([](weak_ptr<CGameObject> pSour, weak_ptr<CGameObject> pDest)
		{
			return pSour.lock()->Get_CamDistance() > pDest.lock()->Get_CamDistance();
		});

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);




	pRenderTargetManager->Begin_MRTWithNoneClearWithIndex(TEXT("MRT_ExtractEffect"), 1);

	if (m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHABLEND].empty())
	{
		pRenderTargetManager->End_MRT();

		return E_FAIL;
	}


	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHABLEND])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render(pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHABLEND].clear();

	if (FAILED(pRenderTargetManager->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;
}

HRESULT CRender_Manager::Extract_OutLine()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);
	pRenderTargetManager->Begin_MRT(TEXT("MRT_ExtractOutLine"));

	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_DepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Depth")))))
		DEBUG_ASSERT;

	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_NormalTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Normal")))))
		DEBUG_ASSERT;

	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_ShaderFlagTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ShaderFlag")))))
		DEBUG_ASSERT;

	m_pOutLineShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pOutLineShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pOutLineShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	//_float fPixelWidth = 1 / 1600.f;
	//_float fPixelHeight = 1 / 900.f;
	//m_pOutLineShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	//m_pOutLineShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	_float fDivider = 1.f;
	m_pOutLineShader->Set_RawValue("g_Divider", &fDivider, sizeof(_float));

	m_pOutLineShader->Begin(0, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::Blur_OutLine()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);
	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurOutLine"));

	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_OutLineTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ExtractOutLine")))))
		DEBUG_ASSERT;

	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_ShaderFlagTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ShaderFlag")))))
		DEBUG_ASSERT;

	m_pOutLineShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pOutLineShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pOutLineShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	_float fDivider = 9.f;
	m_pOutLineShader->Set_RawValue("g_Divider", &fDivider, sizeof(_float));

	//_float fPixelWidth = 1 / 1600.f;
	//_float fPixelHeight = 1 / 900.f;
	//m_pOutLineShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	//m_pOutLineShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	m_pOutLineShader->Begin(1, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();


	return S_OK;
}

HRESULT CRender_Manager::Blur_ExtractGlow(const _float& In_PixelPitchScalar, ID3D11DeviceContext* pDeviceContext)
{

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurXForGlow"));

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ExtractGlow")))))
		DEBUG_ASSERT;

	m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	_float fPixelWidth = 1 / 1600.f * In_PixelPitchScalar;
	_float fPixelHeight = 1 / 900.f * In_PixelPitchScalar;
	m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	_float fBlurStrength = 2.f;
	m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));

	// Blur X
	m_pXBlurShader->Begin(3, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	/*if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForGlow")))))
		DEBUG_ASSERT;*/

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture",
		pRenderTargetManager->Get_SRV(TEXT("Target_BlurXForGlow")))))
		DEBUG_ASSERT;

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurForGlow"));

	//m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	//m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	//m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	//m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	//m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));

	// Blur Y
	m_pXBlurShader->Begin(4, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::ReBlur_ExtractGlow(const _float& In_PixelPitchScalar, ID3D11DeviceContext* pDeviceContext)
{

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurXForGlow"));

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForGlow")))))
		DEBUG_ASSERT;

	//m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	//m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	_float fPixelWidth = 1 / 1600.f * In_PixelPitchScalar;
	_float fPixelHeight = 1 / 900.f * In_PixelPitchScalar;
	m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	/*_float fBlurStrength = 1.f;
	m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));*/

	// Blur X
	m_pXBlurShader->Begin(3, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	/*if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForGlow")))))
		DEBUG_ASSERT;*/

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture",
		pRenderTargetManager->Get_SRV(TEXT("Target_BlurXForGlow")))))
		DEBUG_ASSERT;

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurForGlow"));

	//m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	//m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	//m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	//m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	//m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));

	// Blur Y
	m_pXBlurShader->Begin(4, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::Blend_OutLine()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	Bake_OriginalRenderTexture();

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_OriginalRenderTexture", pRenderTargetManager->Get_SRV(TEXT("Target_CopyOriginalRender")))))
		DEBUG_ASSERT;
	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_OutLineTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurOutLine")))))
		DEBUG_ASSERT;
	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_ShaderFlagTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ShaderFlag")))))
		DEBUG_ASSERT;
	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_IntensityFlagTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurOutLineIntensity")))))
		DEBUG_ASSERT;

	m_pOutLineShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pOutLineShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pOutLineShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	//_float fPixelWidth = 1 / 1600.f;
	//_float fPixelHeight = 1 / 900.f;
	//m_pOutLineShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	//m_pOutLineShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	m_pOutLineShader->Begin(2, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);


	return S_OK;
}

HRESULT CRender_Manager::Extract_Distortion()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);



	if (m_RenderObjects[(_uint)RENDERGROUP::RENDER_DISTORTION].empty())
	{
		return E_FAIL;
	}

	if (FAILED(pRenderTargetManager->Begin_MRT(TEXT("MRT_Distortion"))))
		DEBUG_ASSERT;

		
	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_DISTORTION])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render(pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_DISTORTION].clear();

	if (FAILED(GET_SINGLE(CRenderTarget_Manager)->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;
}

HRESULT CRender_Manager::Blend_Distortion()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	Bake_OriginalRenderTexture();

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	if (FAILED(m_pDistortionShader->Set_ShaderResourceView("g_DistortionTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Distortion")))))
		DEBUG_ASSERT;
	if (FAILED(m_pDistortionShader->Set_ShaderResourceView("g_OriginTexture", pRenderTargetManager->Get_SRV(TEXT("Target_CopyOriginalRender")))))
		DEBUG_ASSERT;

 	m_pDistortionShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pDistortionShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pDistortionShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pDistortionShader->Begin(0, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	return S_OK;
}

HRESULT CRender_Manager::Blend_Glow(ID3D11DeviceContext* pDeviceContext)
{

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Set_ShaderResourceView("g_GlowTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForGlow")));
	m_pShader->Set_ShaderResourceView("g_OriginalEffectTexture", pRenderTargetManager->Get_SRV(TEXT("Target_OriginalEffect")));
	//m_pShader->Set_ShaderResourceView("g_OriginalRenderTexture", GET_SINGLE(CGraphic_Device)->Get_SRV());


	m_pShader->Begin(6, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	/*m_pShader->Begin(7);
	m_pVIBuffer->Render(pDeviceContext);*/


	return S_OK;
}

HRESULT CRender_Manager::Render_UI()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;
	// ID3D11DeviceContext* pDeviceContext = m_pDeferredContext[DEFERRED_UI].Get();

	m_RenderObjects[(_uint)RENDERGROUP::RENDER_BEFOREUI].sort([](weak_ptr<CGameObject> pSour, weak_ptr<CGameObject> pDest)
		{
			if (!pSour.lock() || !pDest.lock())
				return false;

			return pSour.lock()->Get_CamDistance() > pDest.lock()->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_BEFOREUI])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render(pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_BEFOREUI].clear();

	GET_SINGLE(CFont_Manager)->Render_AllFontWithRenderGroup(RENDERGROUP::RENDER_BEFOREUI);

	m_RenderObjects[(_uint)RENDERGROUP::RENDER_UI].sort([](weak_ptr<CGameObject> pSour, weak_ptr<CGameObject> pDest)
		{
			if (!pSour.lock() || !pDest.lock())
				return false;

			return pSour.lock()->Get_CamDistance() > pDest.lock()->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_UI])
	{

		if (pGameObject.lock())
			pGameObject.lock()->Render(pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_UI].clear();

	GET_SINGLE(CFont_Manager)->Render_AllFontWithRenderGroup(RENDERGROUP::RENDER_UI);

	m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_UI].sort([](weak_ptr<CGameObject> pSour, weak_ptr<CGameObject> pDest)
		{
			if (!pSour.lock() || !pDest.lock())
				return false;

			return pSour.lock()->Get_CamDistance() > pDest.lock()->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_UI])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render(pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_UI].clear();

	GET_SINGLE(CFont_Manager)->Render_AllFontWithRenderGroup(RENDERGROUP::RENDER_AFTER_UI);

	return S_OK;
}

HRESULT CRender_Manager::Render_Font()
{


	return S_OK;
}

HRESULT CRender_Manager::Extract_Brightness()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);


	pRenderTargetManager->Begin_MRTWithNoneClear(TEXT("MRT_ExtractEffect"));

	m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pXBlurShader->Set_ShaderResourceView("g_OriginalRenderTexture", GET_SINGLE(CGraphic_Device)->Get_SRV());

	// Extract Brightness
	m_pXBlurShader->Begin(2, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::Blur_ExtractBloom()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurXForBloom"));

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ExtractBloom")))))
		DEBUG_ASSERT;

	m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	_float fPixelWidth = 1 / 1600.f * 3.f;
	_float fPixelHeight = 1 / 900.f * 3.f;
	m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	_float fBlurStrength = 1.f;
	m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));
	// Blur X
	m_pXBlurShader->Begin(5, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurForBloom"));

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurXForBloom")))))
		DEBUG_ASSERT;

	// Blur Y
	m_pXBlurShader->Begin(6, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::ReBlur_ExtractBloom()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurXForBloom"));

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForBloom")))))
		DEBUG_ASSERT;

	/*m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	_float fPixelWidth = 1 / 1600.f * 3.f;
	_float fPixelHeight = 1 / 900.f * 3.f;
	m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	_float fBlurStrength = 1.1f;
	m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));*/
	// Blur X
	m_pXBlurShader->Begin(5, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurForBloom"));

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurXForBloom")))))
		DEBUG_ASSERT;

	// Blur Y
	m_pXBlurShader->Begin(6, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	return S_OK;
}



HRESULT CRender_Manager::Blend_Bloom()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	this->Bake_OriginalRenderTexture();

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);


	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Set_ShaderResourceView("g_ShaderFlagTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ShaderFlag")));
		
	m_pShader->Set_ShaderResourceView("g_ExtractBloomTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ExtractBloom")));
	m_pShader->Set_ShaderResourceView("g_XBlurTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForBloom")));
	m_pShader->Set_ShaderResourceView("g_OriginalRenderTexture", pRenderTargetManager->Get_SRV(TEXT("Target_CopyOriginalRender")));

	/* 블룸 만들기. */
	m_pShader->Begin(5, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);


	return S_OK;
}

HRESULT CRender_Manager::PostProcessing()
{
	//모션 블러, 색수차, 중심 블러(얘는 위에 있음) 등등 화면 전체 해야하는 블러의 경우
	//Bake_OriginalRenderTexture();

	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	//if (FAILED(m_pPostProcessingShader->Set_ShaderResourceView("g_OriginalRenderTexture", pRenderTargetManager->Get_SRV(TEXT("Target_CopyOriginalRender")))))
	//	DEBUG_ASSERT;

	if (FAILED(m_pPostProcessingShader->Set_ShaderResourceView("g_DepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Depth")))))
		DEBUG_ASSERT;

	m_pPostProcessingShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pPostProcessingShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pPostProcessingShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	
	shared_ptr<CPipeLine> pPipeLine = GET_SINGLE(CPipeLine);

	_float4x4		ViewMatrixInv, ProjMatrixInv,ViewMatrix,ProjMatrix;
	_matrix			CamViewMatrix= pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW);


	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, CamViewMatrix)));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ))));

	ProjMatrix = *pPipeLine->Get_Transform_TP(CPipeLine::D3DTS_PROJ);
	ViewMatrix = *pPipeLine->Get_Transform_TP(CPipeLine::D3DTS_VIEW);


	m_pPostProcessingShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));
	m_pPostProcessingShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_pPostProcessingShader->Set_RawValue("g_CamProjMatrix", &ProjMatrix, sizeof(_float4x4));

	m_pPostProcessingShader->Set_RawValue("g_vCamPosition", &ViewMatrixInv.m[3],sizeof(_float4));

	m_pPostProcessingShader->Set_RawValue("g_vLift", &m_LiftGammaGainDesc.vLift, sizeof(_float4));
	m_pPostProcessingShader->Set_RawValue("g_vGamma", &m_LiftGammaGainDesc.vGamma, sizeof(_float4));
	m_pPostProcessingShader->Set_RawValue("g_vGain", &m_LiftGammaGainDesc.vGain, sizeof(_float4));

	m_pPostProcessingShader->Set_RawValue("g_fGrayScale", &m_fGrayScale, sizeof(_float));
	m_pPostProcessingShader->Set_RawValue("g_fSaturation", &m_fSaturation, sizeof(_float));
	m_pPostProcessingShader->Set_RawValue("g_fContrastValue", &m_fContrastValue, sizeof(_float)); 

	_float fChromaticLerpValue = 0.f;
	if (0.f < m_fChromaticStrengthAcc)
	{
		_vector vLerp = XMVectorSet(m_fChromaticStrangth, 0.f, 0.f, 0.f);
		vLerp = CEasing_Utillity::CubicOut(XMVectorSet(0.f, 0.f, 0.f, 0.f), vLerp, m_fChromaticStrengthAcc, m_fChromaticStrangth);
		fChromaticLerpValue = vLerp.m128_f32[0];
	}

	_float fRadialLerpValue = 0.f;
	//if (0.f < m_fRadialBlurStrengthAcc)
	//{
	//	_vector vLerp = XMVectorSet(m_fRadialBlurStrength, 0.f, 0.f, 0.f);
	//	vLerp = CEasing_Utillity::CubicOut(XMVectorSet(0.f, 0.f, 0.f, 0.f), vLerp, m_fRadialBlurStrengthAcc, m_fRadialBlurStrength);
	//	fRadialLerpValue = vLerp.m128_f32[0];
	//}


	_float fMotionLerpValue = 0.f;
	//if (0.f < m_fMotionBlurStrengthAcc)
	//{
	//	_vector vLerp = XMVectorSet(m_fMotionBlurStrength, 0.f, 0.f, 0.f);
	//	vLerp = CEasing_Utillity::CubicOut(XMVectorSet(0.f, 0.f, 0.f, 0.f), vLerp, m_fMotionBlurStrengthAcc, m_fMotionBlurStrength);
	//	fMotionLerpValue = vLerp.m128_f32[0];
	//}

	fMotionLerpValue *= 0.01f;


	m_pPostProcessingShader->Set_RawValue("g_fChromaticStrength", &fChromaticLerpValue, sizeof(_float));//chromatic 전용

	m_pPostProcessingShader->Set_RawValue("g_fMotionBlurStrength", &m_fMotionBlurStrengthAcc, sizeof(_float));//MotionBlur 전용
	m_pPostProcessingShader->Set_RawValue("g_PreCamViewMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&pPipeLine->Get_PreViewMatrix())), sizeof(_float4x4));

	m_pPostProcessingShader->Set_RawValue("g_fRadialBlurStrength", &m_fRadialBlurStrengthAcc, sizeof(_float));//RadialBlur 전용
	m_pPostProcessingShader->Set_RawValue("g_vBlurWorldPosition", &m_vRadialBlurWorldPos, sizeof(_float3));//RadialBlur 전용
	m_pPostProcessingShader->Set_RawValue("g_CameraViewMatrix", &XMMatrixTranspose(CamViewMatrix), sizeof(_float4x4));//RadialBlur 전용


	for (_int i = 0; i < 5; ++i)
	{
		Bake_OriginalRenderTexture();
		if (FAILED(m_pPostProcessingShader->Set_ShaderResourceView("g_OriginalRenderTexture", pRenderTargetManager->Get_SRV(TEXT("Target_CopyOriginalRender")))))
			DEBUG_ASSERT;

		m_pPostProcessingShader->Begin(i, pDeviceContext);
		m_pVIBuffer->Render(pDeviceContext);
	}


	return S_OK;
}

HRESULT CRender_Manager::AntiAliasing()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	Bake_OriginalRenderTexture();

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_AntiAliasingMRT(TEXT("MRT_AntiAliasing"));

	if (FAILED(m_pShader->Set_ShaderResourceView("g_OriginalRenderTexture", pRenderTargetManager->Get_SRV(TEXT("Target_CopyOriginalRender")))))
		DEBUG_ASSERT;

	m_pShader->Set_RawValue("g_WorldMatrix", &m_AntiAliasingWorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_AntiAliasingProjMatrixTranspose, sizeof(_float4x4));

	m_pShader->Begin(7, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_AntiAliasingMRT();

	if (FAILED(m_pShader->Set_ShaderResourceView("g_OriginalRenderTexture", pRenderTargetManager->Get_SRV(TEXT("Target_AntiAliasing")))))
		DEBUG_ASSERT;

	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Begin(7, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	return S_OK;
}

HRESULT CRender_Manager::Render_Final()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_FINAL])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render(pDeviceContext);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_FINAL].clear();

	return S_OK;
}

HRESULT CRender_Manager::Bake_OriginalRenderTexture()
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_MRT(TEXT("MRT_CopyOriginalRender"));

	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Set_ShaderResourceView("g_OriginalRenderTexture", GET_SINGLE(CGraphic_Device)->Get_SRV());

	/* 블룸 만들기. */
	m_pShader->Begin(7, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::Blur(const _float& In_PixelPitchScalar, const _tchar* In_szMRT, const _tchar* In_szTarget)
{
	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_MRT(TEXT("MRT_PublicBlur"));

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(In_szTarget))))
		DEBUG_ASSERT;

	m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	_float fPixelWidth = 1 / 1600.f * In_PixelPitchScalar;
	_float fPixelHeight = 1 / 900.f * In_PixelPitchScalar;
	m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	_float fBlurStrength = 1.f;
	m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));

	// Blur X
	m_pXBlurShader->Begin(0, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	pRenderTargetManager->Begin_MRT(In_szMRT);

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture",
		pRenderTargetManager->Get_SRV(TEXT("Target_PublicBlur")))))
		DEBUG_ASSERT;

	m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));

	// Blur Y
	m_pXBlurShader->Begin(1, pDeviceContext);
	m_pVIBuffer->Render(pDeviceContext);

	pRenderTargetManager->End_MRT();

	return S_OK;
}

void CRender_Manager::Emplace_SleepContext(const _uint In_iIndex)
{
	ID3D11DeviceContext* pContext = nullptr;
	if (SUCCEEDED(DEVICE->CreateDeferredContext(0, &pContext)))
	{
		GET_SINGLE(CGraphic_Device)->SyncronizeDeferredContext(pContext);
		m_pBeforeRenderSleepContexts.emplace_back(pContext);
	}
	
}

#ifdef _DEBUG
HRESULT CRender_Manager::Render_Debug()
{
	m_pDebugComponents.clear();

	/*if (!GAMEINSTANCE->Is_Debug())
	{
		m_pDebugComponents.clear();
		return S_OK;
	}



	m_pDebugComponents.clear();*/

	if (!GAMEINSTANCE->Is_Debug())
	{
		m_szDebugRenderMRTNames.clear();
		return S_OK;
	}





	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		DEBUG_ASSERT;

	if (m_bOldSchoolView)
	{
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_Deferred"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_ShadowDepth"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_ExtractEffect"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_BlurForBloom"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_BlurForGlow"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_CopyOriginalRender"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_BlurEffect"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_ExtractOutLine"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_BlurOutLine"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_ViewShadow"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_StaticShadowDepth"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_Fog"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_Distortion"), m_pShader, m_pVIBuffer);
		GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_AntiAliasing"), m_pShader, m_pVIBuffer);
	}
	else
	{
		for (_size_t i = 0; i < m_szDebugRenderMRTNames.size(); ++i)
		{
			Render_DebugSRT((_uint)i);
		}		
	}


	m_szDebugRenderMRTNames.clear();
	return S_OK;
}

HRESULT CRender_Manager::Set_DebugSize(const _float2 vSize)
{
	m_vDebugSize = vSize;

	return S_OK;
}

HRESULT CRender_Manager::Set_OldSchoolView(const _bool bOldSchool)
{
	m_bOldSchoolView = bOldSchool;

	return S_OK;
}

HRESULT CRender_Manager::Add_DebugSRT(const _tchar* In_szMRTName)
{
	m_szDebugRenderMRTNames.emplace_back(In_szMRTName);

	return S_OK;
}

void CRender_Manager::Render_DebugSRT(const _uint In_iIndex)
{
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	DEVICECONTEXT->RSGetViewports(&iNumViewports, &ViewPortDesc);

	_matrix		WorldMatrix = XMMatrixIdentity();
	_float fX = (m_vDebugSize.x * 0.5f);
	_float fY = (m_vDebugSize.y * 0.5f);
	//fY += In_iIndex * m_vDebugSize.y;

	_int iH = ViewPortDesc.Height / m_vDebugSize.y;
	_int iW = In_iIndex / iH;
	iH = In_iIndex % iH;


	fX += iW * m_vDebugSize.x;
	fY += iH * m_vDebugSize.y;

	WorldMatrix.r[0] = XMVectorSet(m_vDebugSize.x, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, m_vDebugSize.y, 0.f, 0.f);
	WorldMatrix.r[3] = XMVectorSet(fX - (ViewPortDesc.Width * 0.5f), -fY + (ViewPortDesc.Height * 0.5f), 0.f, 1.f);

	WorldMatrix = XMMatrixTranspose(WorldMatrix);

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		DEBUG_ASSERT;

	GET_SINGLE(CRenderTarget_Manager)->Render_DebugSRT(m_szDebugRenderMRTNames[In_iIndex].c_str(), m_pShader, m_pVIBuffer);

}

//HRESULT CRender_Manager::Add_DebugRenderGroup(weak_ptr<CComponent> pComponent)
//{
//	m_pDebugComponents.push_back(pComponent);
//
//	return S_OK;
//}


#endif // _DEBUG




void CRender_Manager::OnDestroy()
{
	std::unique_lock<std::mutex> lock(m_job_q_);

	m_pShader->OnDestroy();
	m_pVIBuffer->OnDestroy();
	m_pDeferredContext[DEFERRED_UI].Reset();

	for (auto& elem : m_pBeforeRenderContexts)
	{
		elem->Release();
	}
	m_pBeforeRenderContexts.clear();

	for (auto& elem : m_pBeforeRenderSleepContexts)
	{
		elem->Release();
	}
	m_pBeforeRenderSleepContexts.clear();

	lock.unlock();
}

void CRender_Manager::OnEngineEventMessage(const ENGINE_EVENT_TYPE In_eEngineEvent)
{
	if (In_eEngineEvent == ENGINE_EVENT_TYPE::ON_SHADER_UPDATE)
	{
		m_pShader->Set_ShaderInfo_Internal();
		m_pXBlurShader->Set_ShaderInfo_Internal();
		m_pOutLineShader->Set_ShaderInfo_Internal();
		m_pDistortionShader->Set_ShaderInfo_Internal();
		m_pPostProcessingShader->Set_ShaderInfo_Internal();
	}

}

void CRender_Manager::Free()
{
	m_pShader.reset();
	m_pXBlurShader.reset();
	m_pOutLineShader.reset();
	m_pDistortionShader.reset();
	m_pPostProcessingShader.reset();
	m_pVIBuffer.reset();
}
