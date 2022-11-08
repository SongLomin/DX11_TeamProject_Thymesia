#include "stdafx.h"
#include "HUD_Hover.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "Fader.h"
#include "Engine_Defines.h"

GAMECLASS_C(CHUD_Hover)
CLONE_C(CHUD_Hover, CGameObject)

HRESULT CHUD_Hover::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CHUD_Hover::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_vFadeColor = _float4(0.f, 0.f, 0.f, 0.7f);
	m_pFaderCom.lock()->CallBack_FadeEnd += bind(&CHUD_Hover::Call_FadeEnd, this, placeholders::_1);
	m_iPassIndex = 5;

	m_tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	m_tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
	m_tFaderDesc.fDelayTime = 0.f;
	m_tFaderDesc.fFadeMaxTime = 1.f;
	m_tFaderDesc.vFadeColor = m_vFadeColor;

	Set_Enable(false);
	return S_OK;
}

HRESULT CHUD_Hover::Start()
{
	__super::Start();

	return S_OK;
}

void CHUD_Hover::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_tUIDesc.fSizeX = m_tBackUpDesc.fSizeX +
		(m_pFaderCom.lock()->Get_FadeTime() / m_tFaderDesc.fFadeMaxTime) * (m_tBackUpDesc.fSizeX * 0.2f);

	m_tUIDesc.fSizeY = m_tBackUpDesc.fSizeY +
		(m_pFaderCom.lock()->Get_FadeTime() / m_tFaderDesc.fFadeMaxTime) * (m_tBackUpDesc.fSizeY * 0.2f);

}

void CHUD_Hover::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CHUD_Hover::Render()
{
	__super::Render();


	return S_OK;
}

void CHUD_Hover::Init_Fade()
{
	m_pFaderCom.lock()->Init_Fader(m_tFaderDesc);
	m_tBackUpDesc = m_tUIDesc;

	Set_Enable(true);
}

void CHUD_Hover::Call_FadeEnd(FADER_TYPE In_eFaderType)
{
	//대리자에 뭐 등록된건 없는데 일단 넣어놓았음.

	CallBack_FadeEnd();
	CallBack_FadeEnd.Clear();
	Set_Enable(false);

	m_tUIDesc = m_tBackUpDesc;
}


HRESULT CHUD_Hover::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vColor", &m_pFaderCom.lock()->Get_FadeColor(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

void CHUD_Hover::Free()
{
	__super::Free();
}
