#include "stdafx.h"
#include "UI_PauseMenu.h"
#include "HUD_Hover.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "Fader.h"
#include "Engine_Defines.h"
#include "UI_PauseMenu_Page_Status.h"
#include "FadeMask.h"
#include "UI_PauseMenu_Page_Inventory.h"
#include "UIManager.h"


GAMECLASS_C(CUI_PauseMenu)
CLONE_C(CUI_PauseMenu, CGameObject)

HRESULT CUI_PauseMenu::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_PauseMenu::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

	Create_Background();
	Create_Pages();
	Create_PageText();
	Create_PageIndicator();


	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	m_bOpenThisFrame = false;
	Set_Enable(false);
	m_iPageIndex = 0;
	m_bFonts = true;
	return S_OK;
}

HRESULT CUI_PauseMenu::Start()
{
	__super::Start();


	return S_OK;
}

void CUI_PauseMenu::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Update_KeyInput();
	m_bOpenThisFrame = false;
}

void CUI_PauseMenu::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (!m_bFonts)
		return;
	for (_uint i = 0; i < (_uint)PAUSE_MENU_END; i++)
	{
		GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_PageTexInfo[i]);
	}
}

HRESULT CUI_PauseMenu::Render(ID3D11DeviceContext* pDeviceContext)
{
	return S_OK;
}

void CUI_PauseMenu::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);

	GAMEINSTANCE->PlaySound2D("Fantasy_Game_Book_Page_Turn_1_M.ogg",
		GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHOOSE_SELECT)
	);

	m_bOpenThisFrame = true;
	GET_SINGLE(CUIManager)->Set_OpenedMenu(true);


	m_pPauseMenuBackground.lock()->Set_AlphaColor(1.f);
	m_pPauseMenuBackground_MainFrame.lock()->Set_AlphaColor(1.f);
	m_pPauseMenuBackground_Main.lock()->Set_AlphaColor(1.f);
	m_pPauseMenuBackground_Top.lock()->Set_AlphaColor(1.f);

	OnPaging();
}

void CUI_PauseMenu::OnDisable()
{
	__super::OnDisable();

	GET_SINGLE(CUIManager)->Set_OpenedMenu(false);
	m_pInventoryBG.lock()->Set_Enable(false);




}

void CUI_PauseMenu::OnPaging()
{
	for (_uint i = 0; i < (_uint)PAUSE_MENU_END; i++)
	{
		GAMEINSTANCE->PlaySound2D("Fantasy_Game_Book_Page_Turn_2_M.ogg",
			GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHOOSE_SELECT));

		if (i == m_iPageIndex)//현재 페이지 인덱스
		{
			if (m_pPages[i].lock() != nullptr)
				m_pPages[i].lock()->Set_Enable(true);

			m_PageTexInfo[i].vPosition.y = 35.f;
			m_PageTexInfo[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);

			m_pPageIndicator[i].lock()->Set_Texture("PageIndex_Indicator_Selected");
			m_pPageTitleUnderLine.lock()->Set_UIPosition(m_PageTexInfo[i].vPosition.x, m_PageTexInfo[i].vPosition.y + 40.f);
		}
		else
		{
			if (m_pPages[i].lock() != nullptr)
				m_pPages[i].lock()->Set_Enable(false);
			m_PageTexInfo[i].vPosition.y = 30.f;
			m_PageTexInfo[i].vColor = _float4(0.4f, 0.4f, 0.4f, 0.4f);

			m_pPageIndicator[i].lock()->Set_Texture("PageIndex_Indicator_UnSelected");
		}
	}
	if (m_iPageIndex == (_uint)PAUSE_MENU_ITEM)
	{
		m_pPauseMenuBackground_Main.lock()->Set_Enable(false);
		m_pPauseMenuBackground_MainFrame.lock()->Set_Enable(false);
		m_pPauseMenuBackground.lock()->Set_Enable(false);

		m_pInventoryBG.lock()->Set_Enable(true);
		
	}
	else
	{
		m_pPauseMenuBackground_Main.lock()->Set_Enable(true);
		m_pPauseMenuBackground_MainFrame.lock()->Set_Enable(true);
		m_pPauseMenuBackground.lock()->Set_Enable(true);



		m_pInventoryBG.lock()->Set_Enable(false);
	}
}

