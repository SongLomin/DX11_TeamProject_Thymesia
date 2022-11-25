#include "Render_Manager.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"


IMPLEMENT_SINGLETON(CRender_Manager)

HRESULT CRender_Manager::Initialize()
{

	/* 렌더타겟들을 추가한다. */

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	DEVICECONTEXT->RSGetViewports(&iNumViewports, &ViewPortDesc);

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

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
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 1.f))))
		DEBUG_ASSERT;

	/* For.Target_Shade */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Shade"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		DEBUG_ASSERT;

	/* For.Target_Specular */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Specular"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;
	/*For.Target_Fog*/
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Fog"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	/* For.Target_LightFlag */
	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_LightFlag"), 
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


	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Bloom"), 
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 0.f, 1.f))))
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

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_ViewShadow"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_PublicBlur"),
		(_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_RenderTarget(TEXT("Target_Distortion"),
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
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_LightFlag"))))
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
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Bloom"), TEXT("Target_Bloom"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_BlurEffect"), TEXT("Target_BlurForEffect"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_ExtractOutLine"), TEXT("Target_ExtractOutLine"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_BlurOutLine"), TEXT("Target_BlurOutLine"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_ViewShadow"), TEXT("Target_ViewShadow"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_PublicBlur"), TEXT("Target_PublicBlur"))))
		DEBUG_ASSERT;

	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Distortion"), TEXT("Target_Distortion"))))
		DEBUG_ASSERT;

	//if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_Glow"), TEXT("Target_Glow"))))
	//	DEBUG_ASSERT;


	/* For.MRT_LightAcc */
	/* 조명연산한 결과를 저장해놓기위한 타겟 (Shade + Specular) */
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
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
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_LightFlag"), fHalf, fHalf + fSize * 3, fSize, fSize)))
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
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_Bloom"), ViewPortDesc.Width - fHalf, fHalf + fSize * 3.f, fSize, fSize)))
		DEBUG_ASSERT;


	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_ExtractGlow"), ViewPortDesc.Width - fHalf - fSize, fHalf, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_BlurForGlow"), ViewPortDesc.Width - fHalf - fSize, fHalf + fSize, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_OriginalEffect"), ViewPortDesc.Width - fHalf - fSize, fHalf + fSize * 2.f, fSize, fSize)))
		DEBUG_ASSERT;
	if (FAILED(pRenderTargetManager->Ready_Debug(TEXT("Target_BlurForEffect"), ViewPortDesc.Width - fHalf - fSize, fHalf + fSize * 3.f, fSize, fSize)))
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
	
	GAMEINSTANCE->Load_Textures("PostEffectMask", TEXT("../Bin/Resources/Textures/UI/PostEffectMask.bmp"));

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

	if (FAILED(Render_Lights()))
		DEBUG_ASSERT;

	/*if (FAILED(Bake_Fog()))
		DEBUG_ASSERT;*/

	if (FAILED(Bake_ViewShadow()))
		DEBUG_ASSERT;

	if (FAILED(Render_Blend()))
		DEBUG_ASSERT;

	//if (FAILED(Blend_OutLine()))
	//	DEBUG_ASSERT;



	if (FAILED(Render_NonLight()))
		DEBUG_ASSERT;
	if (FAILED(Render_AlphaBlend()))
		DEBUG_ASSERT;

	if (FAILED(Extract_Distortion()))
		DEBUG_ASSERT;
	if (FAILED(Blend_Distortion()))
		DEBUG_ASSERT;

	if (FAILED(Blur_ExtractGlow(3.f)))
		DEBUG_ASSERT;
	if (FAILED(ReBlur_ExtractGlow(3.f)))
		DEBUG_ASSERT;

	if (FAILED(Blend_Glow()))
		DEBUG_ASSERT;
	
	if (FAILED(Render_Font()))
		DEBUG_ASSERT;

	if (FAILED(Blur_ExtractBloom()))
		DEBUG_ASSERT;
	if (FAILED(Blend_Bloom()))
		DEBUG_ASSERT;
	if (FAILED(Render_PostEffect()))
		DEBUG_ASSERT;

	if (FAILED(Render_AfterPostEffect()))
		DEBUG_ASSERT;

	if (FAILED(Render_AfterPostEffectGlow()))
		DEBUG_ASSERT;

	if (FAILED(PostProcessing(0)))
		DEBUG_ASSERT;

	if (FAILED(PostProcessing(1)))
		DEBUG_ASSERT;

	if (FAILED(Render_UI()))
		DEBUG_ASSERT;


	GET_SINGLE(CFont_Manager)->Render_AllFont();

	if (FAILED(Render_Final()))
		DEBUG_ASSERT;

