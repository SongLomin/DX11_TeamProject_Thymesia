#include "stdafx.h"
#include "Player_MPBar.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "Player_ProgressBar.h"


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

	m_pBG = GAMEINSTANCE->Add_GameObject<CPlayer_ProgressBar>(LEVEL_STATIC);
	m_pBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_BG");
	m_pBG.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, m_tUIDesc.fSizeX, m_tUIDesc.fSizeY);

	m_pMainBar = GAMEINSTANCE->Add_GameObject<CPlayer_ProgressBar>(LEVEL_STATIC);
	m_pMainBar.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_MainBar");
	m_pMainBar.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, m_tUIDesc.fSizeX, m_tUIDesc.fSizeY - 5.f);

	m_pBorderLeft = GAMEINSTANCE->Add_GameObject<CPlayer_ProgressBar>(LEVEL_STATIC);
	m_pBorderLeft.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_Border_Left");
	m_pBorderLeft.lock()->Set_UIPosition(m_tUIDesc.fX - (m_tUIDesc.fSizeX * 0.5f), m_tUIDesc.fY, 11.f, 8.f);

	m_pBorderRight = GAMEINSTANCE->Add_GameObject<CPlayer_ProgressBar>(LEVEL_STATIC);
	m_pBorderRight.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_MPBar_Border_Right");
	m_pBorderRight.lock()->Set_UIPosition(m_tUIDesc.fX + (m_tUIDesc.fSizeX * 0.5f), m_tUIDesc.fY, 26.f, 8.f);

	m_tUIDesc.fDepth = 0.f;
	
	m_fMaxMp = 150.f;
	m_fCurrentMp = m_fMaxMp;
	m_fLerpMp = m_fCurrentMp;

	m_tUIDesc.fDepth = 0.f;
	m_fLerpAcc = 1.f;

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

//TODO : UI : MPBar 테스트 코드
#ifdef _DEBUG
	if (KEY_INPUT(KEY::J, KEY_STATE::TAP))
	{
		m_fCurrentMp -= 30.f;

	}
	if (KEY_INPUT(KEY::K, KEY_STATE::TAP))
	{

		m_fCurrentMp += 30.f;
		m_fLerpMp = m_fCurrentMp;
		m_fLerpAcc = 1.f;
	}
	if (KEY_INPUT(KEY::Q, KEY_STATE::TAP))
	{
		m_fCurrentMp -= 200.f;

	}
	//if (KEY_INPUT(KEY::W, KEY_STATE::TAP))
	//{
	//
	//	m_fCurrentMp += 200.f;
	//	m_fLerpMp = m_fCurrentMp;
	//	m_fLerpAcc = 1.f;
	//}
#endif // _DEBUG

	if (m_fCurrentMp < 0.f)
		m_fCurrentMp = 0.f;
	else if (m_fCurrentMp > m_fMaxMp)
		m_fCurrentMp = m_fMaxMp;

	if (m_fCurrentMp != m_fLerpMp)
	{
		_float fRight = max(m_fCurrentMp, m_fLerpMp);
		_float fLeft = min(m_fCurrentMp, m_fLerpMp);
		_float fLerp;

		fLerp = SMath::Lerp(fLeft, fRight, fTimeDelta * m_fLerpAcc);

		if (fabs(m_fCurrentMp - m_fLerpMp) > 1.f)
		{
			m_fLerpMp -= (fLerp - fLeft);//러프 차이값만큼 빼줌
			m_fLerpAcc += 0.4f;
		}
		else
		{
			m_fLerpMp = m_fCurrentMp;
			m_fLerpAcc = 1.f;
		}
	}
	_float fRatio = m_fLerpMp / m_fMaxMp;

	m_pMainBar.lock()->Set_Ratio(fRatio);

	m_tTextInfo.szText = to_wstring((_uint)m_fCurrentMp);
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

void CPlayer_MPBar::Set_CurrentMp(_float _fCurrentMp)
{
	if (_fCurrentMp > m_fCurrentMp)
	{
		m_fLerpMp = _fCurrentMp;
		m_fLerpAcc = 1.f;
	}
	m_fCurrentMp = _fCurrentMp;
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
