#include "stdafx.h"
#include "UI_PauseMenu_Tap_Potion.h"
#include "UI_PauseMenu_Page_Status.h"
#include "UI_PauseMenu_Tap_Status.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "GameObject.h"
#include "UI_PauseMenu_Tap_PlagueWeapon.h"


GAMECLASS_C(CUI_PauseMenu_Tap_Potion)
CLONE_C(CUI_PauseMenu_Tap_Potion, CGameObject)

//473

HRESULT CUI_PauseMenu_Tap_Potion::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_PauseMenu_Tap_Potion::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

	UI_DESC MainDesc;


	MainDesc.fDepth = 0.4f;
	MainDesc.fSizeX = 114.f;
	MainDesc.fSizeY = 114.f;
	MainDesc.fX = 656.f + 470.f;
	MainDesc.fY = 306.f;


	m_pFrame = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &MainDesc);
	m_pFrame.lock()->Set_Texture("Tap_PlagueWeapon");


	MainDesc.fSizeX = 90.f;
	MainDesc.fSizeY = 90.f;

	m_pIcon = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &MainDesc);
	m_pIcon.lock()->Set_Texture("Potion_Default");

	UI_DESC	tDecorationDesc;

	tDecorationDesc.fSizeX = 16.f;
	tDecorationDesc.fSizeY = 16.f;
	tDecorationDesc.fX = MainDesc.fX + 90.f;
	tDecorationDesc.fY = MainDesc.fY;
	tDecorationDesc.fDepth = 0.3f;

	for (_uint i = 0; i < 3; i++)
	{
		tDecorationDesc.fY = MainDesc.fY + (58.f * (_float)i);

		m_pDecoration[i] = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &tDecorationDesc);
		m_pDecoration[i].lock()->Set_Texture("Font_Diamond");

		Add_Child(m_pDecoration[i]);
	}
	Add_Child(m_pFrame);
	Add_Child(m_pIcon);







	m_PotionTapInfo.bAlways = false;
	m_PotionTapInfo.bCenterAlign = true;
	m_PotionTapInfo.fRotation = 0.f;
	m_PotionTapInfo.szText = L"Potion";
	m_PotionTapInfo.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_PotionTapInfo.vPosition = _float2(1289.f, 174.f);
	m_PotionTapInfo.vScale = _float2(1.3f, 1.3f);
	m_PotionTapInfo.eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

	return S_OK;
}

HRESULT CUI_PauseMenu_Tap_Potion::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_PauseMenu_Tap_Potion::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CUI_PauseMenu_Tap_Potion::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_PotionTapInfo);


}

HRESULT CUI_PauseMenu_Tap_Potion::Render(ID3D11DeviceContext* pDeviceContext)
{
	//
	return S_OK;
}