#ifdef _DEBUG

	if (FAILED(Render_Debug()))
		DEBUG_ASSERT;
#endif 

	return S_OK;
  }

HRESULT CRender_Manager::Set_MotionBlur(const _float& In_fBlurScale)
{
	m_fBlurWitdh = In_fBlurScale;

	return S_OK;
}

HRESULT CRender_Manager::Add_MotionBlur(const _float& In_fBlurScale)
{
	m_fBlurWitdh += In_fBlurScale;
	m_fBlurWitdh = max(0.f, m_fBlurWitdh);

	return S_OK;
}

HRESULT CRender_Manager::Set_Chromatic(const _float In_fChormaticStrangth)
{
	m_fChromaticStrangth = In_fChormaticStrangth;

	return S_OK;
}

HRESULT CRender_Manager::Add_Chromatic(const _float In_fChormaticStrangth)
{
	m_fChromaticStrangth += In_fChormaticStrangth;
	m_fChromaticStrangth = max(0.f, m_fChromaticStrangth);

	return S_OK;
}

HRESULT CRender_Manager::Set_ShadowLight(_fvector In_vEye, _fvector In_vLookAt)
{
	XMStoreFloat3(&m_vShadowLightEye, In_vEye);
	XMStoreFloat3(&m_vShadowLightLookAt, In_vLookAt);

	return S_OK;
}

HRESULT CRender_Manager::Render_Priority()
{

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_PRIORITY])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_PRIORITY].clear();

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_PRIORITY])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRender_Manager::Render_ShadowDepth()
{
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


	_vector		vLightEye = XMLoadFloat3(&m_vShadowLightEye);
	_vector		vLightAt = XMLoadFloat3(&m_vShadowLightLookAt);

	/*_vector		vLightEye = { -iHeight, iHeight, -iHeight };
	_vector		vLightAt = { 0.f, 0.f, 0.f };*/
	_vector		vLightUp = {0.f, 1.f, 0.f};

	_matrix LightViewMatrix = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	LightViewMatrix = XMMatrixTranspose(LightViewMatrix);

	XMStoreFloat4x4(&m_LightViewMatrixTranspose, LightViewMatrix);

	_matrix LightProjMatrix = XMMatrixPerspectiveFovLH(fFovy, fAspect, fNear, fFar);
	LightProjMatrix = XMMatrixTranspose(LightProjMatrix);

	XMStoreFloat4x4(&m_LightProjMatrixTranspose, LightProjMatrix);


	//m_pShader->Set_RawValue("g_ViewMatrix", &LightViewMatrix, sizeof(_float4x4));
	//m_pShader->Set_RawValue("g_ProjMatrix", &LightProjMatrix, sizeof(_float4x4));

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_SHADOWDEPTH])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render_ShadowDepth(LightViewMatrix, LightProjMatrix);
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_SHADOWDEPTH].clear();


	if (FAILED(GET_SINGLE(CRenderTarget_Manager)->End_ShadowMRT()))
		DEBUG_ASSERT;

	if (m_bFirst || !m_RenderObjects[(_uint)RENDERGROUP::RENDER_STATICSHADOWDEPTH].empty())
	{
		m_bFirst = false;
		if (FAILED(GET_SINGLE(CRenderTarget_Manager)->Begin_StaticShadowMRT(TEXT("MRT_StaticShadowDepth"))))
			DEBUG_ASSERT;

		for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_STATICSHADOWDEPTH])
		{
			if (pGameObject.lock())
				pGameObject.lock()->Render_ShadowDepth(LightViewMatrix, LightProjMatrix);
		}

		m_RenderObjects[(_uint)RENDERGROUP::RENDER_STATICSHADOWDEPTH].clear();

		if (FAILED(GET_SINGLE(CRenderTarget_Manager)->End_StaticShadowMRT()))
			DEBUG_ASSERT;
	}
	

	return S_OK;
}

