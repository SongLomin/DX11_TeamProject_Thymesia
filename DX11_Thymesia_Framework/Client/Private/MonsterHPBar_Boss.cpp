#include "stdafx.h"
#include "MonsterHPBar_Boss.h"
#include "MonsterHPBar_Base.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "ProgressBar.h"
#include "HUD_Hover.h"
#include "Fader.h"
#include "Monster.h"
#include "Status_Boss.h"
#include "BossMonster.h"
#include "MonsterParryingBar.h"
GAMECLASS_C(CMonsterHPBar_Boss)
CLONE_C(CMonsterHPBar_Boss, CGameObject)

HRESULT CMonsterHPBar_Boss::Initialize_Prototype()
{
    __super::Initialize_Prototype();


    return S_OK;
}

HRESULT CMonsterHPBar_Boss::Initialize(void* pArg)
{
    CUI::UI_DESC tDesc;
    tDesc.fX = g_iWinCX / 2.f;
    tDesc.fY = 69.f;
    tDesc.fSizeX = 810.f;
    tDesc.fSizeY = 15.f;
    tDesc.fDepth = 0.f;
    __super::Initialize(&tDesc);

    Set_Enable(false);
    return S_OK;
}

HRESULT CMonsterHPBar_Boss::Start()
{
    __super::Start();

    return S_OK;
}

void CMonsterHPBar_Boss::Tick(_float fTimeDelta)
{
    //__super::Tick(fTimeDelta);

	Check_Track();
	if (!Get_Enable())
	{
		m_pRecovery.lock()->Set_Enable(false);
		m_pStunned.lock()->Set_Enable(false);
	}
}

void CMonsterHPBar_Boss::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CMonsterHPBar_Boss::Render(ID3D11DeviceContext* pDeviceContext)
{

    return S_OK;
}

void CMonsterHPBar_Boss::Set_Target(weak_ptr<CBase> pTarget)
{
	__super::Set_Target(pTarget);

	weak_ptr<CStatus_Boss> pStatus_Monster;
	pStatus_Monster = Weak_StaticCast<CStatus_Boss>(m_pTarget.lock()->Get_Status());

	Create_Decoration(pStatus_Monster);
}


void	CMonsterHPBar_Boss::Bind_EventFunction(weak_ptr<CStatus_Monster> pStatus_Monster)
{
	__super::Bind_EventFunction(pStatus_Monster);

	pStatus_Monster.lock()->Callback_NextPhase += bind(&CMonsterHPBar_Boss::Call_NextPhase, this);
	pStatus_Monster.lock()->CallBack_ReStart += bind
	(
		&CMonsterHPBar_Base::Call_Restart, this
	);
}

void	CMonsterHPBar_Boss::Create_Decoration(weak_ptr<CStatus_Monster> pStatus_Monster)
{
	for (auto& elem : m_listLifeDecoration)
	{
		elem.lock()->Set_Enable(false);
	}
	m_listLifeDecoration.clear();


	weak_ptr<CCustomUI>	pLifeDecoration;
	_float				DecorationOffset = 16.f;
	_float2				DecorationStartPos = m_pBorder.lock()->Get_Point(CUI::UI_POINT::RIGHT);

	for (_uint i = 1; i <= pStatus_Monster.lock()->Get_Desc().m_iLifeCount; i++)
	{
		pLifeDecoration = ADD_STATIC_CUSTOMUI;
		pLifeDecoration.lock()->Set_UIPosition
		(
			DecorationStartPos.x + DecorationOffset * i,
			DecorationStartPos.y, 14.f, 14.f
		);
		pLifeDecoration.lock()->Set_Texture("Font_Diamond");
		pLifeDecoration.lock()->Set_Enable(false);
		pLifeDecoration.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);
		m_listLifeDecoration.push_back(pLifeDecoration);
	}
}

void CMonsterHPBar_Boss::Call_Full_Recovery()
{
	for (auto& elem : m_listLifeDecoration)
	{
		elem.lock()->Set_Enable(false);
	}
	m_listLifeDecoration.clear();
	Create_Decoration(Weak_StaticCast<CStatus_Monster>(m_pTarget.lock()->Get_Status()));

	__super::Call_Full_Recovery();
}


void CMonsterHPBar_Boss::Set_Stun(bool _bStun)
{
	m_bStun = _bStun;

	if (m_bStun)
	{
		m_pGreen.lock()->Set_Enable(false);
		m_pWhite.lock()->Set_Enable(false);
		m_pTrack.lock()->Set_Enable(false);
		m_pDecoration_Head.lock()->Set_Enable(false);
	}
	else
	{
		m_pGreen.lock()->Set_Enable(true);
		m_pWhite.lock()->Set_Enable(true);
		m_pTrack.lock()->Set_Enable(true);
		m_pDecoration_Head.lock()->Set_Enable(true);
	}
}

void CMonsterHPBar_Boss::Set_ChildPosFromThis()
{
  // __super::Set_ChildPosFromThis();
}

void CMonsterHPBar_Boss::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	for (auto& elem : m_listLifeDecoration)
	{
		elem.lock()->Set_Enable(true);
	}
	
}

void CMonsterHPBar_Boss::OnDisable()
{
	__super::OnDisable();
	for (auto& elem : m_listLifeDecoration)
	{
		elem.lock()->Set_Enable(false);
	}
}

void CMonsterHPBar_Boss::Call_NextPhase()
{
	m_pWhite.lock()->Set_Enable(true);
	m_pGreen.lock()->Set_Enable(true);
	m_pBorder.lock()->Set_Enable(true);
    m_pWhite.lock()->Set_Ratio(1.f);
    m_pGreen.lock()->Set_Ratio(1.f);
	m_pParryingBar.lock()->Set_Ratio(0.f,false);
	m_listLifeDecoration.back().lock()->Set_Enable(false);
	m_listLifeDecoration.pop_back();
	for (auto& elem : m_listLifeDecoration)
	{
		elem.lock()->Set_Enable(true);
	}
}
