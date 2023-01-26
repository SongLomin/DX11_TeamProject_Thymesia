#include "stdafx.h"
#include "Effect_Trail_EyeLight.h"
#include "VIBuffer_Trail.h"
#include "Client_Components.h"

GAMECLASS_C(CEffect_Trail_EyeLight)
CLONE_C(CEffect_Trail_EyeLight, CGameObject)

HRESULT CEffect_Trail_EyeLight::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_eRenderGroup = RENDERGROUP::RENDER_ALPHABLEND;

	return S_OK;
}

HRESULT CEffect_Trail_EyeLight::Initialize(void* pArg)//trail을 사용하는 객체로부터 traildesc를 받는다.
{
	__super::Initialize(pArg);

	m_pTextureCom.lock()->Use_Texture("UVColorDiffuse");

	m_pNoiseTextureCom.lock()->Use_Texture(("UVNoise"));

	m_pMaskTextureCom.lock()->Use_Texture("UVMask");
	return S_OK;
}

void CEffect_Trail_EyeLight::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CEffect_Trail_EyeLight::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CEffect_Trail_EyeLight::Render(ID3D11DeviceContext* pDeviceContext)
{

	SetUp_ShaderResource();
	CallBack_Render();

	m_pShaderCom.lock()->Begin(3, pDeviceContext);
	m_pVIBuffer.lock()->Render(pDeviceContext);

	return S_OK;
}


void CEffect_Trail_EyeLight::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();


	_float4 vColor = _float4(1.f, 1.f, 1.f, 1.f);

	m_pShaderCom.lock()->Set_RawValue("g_vColor", &vColor, sizeof(_float4));

	m_pShaderCom.lock()->Set_RawValue("g_vWrapWeight", &vColor, sizeof(_float4));
	vColor = _float4(0.f, 0.f, 0.f, 0.f);
	m_pShaderCom.lock()->Set_RawValue("g_vUVNoise", &vColor, sizeof(_float4));

	_bool bBloomFlag = false;
	m_pShaderCom.lock()->Set_RawValue("g_bBloom", &bBloomFlag, sizeof(_bool));

	_bool bGlow = false;
	m_pShaderCom.lock()->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool));
	
	vColor = _float4(1.f, 0.f, 0.f, 1.f);
	m_pShaderCom.lock()->Set_RawValue("g_vGlowColor", &vColor, sizeof(_float4));


}



void CEffect_Trail_EyeLight::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

}

void CEffect_Trail_EyeLight::OnDisable()
{
	__super::OnDisable();
}


void CEffect_Trail_EyeLight::Free()
{
}