HRESULT CRender_Manager::Render_NonAlphaBlend()
{
	if (FAILED(GET_SINGLE(CRenderTarget_Manager)->Begin_MRT(TEXT("MRT_Deferred"))))
		DEBUG_ASSERT;

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHABLEND])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONALPHABLEND].clear();

	if (FAILED(GET_SINGLE(CRenderTarget_Manager)->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;

	return S_OK;
}

HRESULT CRender_Manager::Render_Lights()
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	/* 셰이드 타겟을 장치에 바인드한다. */
	if (FAILED(pRenderTargetManager->Begin_MRT( TEXT("MRT_LightAcc"))))
		DEBUG_ASSERT;

	/* 모든 빛은 이 노멀텍스쳐(타겟)과 연산이 이뤄지면 되기때문에.
	모든 빛마다 각각 던질피룡가 없다. */
	if (FAILED(m_pShader->Set_ShaderResourceView("g_NormalTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Normal")))))
		DEBUG_ASSERT;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Depth")))))
		DEBUG_ASSERT;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_LightFlagTexture", pRenderTargetManager->Get_SRV(TEXT("Target_LightFlag")))))
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

	GET_SINGLE(CLight_Manager)->Render_Lights(m_pShader, m_pVIBuffer);

	if (FAILED(pRenderTargetManager->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;
}

HRESULT CRender_Manager::Bake_Fog()
{
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

	m_pShader->Begin(11);
	m_pVIBuffer->Render();

	if (FAILED(pRenderTargetManager->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;
}

HRESULT CRender_Manager::Bake_ViewShadow()
{
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

	shared_ptr<CPipeLine> pPipeLine = GET_SINGLE(CPipeLine);

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ))));

	m_pShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));

	m_pShader->Begin(10);

	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	Blur(1.f, TEXT("MRT_ViewShadow"), TEXT("Target_ViewShadow"));
	Blur(1.f, TEXT("MRT_ViewShadow"), TEXT("Target_ViewShadow"));


	return S_OK;
}

HRESULT CRender_Manager::Render_Blend()
{
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

	if (FAILED(m_pShader->Set_ShaderResourceView("g_LightFlagTexture", pRenderTargetManager->Get_SRV(TEXT("Target_LightFlag")))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_ViewShadow", pRenderTargetManager->Get_SRV(TEXT("Target_ViewShadow")))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_FogTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Fog")))))
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

	m_pShader->Begin(3);

	/* 사각형 버퍼를 백버퍼위에 그려낸다. */
	m_pVIBuffer->Render();

	GET_SINGLE(CCollision_Manager)->Render_Collider();

	return S_OK;
}

HRESULT CRender_Manager::Render_NonLight()
{
	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONLIGHT])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRender_Manager::Render_AlphaBlend()
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


	pRenderTargetManager->Begin_MRT(TEXT("MRT_ExtractEffect"));

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHABLEND])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_ALPHABLEND].clear();

	if (FAILED(pRenderTargetManager->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;
}

HRESULT CRender_Manager::Extract_OutLine()
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);
	pRenderTargetManager->Begin_MRT(TEXT("MRT_ExtractOutLine"));

	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_DepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Depth")))))
		DEBUG_ASSERT;

	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_NormalTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Normal")))))
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

	m_pOutLineShader->Begin(0);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::Blur_OutLine()
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);
	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurOutLine"));

	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_OutLineTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ExtractOutLine")))))
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

	m_pOutLineShader->Begin(1);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::Blur_ExtractGlow(const _float& In_PixelPitchScalar)
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
	m_pXBlurShader->Begin(3);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	/*if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForGlow")))))
		DEBUG_ASSERT;*/

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", 
		pRenderTargetManager->Get_SRV(TEXT("Target_BlurXForGlow")))))
		DEBUG_ASSERT;

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurForGlow"));

	m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));

	// Blur Y
	m_pXBlurShader->Begin(4);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::ReBlur_ExtractGlow(const _float& In_PixelPitchScalar)
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurXForGlow"));

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForGlow")))))
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
	m_pXBlurShader->Begin(3);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	/*if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForGlow")))))
		DEBUG_ASSERT;*/

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture",
		pRenderTargetManager->Get_SRV(TEXT("Target_BlurXForGlow")))))
		DEBUG_ASSERT;

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurForGlow"));

	m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));

	// Blur Y
	m_pXBlurShader->Begin(4);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::Blend_OutLine()
{
	Bake_OriginalRenderTexture();

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_OriginalRenderTexture", pRenderTargetManager->Get_SRV(TEXT("Target_CopyOriginalRender")))))
		DEBUG_ASSERT;
	if (FAILED(m_pOutLineShader->Set_ShaderResourceView("g_OutLineTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurOutLine")))))
		DEBUG_ASSERT;

	m_pOutLineShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pOutLineShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pOutLineShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	//_float fPixelWidth = 1 / 1600.f;
	//_float fPixelHeight = 1 / 900.f;
	//m_pOutLineShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	//m_pOutLineShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	m_pOutLineShader->Begin(2);
	m_pVIBuffer->Render();


	return S_OK;
}

