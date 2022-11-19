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

	if (KEY_INPUT(KEY::CTRL, KEY_STATE::TAP) && m_bOpenThisFrame == false)
	{
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
		if (m_iPageIndex < (_uint)PAUSE_MENU_QUIT)
		{
			m_iPageIndex++;
			OnPaging();
		}
	}
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

HRESULT CUI_PauseMenu::Render()
{
	return S_OK;
}

void CUI_PauseMenu::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);

	m_bOpenThisFrame = true;

	//레지스터에 등록된 배틀UI들 모조리 꺼버리기.
	list<weak_ptr<CGameObject>> uiList = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::BATTLEUI);
	for (auto& elem : uiList)
		elem.lock()->Set_Enable(false);

	OnPaging();
}

void CUI_PauseMenu::OnDisable()
{
	__super::OnDisable();

	//레지스터에 등록된 배틀UI들 모조리 다시 켜주기.
	list<weak_ptr<CGameObject>> uiList = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::BATTLEUI);
	for (auto& elem : uiList)
		elem.lock()->Set_Enable(true);


}

void CUI_PauseMenu::OnPaging()
{

	for (_uint i = 0; i < (_uint)PAUSE_MENU_END; i++)
	{
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

	m_pPauseMenuBackground_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_Main.lock()->Set_Depth(0.9f);
	m_pPauseMenuBackground_Main.lock()->Set_Texture("PauseMenu_Background1");


	m_pPauseMenuBackground_Top = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_Top.lock()->Set_Depth(0.3f);
	m_pPauseMenuBackground_Top.lock()->Set_Texture("PauseMenu_Background3");


	m_vecChildUI.push_back(m_pPauseMenuBackground);
	m_vecChildUI.push_back(m_pPauseMenuBackground_Main);
	m_vecChildUI.push_back(m_pPauseMenuBackground_Top);
}

void CUI_PauseMenu::Create_Pages()
{
	m_pPages[PAUSE_MENU_STATUS] = GAMEINSTANCE->Add_GameObject<CUI_PauseMenu_Page_Status>(LEVEL_STATIC);
	m_vecChildUI.push_back(m_pPages[PAUSE_MENU_STATUS]);
}

void CUI_PauseMenu::Create_PageText()
{
	m_szPageTextData[PAUSE_MENU_STATUS] = L"STATUS";
	m_szPageTextData[PAUSE_MENU_TALENT] = L"TALENT";
	m_szPageTextData[PAUSE_MENU_ITEM] = L"ITEM";
	m_szPageTextData[PAUSE_MENU_COLLETION] = L"COLLECTION";
	m_szPageTextData[PAUSE_MENU_OPTION] = L"OPTION";
	m_szPageTextData[PAUSE_MENU_QUIT] = L"QUIT";

	for (_uint i = 0; i < (_uint)PAUSE_MENU_END; i++)
	{
		m_PageTexInfo[i].bAlways = false;
		m_PageTexInfo[i].bCenterAlign = true;
		m_PageTexInfo[i].fRotation = 0.f;
		m_PageTexInfo[i].szText = m_szPageTextData[i];
		m_PageTexInfo[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);
		m_PageTexInfo[i].vPosition = _float2(440.f + (140.f * (_float)i), 30.f);
		m_PageTexInfo[i].vScale = _float2(0.8f, 0.8f);


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

		m_vecChildUI.push_back(m_pPageIndicator[i]);
	}

	desc.fSizeX = 390.f;
	desc.fSizeY = 4.f;                                                                                                                                                                                                                                                                                                                                                                    
	desc.fDepth = 0.2f;
	desc.fY = 87.f;
	desc.fX = 789.f;
	m_pPageIndicatorDecoration = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &desc);
	m_pPageIndicatorDecoration.lock()->Set_Texture("PageIndex_Indicator_Decoration");
	m_vecChildUI.push_back(m_pPageIndicatorDecoration);

	desc.fSizeX = 65.;
	desc.fSizeY = 17.f;
	desc.fDepth = 0.2f;
	desc.fY = 87.f;
	desc.fX = 789.f;

	m_pPageTitleUnderLine = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &desc);
	m_pPageTitleUnderLine.lock()->Set_Texture("PageIndex_UnderLine");
	m_vecChildUI.push_back(m_pPageTitleUnderLine);


}
