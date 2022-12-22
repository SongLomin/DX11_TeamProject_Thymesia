#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CShader;
class CVIBuffer_Rect;
class CComponent;
class CTexture;

class CRender_Manager :
    public CBase
{
	DECLARE_SINGLETON(CRender_Manager)

public:
	enum DEFERRED_GROUP
	{
		DEFERRED_EFFECT,
		DEFERRED_UI,
		DEFERRED_END
	};


public:
	ID3D11DeviceContext* Get_BeforeRenderContext();
	void Release_BeforeRenderContext(ID3D11DeviceContext* pDeviceContext);
	
private:
	void Execute_BeforeRenderCommandList();

public:
	HRESULT Initialize();

	HRESULT Add_RenderGroup(RENDERGROUP	eGroup, weak_ptr<CGameObject> pGameObject);
	HRESULT Draw_RenderGroup();

public:
	HRESULT Set_MotionBlur(const _float In_fBlurScale);
	HRESULT Add_MotionBlur(const _float In_fBlurScale);
	HRESULT Set_Chromatic(const _float In_fChormaticStrangth);
	HRESULT Add_Chromatic(const _float In_fChormaticStrangth);
	HRESULT Set_RadialBlur(const _float In_fRadialBlurStength, _float3 In_vBlurWorldPosition);
	HRESULT Add_RadialBlur(const _float In_fRadialBlurStrength);
	HRESULT Set_FogDesc(const _float4 In_vFogColor,const _float In_fFogRange);
	_float4 Get_FogColor()
	{
		return m_vFogColor;
	}
	_float Get_FogRange()
	{
		return m_fFogRange;
	}

	HRESULT Set_LiftGammaGain(const _float4 In_vLift, const _float4 In_vGamma, const _float4 In_vGain);
	HRESULT Set_GrayScale(const _float In_fGrayScale);

	HRESULT Set_Exposure(const _float In_fExposure);

	HRESULT	Set_ShadowLight(_fvector In_vEye, _fvector In_vLookAt);
	HRESULT Set_DynamicShadowLight(_fvector In_vEye, _fvector In_vLookAt);
	LIFTGAMMAGAIN_DESC& Get_LiftGammaGain()
	{
		return m_LiftGammaGainDesc;
	}

	HRESULT Set_Contrast(const _float In_fContrast);
	HRESULT Set_Saturation(const _float In_fSaturation);
	HRESULT Set_IrradianceMap(const _char* In_szIrradianceMap);
	HRESULT Set_PreFilteredMap(const _char* In_szPreFiltered);

private:
	HRESULT Render_Priority();
	HRESULT Render_ShadowDepth();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_Lights();
	HRESULT Bake_Fog();
	HRESULT Bake_ViewShadow();
	HRESULT Render_Blend(); /* Diffuse * Shade 백버퍼에 그린다. */
	HRESULT Render_Effect();
	HRESULT Render_NonLight();
	HRESULT Render_NonAlphaEffect(ID3D11DeviceContext* pDeviceContext = nullptr);
	HRESULT Render_AlphaBlend(ID3D11DeviceContext* pDeviceContext = nullptr);
	HRESULT Extract_OutLine();
	HRESULT Blur_OutLine();
	HRESULT Blur_ExtractGlow(const _float& In_PixelPitchScalar, ID3D11DeviceContext* pDeviceContext = nullptr);
	HRESULT ReBlur_ExtractGlow(const _float& In_PixelPitchScalar, ID3D11DeviceContext* pDeviceContext = nullptr);
	HRESULT Blend_OutLine();
	HRESULT Extract_Distortion();
	HRESULT Blend_Distortion();
	HRESULT Blend_Glow(ID3D11DeviceContext* pDeviceContext = nullptr); // 원본이 글로우보다 위에 그려진다.
	HRESULT Render_Font();
	HRESULT Extract_Brightness();
	HRESULT Blur_ExtractBloom();
	HRESULT ReBlur_ExtractBloom();
	HRESULT Blend_Bloom();
	HRESULT Render_SSR();

	HRESULT PostProcessing();
	HRESULT AntiAliasing();
	HRESULT Render_UI();
	HRESULT Render_Final();

	HRESULT Bake_OriginalRenderTexture();

	HRESULT Blur(const _float& In_PixelPitchScalar, const _tchar* In_szMRT, const _tchar* In_szTarget);

private:
	void Emplace_SleepContext(const _uint In_iIndex);

#ifdef _DEBUG
public:
	HRESULT Render_Debug();
	HRESULT Set_DebugSize(const _float2 vSize);
	HRESULT Set_OldSchoolView(const _bool bOldSchool);
	HRESULT	Add_DebugSRT(const _tchar* In_szMRTName);

private:
	void Render_DebugSRT(const _uint In_iIndex);

private:
	vector<tstring> m_szDebugRenderMRTNames;
	_float2			m_vDebugSize;
	_bool			m_bOldSchoolView = true;
#endif // _DEBUG

#ifdef _DEBUG
	/* 직교투영을 위한 정보이다. */


private:
	list<weak_ptr<CComponent>>	m_pDebugComponents;
#endif // _DEBUG

private:
	_float4x4					m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;
	shared_ptr<CShader>			m_pShader;
	shared_ptr<CShader>			m_pXBlurShader;
	shared_ptr<CShader>			m_pOutLineShader;
	shared_ptr<CShader>			m_pDistortionShader;
	shared_ptr<CShader>			m_pPostProcessingShader;
	shared_ptr<CVIBuffer_Rect>	m_pVIBuffer;

	shared_ptr<CTexture>		m_pIrradianceTextureCom;
	shared_ptr<CTexture>		m_pPreFilterTextureCom;
	shared_ptr<CTexture>		m_pBRDFLUTTextureCom;


private:
	list<weak_ptr<CGameObject>>				m_RenderObjects[(_uint)RENDERGROUP::RENDER_END];
	typedef list<weak_ptr<CGameObject>>		RENDEROBJECTS;

private:
	_float3		m_vShadowLightEye{ 0.f, 1.f, 0.f };
	_float3		m_vShadowLightLookAt{ 0.f, 0.f, 0.f };
	_float4x4	m_LightViewMatrixTranspose;
	_float4x4	m_LightProjMatrixTranspose;

	_float3		m_vDynamicShadowLightEye{ 0.f, 1.f, 0.f };
	_float3		m_vDynamicShadowLightLookAt{ 0.f, 0.f, 0.f };
	_float4x4	m_DynamicLightViewMatrixTranspose;
	_float4x4	m_DynamicLightProjMatrixTranspose;

	_float4x4	m_AntiAliasingWorldMatrix;
	_float4x4	m_AntiAliasingProjMatrixTranspose;

private:
	_float		m_fBlurWitdh = 0.f;

	_float		m_fMotionBlurStrength = 0.f;
	_float		m_fMotionBlurStrengthAcc = 0.f;

	_float		m_fChromaticStrengthAcc = 0.f;
	_float		m_fChromaticStrangth = 0.f;

	_float		m_fRadialBlurStrength = 0.f;
	_float		m_fRadialBlurStrengthAcc = 0.f;
	_float3		m_vRadialBlurWorldPos = { 0.f,0.f,0.f };
	_bool		m_bFirst = true;

	LIFTGAMMAGAIN_DESC m_LiftGammaGainDesc;

	_float4		m_vFogColor = { 0.8f, 0.8f, 0.8f, 1.f };
	_float		m_fFogRange = 30.f;
	
	_float		m_fGrayScale = 1.f;
	_float		m_fContrastValue = 1.f;
	_float		m_fSaturation = 1.f;

	_float		m_fExposure = 1.f;


private:
	ComPtr<ID3D11DeviceContext> m_pDeferredContext[DEFERRED_END];
	vector<ID3D11DeviceContext*> m_pBeforeRenderContexts;
	vector<ID3D11DeviceContext*> m_pBeforeRenderSleepContexts;

	std::mutex m_job_q_;

public:
	virtual void OnDestroy() override;
	virtual void OnEngineEventMessage(const ENGINE_EVENT_TYPE In_eEngineEvent) override;

protected:
	void Free();


};

END

