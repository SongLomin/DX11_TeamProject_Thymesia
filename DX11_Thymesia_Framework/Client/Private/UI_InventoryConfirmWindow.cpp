#include "stdafx.h"
#include "UI_InventoryConfirmWindow.h"
#include "UI_Utils.h"
#include "Preset_UIDesc.h"

GAMECLASS_C(CUI_InventoryConfirmWindow)
CLONE_C(CUI_InventoryConfirmWindow, CGameObject)

HRESULT CUI_InventoryConfirmWindow::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

HRESULT CUI_InventoryConfirmWindow::Start()
{
	__super::Start();


	return S_OK;
}

void CUI_InventoryConfirmWindow::Tick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();

	__super::Tick(fTimeDelta);

}

void CUI_InventoryConfirmWindow::LateTick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();

	__super::LateTick(fTimeDelta);
}
