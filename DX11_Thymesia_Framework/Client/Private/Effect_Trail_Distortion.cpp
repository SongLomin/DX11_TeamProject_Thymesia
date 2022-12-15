#include "stdafx.h"
#include "Effect_Trail_Distortion.h"
#include "VIBuffer_Trail.h"
#include "Client_Components.h"

GAMECLASS_C(CEffect_Trail_Distortion)
CLONE_C(CEffect_Trail_Distortion, CGameObject)

HRESULT CEffect_Trail_Distortion::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_eRenderGroup = RENDERGROUP::RENDER_DISTORTION;

	return S_OK;
}

HRESULT CEffect_Trail_Distortion::Initialize(void* pArg)//trail을 사용하는 객체로부터 traildesc를 받는다.
{
	__super::Initialize(pArg);
	m_pNoiseTextureCom.lock()->Use_Texture(("UVNoise"));

	m_pMaskTextureCom.lock()->Use_Texture("UVMask");
	return S_OK;
}

void CEffect_Trail_Distortion::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CEffect_Trail_Distortion::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CEffect_Trail_Distortion::Render(ID3D11DeviceContext* pDeviceContext)
{

	SetUp_ShaderResource();
	CallBack_Render();

	m_pShaderCom.lock()->Begin(1, pDeviceContext);
	m_pVIBuffer.lock()->Render(pDeviceContext);

	return S_OK;
}


void CEffect_Trail_Distortion::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	_float4 vColor = _float4(1.f, 1.f, 1.f, 1.f);

	m_pShaderCom.lock()->Set_RawValue("g_vColor", &vColor, sizeof(_float4));

	m_pShaderCom.lock()->Set_RawValue("g_vWrapWeight", &vColor, sizeof(_float4));
	vColor = _float4(0.f, 0.f, 0.f, 0.f);
	m_pShaderCom.lock()->Set_RawValue("g_vUVNoise", &vColor, sizeof(_float4));

}



void CEffect_Trail_Distortion::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

}

void CEffect_Trail_Distortion::OnDisable()
{
	__super::OnDisable();
}


void CEffect_Trail_Distortion::Free()
{
}