void CUI_PauseMenu::Update_KeyInput()
{
	if (GET_SINGLE(CUIManager)->Is_Animation())
		return;

	if (KEY_INPUT(KEY::CTRL, KEY_STATE::TAP) && m_bOpenThisFrame == false)
	{

		GAMEINSTANCE->PlaySound2D("Fantasy_Game_Book_Page_Turn_1_M.ogg",
			GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHOOSE_SELECT));

		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 0.3f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_PauseMenu::Call_FadeInPauseMenu, this);
	}


	if (KEY_INPUT(KEY::Q, KEY_STATE::TAP))
	{
		if (m_iPageIndex > 0)
		{
			m_iPageIndex--;
			OnPaging();
		}
	}
	if (KEY_INPUT(KEY::E, KEY_STATE::TAP))
	{
		if (m_iPageIndex < (_uint)PAUSE_MENU_TYPE::PAUSE_MENU_ITEM)
		{
			m_iPageIndex++;
			OnPaging();
		}
	}
}

void CUI_PauseMenu::Call_FadeInPauseMenu()
{
	Set_Enable(false);
	m_pFadeMask.lock()->Set_Enable(false);
}

void CUI_PauseMenu::Create_Background()
{

	m_pPauseMenuBackground = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground.lock()->Set_Depth(1.f);
	m_pPauseMenuBackground.lock()->Set_Texture("PauseMenu_Background0");
	m_pPauseMenuBackground.lock()->Set_UIPosition(0, 0, 1600.f, 900.f, CUI::ALIGN_LEFTTOP);
	

	m_pPauseMenuBackground_MainFrame = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_MainFrame.lock()->Set_Depth(0.9f);
	m_pPauseMenuBackground_MainFrame.lock()->Set_Texture("PauseMenu_Background1");
	m_pPauseMenuBackground_MainFrame.lock()->Set_UIPosition(0, 0, 1600.f, 900.f, CUI::ALIGN_LEFTTOP);


	m_pPauseMenuBackground_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_Main.lock()->Set_Depth(0.8f);
	m_pPauseMenuBackground_Main.lock()->Set_Texture("PauseMenu_Background2");
	m_pPauseMenuBackground_Main.lock()->Set_UIPosition(0.f, 0.f, 1598.f, 895.f, ALIGN_LEFTTOP);


	m_pPauseMenuBackground_Top = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_Top.lock()->Set_Depth(0.7f);
	m_pPauseMenuBackground_Top.lock()->Set_Texture("PauseMenu_Background3");
	m_pPauseMenuBackground_Top.lock()->Set_UIPosition(0, 0, 1600.f, 900.f, CUI::ALIGN_LEFTTOP);


	m_pInventoryBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pInventoryBG.lock()->Set_Depth(0.9f);
	m_pInventoryBG.lock()->Set_Texture("Inventory_BG");
	m_pInventoryBG.lock()->Set_UIPosition(0, 0, 1600.f, 900.f, CUI::ALIGN_LEFTTOP);

	Add_Child(m_pPauseMenuBackground);
	Add_Child(m_pPauseMenuBackground_MainFrame);
	Add_Child(m_pPauseMenuBackground_Main);
	Add_Child(m_pPauseMenuBackground_Top);

	m_pInventoryBG.lock()->Set_RenderGroup(RENDERGROUP::RENDER_AFTER_UI);
	m_pPauseMenuBackground_Top.lock()->Set_RenderGroup(RENDERGROUP::RENDER_AFTER_UI);
	m_pPauseMenuBackground_Main.lock()->Set_RenderGroup(RENDERGROUP::RENDER_AFTER_UI);
	m_pPauseMenuBackground_MainFrame.lock()->Set_RenderGroup(RENDERGROUP::RENDER_AFTER_UI);
	m_pPauseMenuBackground.lock()->Set_RenderGroup(RENDERGROUP::RENDER_AFTER_UI);


}