HRESULT CRender_Manager::Extract_Distortion()
{
	if (FAILED(GET_SINGLE(CRenderTarget_Manager)->Begin_MRT(TEXT("MRT_Distortion"))))
		DEBUG_ASSERT;

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_DISTORTION])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_DISTORTION].clear();

	if (FAILED(GET_SINGLE(CRenderTarget_Manager)->End_MRT()))
		DEBUG_ASSERT;

	return S_OK;
}

HRESULT CRender_Manager::Blend_Distortion()
{
	Bake_OriginalRenderTexture();

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	if (FAILED(m_pDistortionShader->Set_ShaderResourceView("g_DistortionTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Distortion")))))
		DEBUG_ASSERT;
	if (FAILED(m_pDistortionShader->Set_ShaderResourceView("g_OriginTexture", pRenderTargetManager->Get_SRV(TEXT("Target_CopyOriginalRender")))))
		DEBUG_ASSERT;

 	m_pDistortionShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pDistortionShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pDistortionShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pDistortionShader->Begin(0);
	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRender_Manager::Blend_Glow()
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Set_ShaderResourceView("g_GlowTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForGlow")));
	m_pShader->Set_ShaderResourceView("g_OriginalEffectTexture", pRenderTargetManager->Get_SRV(TEXT("Target_OriginalEffect")));
	//m_pShader->Set_ShaderResourceView("g_OriginalRenderTexture", GET_SINGLE(CGraphic_Device)->Get_SRV());


	m_pShader->Begin(6);
	m_pVIBuffer->Render();

	/*m_pShader->Begin(7);
	m_pVIBuffer->Render();*/


	return S_OK;
}

HRESULT CRender_Manager::Render_UI()
{
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_BEFOREUI].sort([](weak_ptr<CGameObject> pSour, weak_ptr<CGameObject> pDest)
		{
			return pSour.lock()->Get_CamDistance() > pDest.lock()->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_BEFOREUI])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_BEFOREUI].clear();

	m_RenderObjects[(_uint)RENDERGROUP::RENDER_UI].sort([](weak_ptr<CGameObject> pSour, weak_ptr<CGameObject> pDest)
		{
			return pSour.lock()->Get_CamDistance() > pDest.lock()->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_UI])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_UI].clear();

	m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_UI].sort([](weak_ptr<CGameObject> pSour, weak_ptr<CGameObject> pDest)
		{
			return pSour.lock()->Get_CamDistance() > pDest.lock()->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_UI])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_UI].clear();

	return S_OK;
}

HRESULT CRender_Manager::Render_Font()
{


	return S_OK;
}

HRESULT CRender_Manager::Extract_Brightness()
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);


	pRenderTargetManager->Begin_MRTWithNoneClear(TEXT("MRT_ExtractEffect"));

	m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pXBlurShader->Set_ShaderResourceView("g_OriginalRenderTexture", GET_SINGLE(CGraphic_Device)->Get_SRV());

	// Extract Brightness
	m_pXBlurShader->Begin(2);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::Blur_ExtractBloom()
{
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
	m_pXBlurShader->Begin(0);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurForBloom"));

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurXForBloom")))))
		DEBUG_ASSERT;

	// Blur Y
	m_pXBlurShader->Begin(1);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	return S_OK;
}



