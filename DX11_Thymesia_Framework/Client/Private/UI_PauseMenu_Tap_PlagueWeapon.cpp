#include "stdafx.h"
#include "UI_PauseMenu_Tap_PlagueWeapon.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "Fader.h"
#include "GameManager.h"
#include "Client_GameObjects.h"
#include "CustomUI.h"
#include "UI_PlagueWeapon.h"
#include "Player.h"
#include "PlayerSkillHeader.h"

GAMECLASS_C(CUI_PauseMenu_Tap_PlagueWeapon)
CLONE_C(CUI_PauseMenu_Tap_PlagueWeapon, CGameObject)

HRESULT CUI_PauseMenu_Tap_PlagueWeapon::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_PauseMenu_Tap_PlagueWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

	UI_DESC MainDesc;

	MainDesc.fDepth = 0.4f;
	MainDesc.fSizeX = 114.f;
	MainDesc.fSizeY = 114.f;
	MainDesc.fX = 1000.f;
	MainDesc.fY = 306.f;

	m_PlagueWeaponMain = GAMEINSTANCE->Add_GameObject<CUI_PlagueWeapon>(LEVEL_STATIC, &MainDesc);
	m_PlagueWeaponMain.lock()->Set_Texture("Tap_PlagueWeapon");
	m_PlagueWeaponMain.lock()->Set_MainText(L"Plague Weapon - 1");
	m_PlagueWeaponMain.lock()->Set_SkillInfo("SkillIcon_Axe", L"Axe");

	MainDesc.fY = 306.f + 172.f;

	m_PlagueWeaponSub = GAMEINSTANCE->Add_GameObject<CUI_PlagueWeapon>(LEVEL_STATIC, &MainDesc);
	m_PlagueWeaponSub.lock()->Set_Texture("Tap_PlagueWeapon");
	m_PlagueWeaponSub.lock()->Set_MainText(L"Plague Weapon - 2");
	m_PlagueWeaponSub.lock()->Set_SkillInfo("SkillIcon_Axe", L"Axe");

	MainDesc.fY = 306.f + 172 * 2.f;
	MainDesc.fSizeX = 256.f;
	MainDesc.fSizeY = 256.f;

	m_PlagueWeaponSteal = GAMEINSTANCE->Add_GameObject<CUI_PlagueWeapon>(LEVEL_STATIC, &MainDesc);
	m_PlagueWeaponSteal.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Steal");
	m_PlagueWeaponSteal.lock()->Set_MainText(L"PlagueWeapon - Plunder");
	m_PlagueWeaponSteal.lock()->Set_SkillInfo("SkillIcon_Axe", L"Axe");
	m_PlagueWeaponSteal.lock()->Set_IconSize(228.f, 228.f);

	m_PlagueWeaponSteal_Decoration = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &MainDesc);
	m_PlagueWeaponSteal_Decoration.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Steal_Decoration");
	
	m_PlagueWeaponText.bAlways = false;
	m_PlagueWeaponText.bCenterAlign = true;
	m_PlagueWeaponText.fRotation = 0.f;
	m_PlagueWeaponText.szText = L"Plague Weapon";
	m_PlagueWeaponText.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_PlagueWeaponText.vPosition = _float2(1000.f, 174.f);
	m_PlagueWeaponText.vScale = _float2(1.3f, 1.3f);
	m_PlagueWeaponText.eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

	Add_Child(m_PlagueWeaponMain);
	Add_Child(m_PlagueWeaponSub);
	Add_Child(m_PlagueWeaponSteal);
	Add_Child(m_PlagueWeaponSteal_Decoration);

	return S_OK;
}

HRESULT CUI_PauseMenu_Tap_PlagueWeapon::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_PauseMenu_Tap_PlagueWeapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_PauseMenu_Tap_PlagueWeapon::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_PlagueWeaponText);

}

HRESULT CUI_PauseMenu_Tap_PlagueWeapon::Render(ID3D11DeviceContext* pDeviceContext)
{
	//
	return S_OK;
}

void CUI_PauseMenu_Tap_PlagueWeapon::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	weak_ptr<CPlayer>	pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock();
	
	if (pPlayer.lock())
	{
		weak_ptr<CPlayerSkill_System>	 pSkillSystem = pPlayer.lock()->Get_Component<CPlayerSkill_System>();

		weak_ptr<CSkill_Base> pMainSkill = pSkillSystem.lock()->Get_EquipSkill(CPlayerSkill_System::SOCKET_TYPE::SOCKET_MAIN);
		weak_ptr<CSkill_Base> pSubSkill = pSkillSystem.lock()->Get_EquipSkill(CPlayerSkill_System::SOCKET_TYPE::SOCKET_MAIN);
		weak_ptr<CSkill_Base> pStealSkill = pSkillSystem.lock()->Get_StealSkill();

		if (pMainSkill.lock())
		{
			m_PlagueWeaponMain.lock()->Set_SkillIcon(pMainSkill.lock()->Get_SkillName());
		}
		if (pSubSkill.lock())
		{
			m_PlagueWeaponSub.lock()->Set_SkillIcon(pSubSkill.lock()->Get_SkillName());
		}
		if (pStealSkill.lock())
		{
			m_PlagueWeaponSteal.lock()->Set_SkillIcon(pStealSkill.lock()->Get_SkillName());
		}
	}
	
}