void CUI_PauseMenu::Create_Pages()
{
	m_pPages[PAUSE_MENU_STATUS] = GAMEINSTANCE->Add_GameObject<CUI_PauseMenu_Page_Status>(LEVEL_STATIC);
	m_vecChildUI.push_back(m_pPages[PAUSE_MENU_STATUS]);


	m_pPages[PAUSE_MENU_ITEM] = GAMEINSTANCE->Add_GameObject<CUI_PauseMenu_Page_Inventory>(LEVEL_STATIC);
	m_vecChildUI.push_back(m_pPages[PAUSE_MENU_ITEM]);
}

void CUI_PauseMenu::Create_PageText()
{
	m_szPageTextData[PAUSE_MENU_STATUS] = L"STATUS";
	m_szPageTextData[PAUSE_MENU_ITEM] = L"ITEM";

	for (_uint i = 0; i < (_uint)PAUSE_MENU_END; i++)
	{
		m_PageTexInfo[i].bAlways = false;
		m_PageTexInfo[i].bCenterAlign = true;
		m_PageTexInfo[i].fRotation = 0.f;
		m_PageTexInfo[i].szText = m_szPageTextData[i];
		m_PageTexInfo[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);
		m_PageTexInfo[i].vPosition = _float2(720.f + (140.f * (_float)i), 30.f);
		m_PageTexInfo[i].vScale = _float2(0.8f, 0.8f);
		m_PageTexInfo[i].eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

		/*
		UI_DESC	tUIDesc;
		tUIDesc.fDepth = 0.f;
		tUIDesc.fX = 440.f + (140.f * (_float)i);
		tUIDesc.fY = 30.f;
		tUIDesc.fSizeX = 100.f;
		tUIDesc.fSizeY = 25.f;
		//m_pFonts
		m_pFonts[i] = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &tUIDesc);
		m_pFonts[i].lock()->Set_Texture("PageFont_Status");
		m_vecChildUI.push_back(m_pFonts[i]);
		*/
	}

}

void CUI_PauseMenu::Create_PageIndicator()
{
	UI_DESC desc;

	desc.fSizeX = 18.f;
	desc.fSizeY = 18.f;
	desc.fDepth = 0.1f;
	desc.fY = 87.f;

	for (_uint i = 0; i < (_uint)PAUSE_MENU_END; i++)
	{
		desc.fX = 722.f + ((_float)i * 30.f);

		m_pPageIndicator[i] = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &desc);
		m_pPageIndicator[i].lock()->Set_Texture("PageIndex_Indicator_UnSelected");

		Add_Child(m_pPageIndicator[i]);
	}

	desc.fSizeX = 390.f;
	desc.fSizeY = 4.f;                                                                                                                                                                                                                                                                                                                                                                    
	desc.fDepth = 0.2f;
	desc.fY = 87.f;
	desc.fX = 789.f;
	m_pPageIndicatorDecoration = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &desc);
	m_pPageIndicatorDecoration.lock()->Set_Texture("PageIndex_Indicator_Decoration");
	Add_Child(m_pPageIndicatorDecoration);

	desc.fSizeX = 65.;
	desc.fSizeY = 17.f;
	desc.fDepth = 0.2f;
	desc.fY = 87.f;
	desc.fX = 789.f;

	m_pPageTitleUnderLine = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &desc);
	m_pPageTitleUnderLine.lock()->Set_Texture("PageIndex_UnderLine");
	Add_Child(m_pPageTitleUnderLine);


}
