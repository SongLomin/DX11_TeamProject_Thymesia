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
    __super::Tick(fTimeDelta);
}

void CMonsterHPBar_Boss::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CMonsterHPBar_Boss::Render()
{
    //
    return S_OK;
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
