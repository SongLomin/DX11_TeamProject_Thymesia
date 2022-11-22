#include "stdafx.h"
#include "Player_MPBar.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "ProgressBar.h"
#include "UI_LerpBar.h"



GAMECLASS_C(CPlayer_MPBar)
CLONE_C(CPlayer_MPBar, CGameObject);

HRESULT CPlayer_MPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_MPBar::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	Set_UIPosition(200.f, 830.f, 300.f, 8.f);

	m_pBG = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
	m_pBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_BG");
	m_pBG.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, m_tUIDesc.fSizeX, m_tUIDesc.fSizeY);

	m_pMainBar = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
	m_pMainBar.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_MainBar");
	m_pMainBar.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, m_tUIDesc.fSizeX, m_tUIDesc.fSizeY - 5.f);

	m_pBorderLeft = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
	m_pBorderLeft.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_Border_Left");
	m_pBorderLeft.lock()->Set_UIPosition(m_tUIDesc.fX - (m_tUIDesc.fSizeX * 0.5f), m_tUIDesc.fY, 11.f, 8.f);

	m_pBorderRight = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
	m_pBorderRight.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_Border_Right");
	m_pBorderRight.lock()->Set_UIPosition(m_tUIDesc.fX + (m_tUIDesc.fSizeX * 0.5f), m_tUIDesc.fY, 26.f, 8.f);

	m_tUIDesc.fDepth = 0.f;
	
	m_fMaxMp = 150.f;
	m_fCurrentMp = m_fMaxMp;
	m_fLerpedMp = m_fMaxMp;

	m_tUIDesc.fDepth = 0.f;

	m_tTextInfo.bAlways = false;
	m_tTextInfo.bCenterAlign = false;
	m_tTextInfo.fRotation = 0.f;
	m_tTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
	m_tTextInfo.vScale = _float2(0.5, 0.5f);
	m_tTextInfo.vPosition = _float2(m_tUIDesc.fX + m_tUIDesc.fSizeX * 0.5f + 20.f, m_tUIDesc.fY - 10.f);

	m_eRenderGroup = RENDERGROUP::RENDER_UI;
	

	m_vecChildUI.push_back(m_pMainBar);
	m_vecChildUI.push_back(m_pBG);
	m_vecChildUI.push_back(m_pBorderLeft);
	m_vecChildUI.push_back(m_pBorderRight);

	return S_OK;
}

HRESULT CPlayer_MPBar::Start()
{
	__super::Start();

	return S_OK;
}

void CPlayer_MPBar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY_INPUT(KEY::Z, KEY_STATE::TAP))
	{

		_float Hp = m_fCurrentMp -= 100.f;
		Set_CurrentHp(Hp, true);

	}

	else if (KEY_INPUT(KEY::X, KEY_STATE::TAP))
	{
		_float Hp = m_fCurrentMp += 100.f;
		Set_CurrentHp(Hp, true);
	}


	if (m_fCurrentMp < 0.f)
		m_fCurrentMp = 0.f;
	else if (m_fCurrentMp > m_fMaxMp)
		m_fCurrentMp = m_fMaxMp;

	if (Is_Lerping())
		m_fLerpedMp = Get_Lerp().x;

	_float fRatio = m_fLerpedMp / m_fMaxMp;

	m_pMainBar.lock()->Set_Ratio(fRatio);

	m_tTextInfo.szText = to_wstring((_uint)m_fLerpedMp);
	m_tTextInfo.szText.append(L"/");
	m_tTextInfo.szText.append(to_wstring((_uint)m_fMaxMp));

}

void CPlayer_MPBar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_tTextInfo);

}

HRESULT CPlayer_MPBar::Render()
{
	//__super::Render();

	return S_OK;;
}

void CPlayer_MPBar::OnEventMessage(_uint iArg)
{
}

void CPlayer_MPBar::Set_CurrentHp(_float _fCurrentHp, _bool bLerp, EASING_TYPE eLerpType)
{
	m_fCurrentMp = _fCurrentHp;

	if (m_fCurrentMp <= 0.f)
		m_fCurrentMp = 0.f;

	if (!bLerp)
		m_fLerpedMp = m_fCurrentMp;
	else
		Set_Lerp(m_fLerpedMp, m_fCurrentMp, 1.f, eLerpType);
}


HRESULT CPlayer_MPBar::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

void CPlayer_MPBar::Free()
{
	__super::Free();

}
