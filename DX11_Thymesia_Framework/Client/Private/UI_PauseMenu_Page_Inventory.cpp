#include "stdafx.h"
#include "UI_PauseMenu_Page_Inventory.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "UI_Inventory.h"
#include "GameManager.h"
#include "UI_ItemInformation.h"
#include "UIManager.h"

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


	m_pInventory.lock()->Callback_OnMouseOver +=
		bind(&CUI_PauseMenu_Page_Inventory::Call_OnMouseOver, this, placeholders::_1);

	m_pInventory.lock()->Callback_OnMouseOut +=
		bind(&CUI_PauseMenu_Page_Inventory::Call_OnMouseOut, this);


	Add_Child(m_pInventory);
	m_pInventory.lock()->Set_RenderGroup(RENDERGROUP::RENDER_UI);
	
}

void CUI_PauseMenu_Page_Inventory::Create_ItemInformationTap()
{
	m_pItemInformation = GAMEINSTANCE->Add_GameObject<CUI_ItemInformation>(LEVEL_STATIC);
	Add_Child(m_pItemInformation);

}

void CUI_PauseMenu_Page_Inventory::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	GET_SINGLE(CUIManager)->EnableCursor();
}

void CUI_PauseMenu_Page_Inventory::OnDisable()
{
	__super::OnDisable();

	GET_SINGLE(CUIManager)->DisableCursor();
}

void CUI_PauseMenu_Page_Inventory::Call_OnMouseOver(weak_ptr<CItem> pItem)
{
	m_pItemInformation.lock()->Bind_Item(pItem);
}

void CUI_PauseMenu_Page_Inventory::Call_OnMouseOut()
{
	m_pItemInformation.lock()->UnBind_Item();
}
