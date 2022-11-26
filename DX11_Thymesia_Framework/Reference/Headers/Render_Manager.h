#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CShader;
class CVIBuffer_Rect;
class CComponent;

class CRender_Manager :
    public CBase
{
	DECLARE_SINGLETON(CRender_Manager)

public:
	HRESULT Initialize();

	HRESULT Add_RenderGroup(RENDERGROUP	eGroup, weak_ptr<CGameObject> pGameObject);
	HRESULT Draw_RenderGroup();

public:
	HRESULT Set_MotionBlur(const _float& In_fBlurScale);
	HRESULT Add_MotionBlur(const _float& In_fBlurScale);
	HRESULT Set_Chromatic(const _float In_fChormaticStrangth);
	HRESULT Add_Chromatic(const _float In_fChormaticStrangth);

	HRESULT	Set_ShadowLight(_fvector In_vEye, _fvector In_vLookAt);
	

private:
	HRESULT Render_Priority();
	HRESULT Render_ShadowDepth();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_Lights();
	HRESULT Bake_Fog();
	HRESULT Bake_ViewShadow();
	HRESULT Render_Blend(); /* Diffuse * Shade 백버퍼에 그린다. */
	HRESULT Render_NonLight();
	HRESULT Render_AlphaBlend();
	HRESULT Extract_OutLine();
	HRESULT Blur_OutLine();
	HRESULT Blur_ExtractGlow(const _float& In_PixelPitchScalar);
	HRESULT ReBlur_ExtractGlow(const _float& In_PixelPitchScalar);
	HRESULT Blend_OutLine();
	HRESULT Extract_Distortion();
	HRESULT Blend_Distortion();
	HRESULT Blend_Glow(); // 원본이 글로우보다 위에 그려진다.
	HRESULT Render_Font();
	HRESULT Extract_Brightness();
	HRESULT Blur_ExtractBloom();
	HRESULT Blend_Bloom();
	HRESULT Render_PostEffect();
	HRESULT Render_AfterPostEffect();
	HRESULT Render_AfterPostEffectGlow();
	HRESULT Blur_Effect();

	HRESULT PostProcessing();
	HRESULT Render_UI();
	HRESULT Render_Final();

	HRESULT Bake_OriginalRenderTexture();

	HRESULT Blur(const _float& In_PixelPitchScalar, const _tchar* In_szMRT, const _tchar* In_szTarget);

#ifdef _DEBUG
public:
	HRESULT Render_Debug();
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

private:
	list<weak_ptr<CGameObject>>				m_RenderObjects[(_uint)RENDERGROUP::RENDER_END];
	typedef list<weak_ptr<CGameObject>>		RENDEROBJECTS;

private:
	_float3		m_vShadowLightEye{ 0.f, 1.f, 0.f };
	_float3		m_vShadowLightLookAt{ 0.f, 0.f, 0.f };
	_float4x4	m_LightViewMatrixTranspose;
	_float4x4	m_LightProjMatrixTranspose;

private:
	_float		m_fBlurWitdh = 0.f;
	_float		m_fChromaticStrengthAcc = 0.f;
	_float		m_fChromaticStrangth = 0.f;

	_bool		m_bFirst = true;

public:
	virtual void OnDestroy() override;

protected:
	void Free();


};

END

