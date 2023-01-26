#include "stdafx.h"
#include "UI_BloodOverlay.h"
#include "EasingComponent_Alpha.h"
#include "Shader.h"
#include "UI_Utils.h"
#include "Texture.h"

GAMECLASS_C(CUI_BloodOverlay)
CLONE_C(CUI_BloodOverlay, CGameObject)

HRESULT CUI_BloodOverlay::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	
	return S_OK;
}

HRESULT CUI_BloodOverlay::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	//Set_UIPosition(0.f, 0.f, 1600.f, 900.f, ALIGN_CENTER);
	RENDERGROUP::RENDER_AFTER_UI;

	Set_Texture("BloodOverlay");
	Set_Depth(0.6f);
	Set_Enable(false);
	Set_PassIndex(14);

	SetUp_Component();



	m_pMaskTexture.lock()->Use_Texture("BloodOverlay_Mask");

	return S_OK;
}

void CUI_BloodOverlay::Call_Overlay(_float fLifeTime)
{
	Set_Enable(true);

	m_pEasingAlpha.lock()->Set_Lerp(1.f, 0.f, fLifeTime,
		EASING_TYPE::QUAD_OUT, CEasingComponent::ONCE, false);

	m_pEasingAlpha.lock()->Callback_LerpEnd +=
		bind(&CUI_BloodOverlay::Call_LerpEnd, this);
}

HRESULT CUI_BloodOverlay::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	m_pMaskTexture.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 0);

	return S_OK;
}

void CUI_BloodOverlay::SetUp_Component()
{
	m_pEasingAlpha = Add_Component<CEasingComponent_Alpha>();
	m_pMaskTexture = Add_Component<CTexture>();

}

void CUI_BloodOverlay::Call_LerpEnd()
{
	Set_Enable(false);
}