HRESULT CRender_Manager::Blend_Bloom()
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_MRT(TEXT("MRT_Bloom"));

	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Set_ShaderResourceView("g_ExtractBloomTexture", pRenderTargetManager->Get_SRV(TEXT("Target_ExtractBloom")));
	m_pShader->Set_ShaderResourceView("g_XBlurTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForBloom")));
	m_pShader->Set_ShaderResourceView("g_OriginalRenderTexture", GET_SINGLE(CGraphic_Device)->Get_SRV());

	/* 블룸 만들기. */
	m_pShader->Begin(5);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::Render_PostEffect()
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	
	if (m_fBlurWitdh < 0.f)
		m_fBlurWitdh = 0.f;

	m_pShader->Set_RawValue("g_fBlurWidth", &m_fBlurWitdh, sizeof(_float));

	//m_pShader->Set_ShaderResourceView("g_PostEffectMaskTexture", GAMEINSTANCE->Get_TexturesFromKey("PostEffectMask").front());
	m_pShader->Set_ShaderResourceView("g_BloomTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Bloom")));
	


	/* 셰이더 합치기. */
	m_pShader->Begin(4);
	m_pVIBuffer->Render();

	

	return S_OK;
}



HRESULT CRender_Manager::Render_AfterPostEffect()
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	Bake_OriginalRenderTexture();

	

	m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_POSTPROCESS].sort([](weak_ptr<CGameObject> pSour, weak_ptr<CGameObject> pDest)
		{
			return pSour.lock()->Get_CamDistance() < pDest.lock()->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_POSTPROCESS])
	{

		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_POSTPROCESS].clear();

	pRenderTargetManager->Begin_MRT(TEXT("MRT_ExtractEffect"));
	
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_POSTPROCESS_EFFECT].sort([](weak_ptr<CGameObject> pSour, weak_ptr<CGameObject> pDest)
		{
			return pSour.lock()->Get_CamDistance() < pDest.lock()->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_POSTPROCESS_EFFECT])
	{

		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}

	m_RenderObjects[(_uint)RENDERGROUP::RENDER_AFTER_POSTPROCESS_EFFECT].clear();

	pRenderTargetManager->End_MRT();
	return S_OK;
}

HRESULT CRender_Manager::Render_AfterPostEffectGlow()
{
	Blur_Effect();
	Blur_ExtractGlow(3.f);
	ReBlur_ExtractGlow(3.f);
	
	Blend_Glow();

	Blur_ExtractBloom();
	Extract_Brightness();
	Blend_Bloom();
	Render_PostEffect();

	return S_OK;
}

HRESULT CRender_Manager::Blur_Effect()
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_MRT(TEXT("MRT_BlurEffect"));

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_OriginalEffect")))))
		DEBUG_ASSERT;

	m_pXBlurShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pXBlurShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	_float fPixelWidth = 1 / 1600.f * 1.f;
	_float fPixelHeight = 1 / 900.f * 1.f;
	m_pXBlurShader->Set_RawValue("g_PixelWidth", &fPixelWidth, sizeof(_float));
	m_pXBlurShader->Set_RawValue("g_PixelHeight", &fPixelHeight, sizeof(_float));

	_float fBlurStrength = 2.f;
	m_pXBlurShader->Set_RawValue("g_BlurStrength", &fBlurStrength, sizeof(_float));
	// Blur X
	m_pXBlurShader->Begin(0);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	pRenderTargetManager->Begin_SingleRT(TEXT("MRT_ExtractEffect"), 0);

	if (FAILED(m_pXBlurShader->Set_ShaderResourceView("g_ExtractMapTexture", pRenderTargetManager->Get_SRV(TEXT("Target_BlurForEffect")))))
		DEBUG_ASSERT;

	// Blur Y
	m_pXBlurShader->Begin(1);
	m_pVIBuffer->Render();


	pRenderTargetManager->End_MRT();


	return S_OK;
}

HRESULT CRender_Manager::PostProcessing(const _int In_iPass)
{
	//모션 블러, 색수차, 중심 블러(얘는 위에 있음) 등등 화면 전체 해야하는 블러의 경우
	Bake_OriginalRenderTexture();

	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	if (FAILED(m_pPostProcessingShader->Set_ShaderResourceView("g_OriginalRenderTexture", pRenderTargetManager->Get_SRV(TEXT("Target_CopyOriginalRender")))))
		DEBUG_ASSERT;

	if (FAILED(m_pPostProcessingShader->Set_ShaderResourceView("g_DepthTexture", pRenderTargetManager->Get_SRV(TEXT("Target_Depth")))))
		DEBUG_ASSERT;

	m_pPostProcessingShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pPostProcessingShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pPostProcessingShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	
	shared_ptr<CPipeLine> pPipeLine = GET_SINGLE(CPipeLine);

	_float4x4		ViewMatrixInv, ProjMatrixInv,ViewMatrix,ProjMatrix;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ))));

	ProjMatrix = *pPipeLine->Get_Transform_TP(CPipeLine::D3DTS_PROJ);
	ViewMatrix = *pPipeLine->Get_Transform_TP(CPipeLine::D3DTS_VIEW);


	m_pPostProcessingShader->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));
	m_pPostProcessingShader->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_pPostProcessingShader->Set_RawValue("g_CamProjMatrix", &ProjMatrix, sizeof(_float4x4));
