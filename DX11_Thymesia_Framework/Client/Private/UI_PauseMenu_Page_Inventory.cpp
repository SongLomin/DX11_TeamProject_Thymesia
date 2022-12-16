#include "stdafx.h"
#include "UI_PauseMenu_Page_Inventory.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "UI_Inventory.h"

GAMECLASS_C(CUI_PauseMenu_Page_Inventory)
CLONE_C(CUI_PauseMenu_Page_Inventory, CGameObject)

HRESULT CUI_PauseMenu_Page_Inventory::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

	Create_InventoryUITap();
	Create_ItemInformationTap();

	return S_OK;
}

HRESULT CUI_PauseMenu_Page_Inventory::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_PauseMenu_Page_Inventory::Tick(_float fTimeDelta)
{
	fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

	__super::Tick(fTimeDelta);

}

void CUI_PauseMenu_Page_Inventory::LateTick(_float fTimeDelta)
{
	fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

	__super::LateTick(fTimeDelta);

}

void CUI_PauseMenu_Page_Inventory::Create_InventoryUITap()
{
	m_pInventory = GAMEINSTANCE->Add_GameObject<CUI_Inventory>(LEVEL_STATIC);

	Add_Child(m_pInventory);
	m_pInventory.lock()->Set_RenderGroup(RENDERGROUP::RENDER_UI);
	
}

void CUI_PauseMenu_Page_Inventory::Create_ItemInformationTap()
{

}
