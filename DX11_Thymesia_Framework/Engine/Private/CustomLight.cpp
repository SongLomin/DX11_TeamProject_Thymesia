#include "CustomLight.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "RenderTarget_Manager.h"
#include "GameInstance.h"

GAMECLASS_C(CCustomLight)

_uint CCustomLight::g_iLightIndex = 0;

HRESULT CCustomLight::Initialize(const LIGHTDESC& LightDesc)
{
	m_LightDesc = LightDesc;

	m_LightDesc.iLightIndex = g_iLightIndex++;

	return S_OK;
}

HRESULT CCustomLight::Render(weak_ptr<CShader> pShader, weak_ptr<CVIBuffer_Rect> pVIBuffer, ID3D11DeviceContext* pDeviceContext)
{
	if (!m_LightDesc.bEnable)
		return E_FAIL;


	_uint			iPassIndex = 0;
	

	//pShader.lock()->Set_ShaderResourceView("g_ShadeTexture", GET_SINGLE(CRenderTarget_Manager)->Get_SRV(TEXT("Target_Shade")));


	/*ºû Á¤º¸¸¦ ¼ÎÀÌ´õ¿¡ ´øÁø´Ù. */
	if (LIGHTDESC::TYPE_DIRECTIONAL == m_LightDesc.eActorType)
	{
		iPassIndex = 1;

		if (FAILED(pShader.lock()->Set_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
			DEBUG_ASSERT;
	}

	else if(LIGHTDESC::TYPE_POINT == m_LightDesc.eActorType)
	{
		_bool IsInFrustum = GAMEINSTANCE->isIn_Frustum_InWorldSpace(XMLoadFloat4(&m_LightDesc.vPosition), m_LightDesc.fRange * 5.f);

		if (!IsInFrustum)
			return E_FAIL;
		
		iPassIndex = 2;

		if (FAILED(pShader.lock()->Set_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			DEBUG_ASSERT;

		if (FAILED(pShader.lock()->Set_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float))))
			DEBUG_ASSERT;
	}

	else if (LIGHTDESC::TYPE_SPOTLIGHT == m_LightDesc.eActorType)
	{
		///*_bool IsInFrustum = GAMEINSTANCE->isIn_Frustum_InWorldSpace(XMLoadFloat4(&m_LightDesc.vPosition), m_LightDesc.fRange);

		//if (!IsInFrustum)
		//	return E_FAIL;*/

		iPassIndex = 9;
		
		if (FAILED(pShader.lock()->Set_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
			DEBUG_ASSERT;

		if (FAILED(pShader.lock()->Set_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			DEBUG_ASSERT;

		if (FAILED(pShader.lock()->Set_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float))))
			DEBUG_ASSERT;

		if (FAILED(pShader.lock()->Set_RawValue("g_fCutOff", &m_LightDesc.fCutOff, sizeof(_float))))
			DEBUG_ASSERT;
		if (FAILED(pShader.lock()->Set_RawValue("g_fOuterCutOff", &m_LightDesc.fOuterCutOff, sizeof(_float))))
			DEBUG_ASSERT;

	}

	if (FAILED(pShader.lock()->Set_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		DEBUG_ASSERT;

	if (FAILED(pShader.lock()->Set_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		DEBUG_ASSERT;

	if (FAILED(pShader.lock()->Set_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		DEBUG_ASSERT;

	if (FAILED(pShader.lock()->Set_RawValue("g_fLightIntensity", &m_LightDesc.fIntensity, sizeof(_float))))
		DEBUG_ASSERT;


	pShader.lock()->Begin(iPassIndex, pDeviceContext);

	/* ¼ÎÀÌµå Å¸°Ù¿¡ ±×¸°´Ù.  */
	pVIBuffer.lock()->Render(pDeviceContext);

	return S_OK;
}

shared_ptr<CCustomLight> CCustomLight::Create(const LIGHTDESC& LightDesc)
{
	shared_ptr<CCustomLight> pInstance = make_shared<CCustomLight>();

	pInstance->m_this = pInstance;

	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		DEBUG_ASSERT;
	}

	return pInstance;
}

void CCustomLight::Free()
{
}
