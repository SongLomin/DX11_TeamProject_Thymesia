#pragma once
#include "ImGui_Window.h"

BEGIN(Client)
class CWindow_Shader_Dev :
	public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_Shader_Dev)

private:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

private: /* For. Shader Log */
	list<pair<_bool, string>>	m_szCompileMessage;

private: /* For. LiftGammaGain */
	_float4 m_vLiftColor = {};
	_float4 m_vGammaColor = {};
	_float4 m_vGainColor = {};

private: /* For. Fog */
	_float4 m_vFogColor = {};
	_float	m_fFogRange = 0.f;

private: /* For. ShadowLight */
	_float3 m_vEye = {};
	_float3 m_vLookAt = {};

private:/* For. Screen Tone*/
	_float m_fContrast = 1.f;
	_float m_fSaturation = 1.f;
	_float m_fExposure = 1.f;

private:/* For. Irradiance Color Scale */
	_float3 m_vIrradianceColorScale{ 1.f,1.f,1.f };

private:/* For. Sharpness */
	_float	m_fSharpness{ 0.f };

private:
	virtual void OnEventMessage(_uint iArg) override;

private:
    void Free();

};

END