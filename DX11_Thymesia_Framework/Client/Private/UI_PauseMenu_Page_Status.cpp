#include "stdafx.h"
#include "UI_PauseMenu_Page_Status.h"

GAMECLASS_C(CUI_PauseMenu_Page_Status)
CLONE_C(CUI_PauseMenu_Page_Status, CGameObject)

HRESULT CUI_PauseMenu_Page_Status::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_PauseMenu_Page_Status::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

HRESULT CUI_PauseMenu_Page_Status::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_PauseMenu_Page_Status::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_PauseMenu_Page_Status::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CUI_PauseMenu_Page_Status::Render()
{

	//

	return S_OK;
}
