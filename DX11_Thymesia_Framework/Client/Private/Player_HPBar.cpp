#include "stdafx.h"
#include "Player_HPBar.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "Player_ProgressBar.h"
#include "CustomUI.h"
#include "HUD_Hover.h"
#include "Fader.h"

GAMECLASS_C(CPlayer_HPBar)
CLONE_C(CPlayer_HPBar, CGameObject);

HRESULT CPlayer_HPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_HPBar::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	Set_UIPosition(300.f, 750.f, 400.f , 15.f);

	m_pBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_BG");
	m_pBG.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, m_tUIDesc.fSizeX, m_tUIDesc.fSizeY);
	m_pBG.lock()->Set_Depth(0.2f);


	m_pMainBar = GAMEINSTANCE->Add_GameObject<CPlayer_ProgressBar>(LEVEL_STATIC);
	m_pMainBar.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_MainBar");
	m_pMainBar.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, m_tUIDesc.fSizeX, m_tUIDesc.fSizeY - 5.f);
	m_pMainBar.lock()->Set_Depth(0.1f);

	m_pBorderLeft = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pBorderLeft.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Border_Left");
	m_pBorderLeft.lock()->Set_UIPosition(m_tUIDesc.fX - (m_tUIDesc.fSizeX * 0.5f), m_tUIDesc.fY, 26.f, 15.f );
	
	m_pBorderRight = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pBorderRight.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Border_Right");
	m_pBorderRight.lock()->Set_UIPosition(m_tUIDesc.fX + (m_tUIDesc.fSizeX * 0.5f), m_tUIDesc.fY, 26.f, 15.f);


	Engine::FaderDesc tFaderDesc;
	
	UI_DESC	tTrackDesc;
	tTrackDesc.fX = m_tUIDesc.fX + (m_tUIDesc.fSizeX * 0.5f);
	tTrackDesc.fY = m_tUIDesc.fY;
	tTrackDesc.fSizeX = 19.f;
	tTrackDesc.fSizeY = 45.f;
	tTrackDesc.fDepth = 0.f;

	tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	tFaderDesc.eFaderType = FADER_TYPE::FADER_INOUTLOOPING;
	tFaderDesc.fDelayTime = 0.f;
	tFaderDesc.fFadeMaxTime = 1.f;
	tFaderDesc.vFadeColor = _float4(0, 0, 0, 1.f);

	CHUD_Hover::HUDHOVERDESC tHoverDesc;
	ZeroMemory(&tHoverDesc, sizeof(CHUD_Hover::HUDHOVERDESC));

	tHoverDesc.m_bSizeChange = false;

	m_pTrack = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &tTrackDesc);
	m_pTrack.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Track");
	m_pTrack.lock()->Init_Fader(tFaderDesc, tHoverDesc, CHUD_Hover::HUD_HOVER_ANIMATION_FROM_ALPHA);


	m_fLerpHp = 300.f;
	m_fCurrentHp = 300.f;
	m_fMaxHp = 300.f;

	m_tTextInfo.bAlways = false;
	m_tTextInfo.bCenterAlign = false;
	m_tTextInfo.fRotation = 0.f;
	m_tTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
	m_tTextInfo.vScale = _float2(0.5, 0.5f);
	m_tTextInfo.vPosition = _float2(m_tUIDesc.fX + m_tUIDesc.fSizeX * 0.5f + 20.f, m_tUIDesc.fY - 10.f);


	m_tUIDesc.fDepth = 0.f;
	m_fLerpAcc = 1.f;
	m_eRenderGroup = RENDERGROUP::RENDER_UI;
	GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Cast<CGameObject>(m_this));

	return S_OK;
}

HRESULT CPlayer_HPBar::Start()
{
	__super::Start();

	return S_OK;
}

void CPlayer_HPBar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#ifdef _DEBUG
	if (KEY_INPUT(KEY::J, KEY_STATE::TAP))
	{
		m_fCurrentHp -= 30.f;

	}
	if (KEY_INPUT(KEY::K, KEY_STATE::TAP))
	{

		m_fCurrentHp += 30.f;
		m_fLerpHp = m_fCurrentHp;
		m_fLerpAcc = 1.f;
	}
	if (KEY_INPUT(KEY::Q, KEY_STATE::TAP))
	{
		m_fCurrentHp -= 200.f;

	}
	//if (KEY_INPUT(KEY::W, KEY_STATE::TAP))
	//{
	//
	//	m_fCurrentHp += 200.f;
	//	m_fLerpHp = m_fCurrentHp;
	//	m_fLerpAcc = 1.f;
	//}
#endif // _DEBUG

	if (m_fCurrentHp < 0.f)
		m_fCurrentHp = 0.f;
	else if (m_fCurrentHp > m_fMaxHp)
		m_fCurrentHp = m_fMaxHp;

	if (m_fCurrentHp != m_fLerpHp)
	{
		_float fRight = max(m_fCurrentHp, m_fLerpHp);
		_float fLeft = min(m_fCurrentHp, m_fLerpHp);
		_float fLerp;

		fLerp = SMath::Lerp(fLeft, fRight, fTimeDelta * m_fLerpAcc);
		
		if (fabs(m_fCurrentHp - m_fLerpHp) > 1.f)
		{ 
			m_fLerpHp -= (fLerp - fLeft);//·¯ÇÁ Â÷ÀÌ°ª¸¸Å­ »©ÁÜ
			m_fLerpAcc += 0.4f;
		}
		else
		{
			m_fLerpHp = m_fCurrentHp;
			m_fLerpAcc = 1.f;
		}
	}
	_float fRatio = m_fLerpHp / m_fMaxHp;

	if (fRatio < 1.f)
	{
		m_pTrack.lock()->Set_UIPosition((m_tUIDesc.fX - (m_tUIDesc.fSizeX * 0.5f)) + (m_tUIDesc.fSizeX * fRatio), m_tUIDesc.fY);
		m_pTrack.lock()->Set_Enable(true);
	}
	else
		m_pTrack.lock()->Set_Enable(false);

	m_pMainBar.lock()->Set_Ratio(fRatio);

	m_pTrack.lock()->Get_UIDESC();



	m_tTextInfo.szText = to_wstring((_uint)m_fLerpHp);
	m_tTextInfo.szText.append(L"/");
	m_tTextInfo.szText.append(to_wstring((_uint)m_fMaxHp));
}

void CPlayer_HPBar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_tTextInfo);

}

HRESULT CPlayer_HPBar::Render()
{
	//__super::Render();

	return S_OK;;
}

void CPlayer_HPBar::OnEventMessage(_uint iArg)
{
}

void CPlayer_HPBar::Set_CurrentHp(_float _fCurrentHp)
{
	if (_fCurrentHp > m_fCurrentHp)
	{
		m_fLerpHp = _fCurrentHp;
		m_fLerpAcc = 1.f;
	}
	m_fCurrentHp = _fCurrentHp;
}


void CPlayer_HPBar::Free()
{
	__super::Free();

}
