#include "stdafx.h"
#include "HUD_PlagueWeapon.h"
#include "HUD_PlagueWeaponBase.h"
#include "HUD_PlagueWeapon_Main.h"
#include "HUD_PlagueWeapon_Steal.h"


GAMECLASS_C(CHUD_PlagueWeapon);
CLONE_C(CHUD_PlagueWeapon, CGameObject);

HRESULT CHUD_PlagueWeapon::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CHUD_PlagueWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	UI_DESC tMainSKillDesc = {1405.f, 785.f, 93.f, 93.f, 0.f };


	m_pMainSkill = GAMEINSTANCE->Add_GameObject<CHUD_PlagueWeapon_Main>(LEVEL_STATIC, &tMainSKillDesc);

	UI_DESC tStealSKillDesc = { 1502.f, 785.f, 93.f, 93.f, 0.f };

	m_pStealSkill = GAMEINSTANCE->Add_GameObject<CHUD_PlagueWeapon_Steal>(LEVEL_STATIC, &tStealSKillDesc);


	m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

	Add_Child(m_pMainSkill);
	Add_Child(m_pStealSkill);


	return S_OK;
}

HRESULT CHUD_PlagueWeapon::Start()
{
	//TODO : 여기에 플레이어 장착 코드 넣어야함.
	__super::Start();

	return S_OK;
}

void CHUD_PlagueWeapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CHUD_PlagueWeapon::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}
