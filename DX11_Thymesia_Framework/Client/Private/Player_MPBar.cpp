#include "stdafx.h"
#include "Player_MPBar.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "ProgressBar.h"
#include "UI_LerpBar.h"
#include "Preset_UIDesc.h"
#include "Status_Player.h"
#include "EasingComponent_Float.h"


GAMECLASS_C(CPlayer_MPBar)
CLONE_C(CPlayer_MPBar, CGameObject);

HRESULT CPlayer_MPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_MPBar::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	CPreset_UIDesc::Set_CUI_PlayerMPBar(Weak_StaticCast<CPlayer_MPBar>(m_this));

	SetUp_Component();


	GET_SINGLE(CGameManager)->CallBack_ChangePlayer +=
		bind(&CPlayer_MPBar::Bind_Player, this);


	
	return S_OK;
}

HRESULT CPlayer_MPBar::Start()
{
	__super::Start();

	Bind_Player();
	return S_OK;
}

void CPlayer_MPBar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pEasingFloatCom.lock()->Is_Lerping())
	{
		m_fLerpedMp = m_pEasingFloatCom.lock()->Get_Lerp();
	}
	m_pMainBar.lock()->Set_Ratio(m_fLerpedMp / m_fMaxMp);
	m_tTextInfo.szText = to_wstring((_uint)m_fLerpedMp);
	m_tTextInfo.szText.append(L"/");
	m_tTextInfo.szText.append(to_wstring((_uint)m_fMaxMp));

}

void CPlayer_MPBar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_tTextInfo);

}

HRESULT CPlayer_MPBar::Render(ID3D11DeviceContext* pDeviceContext)
{
	//__super::Render(pDeviceContext);

	return S_OK;;
}

void CPlayer_MPBar::OnEventMessage(_uint iArg)
{
	
}

void CPlayer_MPBar::ChangeMP(_float fCurrentMP)
{
	m_fCurrentMp = fCurrentMP;

	m_pEasingFloatCom.lock()->Set_Lerp(m_fLerpedMp, m_fCurrentMp, 1.f, EASING_TYPE::QUAD_IN,
		CEasingComponent::ONCE);
}


void CPlayer_MPBar::SetUp_Component()
{
	__super::SetUp_Component();

	m_pEasingFloatCom = Add_Component<CEasingComponent_Float>();
}

HRESULT CPlayer_MPBar::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	return S_OK;
}

void CPlayer_MPBar::Bind_Player()
{
	__super::Bind_Player();

	m_pPlayerStatus.lock()->Callback_ChangeMP
		+= bind(&CPlayer_MPBar::Call_ChangeMP, this, placeholders::_1);

	m_pPlayerStatus.lock()->Callback_Update_Status
		+= bind(&CPlayer_MPBar::Call_UpdateStatus, this);

	Update_MPBar();
}

void CPlayer_MPBar::Update_MPBar()
{
	CStatus_Player::PLAYERDESC tPlayerDesc = m_pPlayerStatus.lock()->Get_PlayerDesc();

	m_fMaxMp = tPlayerDesc.m_fMaxMP;
	m_fCurrentMp = m_fMaxMp;
	m_fLerpedMp = m_fMaxMp;

	m_pMainBar.lock()->Set_Ratio(1.f);
}

void CPlayer_MPBar::Call_UpdateStatus()
{
	Update_MPBar();
}

void CPlayer_MPBar::Call_ChangeMP(_float	fCurrentMp)
{
	ChangeMP(fCurrentMp);
}

void CPlayer_MPBar::Free()
{
	__super::Free();

}
