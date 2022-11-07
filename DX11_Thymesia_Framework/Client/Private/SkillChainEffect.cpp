#include "stdafx.h"
#include "SkillChainEffect.h"
#include "Texture.h"
#include "Shader.h"
#include "Fader.h"

GAMECLASS_C(CSkillChainEffect)
CLONE_C(CSkillChainEffect, CGameObject)

HRESULT CSkillChainEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkillChainEffect::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxTex"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);

	
	//m_pTextureCom.lock()->Use_Texture(TEXT("Default"));

	Set_UIPosition(g_iWinCX / 2.f, g_iWinCY / 2.f, 80.f, 80.f);

	
	m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

	m_iPassIndex = 0;

	

	return S_OK;
}

HRESULT CSkillChainEffect::Start()
{
	__super::Start();
	return S_OK;
}

void CSkillChainEffect::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkillChainEffect::LateTick(_float fTimeDelta)
{
	if (m_fDelayTime > 0.f)
	{
		m_fDelayTime -= fTimeDelta;
		return;
	}

	__super::LateTick(fTimeDelta);

	if (!m_bEndDelay)
	{
		CFader::FaderDesc FaderDesc;

		FaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		FaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		FaderDesc.vFadeColor = { 0.f, 0.f, 0.f, 1.f };
		FaderDesc.fDelayTime = 0.f;
		FaderDesc.fFadeMaxTime = 0.36f;


		m_pFaderCom.lock()->Init_Fader(FaderDesc);
		m_pFaderCom.lock()->CallBack_FadeEnd += bind(&CSkillChainEffect::Set_Dead, this);

		m_bEndDelay = true;
	}

	if (EFFECT_TYPE::DROP == m_eEffectType)
	{
		if (m_pParent.lock())
		{
			m_tUIDesc = m_pParent.lock()->Get_UIDESC();
			m_tUIDesc.fSizeX *= 1.2f;
			m_tUIDesc.fSizeY *= 1.2f;
		}
	}

	else if(EFFECT_TYPE::USE == m_eEffectType)
	{
		m_tUIDesc.fSizeX += m_tUIDesc.fSizeX * fTimeDelta * 2.3f;
		m_tUIDesc.fSizeY += m_tUIDesc.fSizeY * fTimeDelta * 2.3f;
		
	}

}

HRESULT CSkillChainEffect::Render()
{
	__super::Render();

	return S_OK;
}

void CSkillChainEffect::Init_DropEffect(weak_ptr<CUI> pParent)
{
	m_pParent = pParent;
	m_eEffectType = EFFECT_TYPE::DROP;

	m_tUIDesc = m_pParent.lock()->Get_UIDESC();

	m_pTextureCom.lock()->Use_Texture(("Icon"));
	m_iTextureIndex = 3;

	CFader::FaderDesc FaderDesc;

	FaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
	FaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	FaderDesc.vFadeColor = { 0.f, 0.f, 0.f, 1.f };
	FaderDesc.fDelayTime = 0.f;
	FaderDesc.fFadeMaxTime = 0.8f;


	m_pFaderCom.lock()->Init_Fader(FaderDesc);
	m_pFaderCom.lock()->CallBack_FadeEnd += bind(&CSkillChainEffect::Set_Dead, this);

}

void CSkillChainEffect::Init_UseEffect(const UI_DESC& In_UIDesc, const _float& fDelayTime)
{
	m_tUIDesc = In_UIDesc;
	m_eEffectType = EFFECT_TYPE::USE;
	m_fDelayTime = fDelayTime;

	m_pTextureCom.lock()->Use_Texture(("Icon"));
	m_iTextureIndex = 2;
}

HRESULT CSkillChainEffect::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	//_float FaderAhlpaScale = 1.f;
	_float FaderAhlpaScale = m_pFaderCom.lock()->Get_FadeColor().w;
	m_pShaderCom.lock()->Set_RawValue("g_fAhlpaScale", &FaderAhlpaScale, sizeof(_float));

	return S_OK;
}

void CSkillChainEffect::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("SkillBall GameObject"), ImGuiTreeNodeFlags_DefaultOpen)
		{

		}
	}

}

void CSkillChainEffect::Free()
{
}
