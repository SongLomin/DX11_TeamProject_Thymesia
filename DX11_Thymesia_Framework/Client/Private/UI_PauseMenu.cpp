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

	Create_PageText();
	Create_PageIndicator();


	m_pPauseMenuBackground_Top = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_Top.lock()->Set_Depth(0.2f);
	m_pPauseMenuBackground_Top.lock()->Set_Texture("PauseMenu_Background2");


	m_vecChildUI.push_back(m_pPauseMenuBackground);
	m_vecChildUI.push_back(m_pPauseMenuBackground_Main);
	m_vecChildUI.push_back(m_pPauseMenuBackground_Top);
	
	m_bOpenThisFrame = false;
	Set_Enable(false);
	m_iPageIndex = 0;
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
		Set_Enable(false);


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

	for (_uint i = 0; i < (_uint)PAUSE_MENU_END; i++)
	{
		GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_PageTexInfo[i]);
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
			m_PageTexInfo[i].vPosition.y = 35.f;
			m_PageTexInfo[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);

			m_pPageIndicator[i].lock()->Set_Texture("PageIndex_Indicator_Selected");
		}
		else
		{
			m_PageTexInfo[i].vPosition.y = 30.f;
			m_PageTexInfo[i].vColor = _float4(0.4f, 0.4f, 0.4f, 0.4f);

			m_pPageIndicator[i].lock()->Set_Texture("PageIndex_Indicator_UnSelected");

		}
	}

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

void CUI_PauseMenu::Create_PageText()
{
	m_szPageTextData[PAUSE_MENU_STATUS] = L"상태";
	m_szPageTextData[PAUSE_MENU_TALENT] = L"특성";
	m_szPageTextData[PAUSE_MENU_ITEM] = L"아이템";
	m_szPageTextData[PAUSE_MENU_COLLETION] = L"컬렉션";
	m_szPageTextData[PAUSE_MENU_OPTION] = L"옵션";
	m_szPageTextData[PAUSE_MENU_QUIT] = L"종료";


	for (_uint i = 0; i < (_uint)PAUSE_MENU_END; i++)
	{
		m_PageTexInfo[i].bAlways = false;
		m_PageTexInfo[i].bCenterAlign = true;
		m_PageTexInfo[i].fRotation = 0.f;
		m_PageTexInfo[i].szText = m_szPageTextData[i];
		m_PageTexInfo[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);
		m_PageTexInfo[i].vPosition = _float2(440.f + (140.f * (_float)i), 30.f);
		m_PageTexInfo[i].vScale = _float2(0.6f, 0.6f);
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
}