//	m_pPostProcessingShader->Set_RawValue("g_CamViewMatrix", &ViewMatrix, sizeof(_float4x4));

	m_pPostProcessingShader->Set_RawValue("g_vCamPosition", &ViewMatrixInv.m[3],sizeof(_float4));
	m_pPostProcessingShader->Set_RawValue("g_BlurStrength", &m_fChromaticStrangth, sizeof(_float));

	_float4	vLinearVelocity, vAngularVelocity;
	XMStoreFloat4(&vLinearVelocity, pPipeLine->Get_LinearVelocity());
	XMStoreFloat4(&vAngularVelocity, pPipeLine->Get_AngularVelocity());

	/*m_pPostProcessingShader->Set_RawValue("g_vLinearVelocity", &vLinearVelocity, sizeof(_float4));
	m_pPostProcessingShader->Set_RawValue("g_vAngularVelocity", &vAngularVelocity, sizeof(_float4));*/
	m_pPostProcessingShader->Set_RawValue("g_PreCamViewMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&pPipeLine->Get_PreViewMatrix())), sizeof(_float4x4));


	m_pPostProcessingShader->Begin(In_iPass);
	m_pVIBuffer->Render();



	return S_OK;
}

HRESULT CRender_Manager::Render_Final()
{
	for (auto& pGameObject : m_RenderObjects[(_uint)RENDERGROUP::RENDER_FINAL])
	{
		if (pGameObject.lock())
			pGameObject.lock()->Render();
	}
	m_RenderObjects[(_uint)RENDERGROUP::RENDER_FINAL].clear();

	return S_OK;
}

HRESULT CRender_Manager::Bake_OriginalRenderTexture()
{
	shared_ptr<CRenderTarget_Manager> pRenderTargetManager = GET_SINGLE(CRenderTarget_Manager);

	pRenderTargetManager->Begin_MRT(TEXT("MRT_CopyOriginalRender"));

	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Set_ShaderResourceView("g_OriginalRenderTexture", GET_SINGLE(CGraphic_Device)->Get_SRV());

	/* 블룸 만들기. */
	m_pShader->Begin(7);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	return S_OK;
}

HRESULT CRender_Manager::Blur(const _float& In_PixelPitchScalar, const _tchar* In_szMRT, const _tchar* In_szTarget)
{
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
	m_pXBlurShader->Begin(0);
	m_pVIBuffer->Render();

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
	m_pXBlurShader->Begin(1);
	m_pVIBuffer->Render();

	pRenderTargetManager->End_MRT();

	return S_OK;
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
		return S_OK;
	}





	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		DEBUG_ASSERT;

	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		DEBUG_ASSERT;

	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_Deferred"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_ShadowDepth"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_ExtractEffect"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_BlurForBloom"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_BlurForGlow"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_Bloom"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_CopyOriginalRender"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_BlurEffect"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_ExtractOutLine"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_BlurOutLine"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_ViewShadow"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_StaticShadowDepth"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_Fog"), m_pShader, m_pVIBuffer);
	GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_Distortion"), m_pShader, m_pVIBuffer);
	//GET_SINGLE(CRenderTarget_Manager)->Render_Debug(TEXT("MRT_Glow"), m_pShader, m_pVIBuffer);

	/*for (auto& pComponent : m_pDebugComponents)
	{
		if (pComponent.lock())
			pComponent.lock()->Render();
	}*/

	return S_OK;
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
	m_pShader->OnDestroy();
	m_pVIBuffer->OnDestroy();

}

void CRender_Manager::Free()
{
	m_pShader.reset();
	m_pVIBuffer.reset();
}
