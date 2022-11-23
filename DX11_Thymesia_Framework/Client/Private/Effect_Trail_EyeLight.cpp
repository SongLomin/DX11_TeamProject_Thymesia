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

	m_pNoiseTextureCom = Add_Component<CTexture>();
	m_pNoiseTextureCom.lock()->Use_Texture(("UVNoise"));

	m_pMaskTextureCom = Add_Component<CTexture>();
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

HRESULT CEffect_Trail_EyeLight::Render()
{

	SetUp_ShaderResource();
	CallBack_Render();

	m_pShaderCom.lock()->Begin(0);
	m_pVIBuffer.lock()->Render();

	return S_OK;
}


void CEffect_Trail_EyeLight::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");

	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
	m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 1);

	m_pNoiseTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", 0);
	m_pMaskTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 66);

	_float4 vColor = _float4(1.f, 1.f, 1.f, 1.f);

	m_pShaderCom.lock()->Set_RawValue("g_vColor", &vColor, sizeof(_float4));

	m_pShaderCom.lock()->Set_RawValue("g_vWrapWeight", &vColor, sizeof(_float4));
	vColor = _float4(0.f, 0.f, 0.f, 0.f);
	m_pShaderCom.lock()->Set_RawValue("g_vUVNoise", &vColor, sizeof(_float4));

	_bool bFlag = true;
	m_pShaderCom.lock()->Set_RawValue("g_bBloom", &bFlag, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_bGlow", &bFlag, sizeof(_bool));
	
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