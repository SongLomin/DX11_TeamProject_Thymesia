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


	m_tUIDesc.fDepth = 0.f;
	m_tBackUpDesc = m_tUIDesc;

	ZeroMemory(&m_tHoverDesc, sizeof(HUDHOVERDESC));
	
	m_eHoverType = HUD_HOVER_ANIMATION_END;

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
	
	if (m_tHoverDesc.m_bSizeChange)
	{
		if (m_eHoverType == HUD_HOVER_ANIMATION_FROM_ALPHA)
		{

			FaderDesc tFaderDesc = m_pFaderCom.lock()->Get_FaderDesc();
			m_tUIDesc.fSizeX = m_tBackUpDesc.fSizeX +
				((tFaderDesc.fFadeMaxTime - m_pFaderCom.lock()->Get_FadeTime()) / tFaderDesc.fFadeMaxTime) * (m_tBackUpDesc.fSizeX * m_tHoverDesc.m_fSizeMag);

			m_tUIDesc.fSizeY = m_tBackUpDesc.fSizeY +
				((tFaderDesc.fFadeMaxTime - m_pFaderCom.lock()->Get_FadeTime()) / tFaderDesc.fFadeMaxTime) * (m_tBackUpDesc.fSizeY * m_tHoverDesc.m_fSizeMag);
		}
		else if (m_eHoverType == HUD_HOVER_ANIMATION_JUSTADD)
		{
			FaderDesc tFaderDesc = m_pFaderCom.lock()->Get_FaderDesc();
			
			/*
			
			*/
			_float	fAmountX = (m_tBackUpDesc.fSizeX * m_tHoverDesc.m_fSizeMag) * (fTimeDelta / tFaderDesc.fFadeMaxTime);
			_float	fAmountY = (m_tBackUpDesc.fSizeY * m_tHoverDesc.m_fSizeMag) * (fTimeDelta / tFaderDesc.fFadeMaxTime);

			m_tUIDesc.fSizeX += fAmountX;
			m_tUIDesc.fSizeY += fAmountY;
		}
	}
	
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


void CHUD_Hover::Init_Fader(const Engine::FaderDesc& _tFaderDesc, const HUDHOVERDESC& _tHoverDesc, HUD_HOVER_ANIMAITON_TYPE eHoverType)
{
	m_pFaderCom.lock()->Init_Fader(_tFaderDesc);
	m_tHoverDesc = _tHoverDesc;
	m_eHoverType = eHoverType;
	Set_Enable(true);
}

void CHUD_Hover::Set_UIDesc(UI_DESC _tUIDesc)
{
	m_tUIDesc = _tUIDesc;
	m_tBackUpDesc = _tUIDesc;
}

void CHUD_Hover::Call_FadeEnd(FADER_TYPE In_eFaderType)
{
	//대리자에 뭐 등록된건 없는데 일단 넣어놓았음.

	CallBack_FadeEnd(In_eFaderType);
	//CallBack_FadeEnd.Clear();

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
