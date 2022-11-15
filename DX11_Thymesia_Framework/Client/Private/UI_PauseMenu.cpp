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

	m_pPauseMenuBackground = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground.lock()->Set_Depth(0.4f);
	m_pPauseMenuBackground.lock()->Set_Texture("PauseMenu_Background0");

	m_pPauseMenuBackground_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_Main.lock()->Set_Depth(0.3f);
	m_pPauseMenuBackground_Main.lock()->Set_Texture("PauseMenu_Background1");


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

	m_bOpenThisFrame = false;
}

void CUI_PauseMenu::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

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


}

void CUI_PauseMenu::OnDisable()
{
	__super::OnDisable();

	//레지스터에 등록된 배틀UI들 모조리 다시 켜주기.
	list<weak_ptr<CGameObject>> uiList = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::BATTLEUI);
	for (auto& elem : uiList)
		elem.lock()->Set_Enable(true);


}
