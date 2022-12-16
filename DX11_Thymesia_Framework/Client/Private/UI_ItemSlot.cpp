#include "stdafx.h"
#include "UI_ItemSlot.h"
#include "CustomUI.h"
#include "UI_Utils.h"
#include "GameManager.h"

GAMECLASS_C(CUI_ItemSlot)
CLONE_C(CUI_ItemSlot, CGameObject)

HRESULT CUI_ItemSlot::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_UI;

	Create_ItemSlot();

	return S_OK;
}

HRESULT CUI_ItemSlot::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_ItemSlot::Tick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();
	__super::Tick(fTimeDelta);


}

void CUI_ItemSlot::LateTick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();
	__super::LateTick(fTimeDelta);

}

void CUI_ItemSlot::Set_UIPosition(const _float fX, const _float fY, UI_ALIGN_TYPE eType)
{
	__super::Set_UIPosition(fX, fY, eType);

	m_pMain.lock()->Set_UIPosition(fX, fY, eType);
	m_pFrame.lock()->Set_UIPosition(fX, fY, eType);
	m_pHover.lock()->Set_UIPosition(fX, fY, eType);

}

void CUI_ItemSlot::Create_ItemSlot()
{
	m_pMain = ADD_STATIC_CUSTOMUI;
	m_pMain.lock()->Set_Size(66.f, 66.f);
	m_pMain.lock()->Set_Texture("ItemSlot_Main");
	m_pMain.lock()->Set_Depth(0.5);


	m_pFrame = ADD_STATIC_CUSTOMUI;
	m_pFrame.lock()->Set_Size(70.f, 70.f);
	m_pFrame.lock()->Set_Texture("ItemSlot_Frame");
	m_pFrame.lock()->Set_Depth(0.6);


	m_pHover = ADD_STATIC_CUSTOMUI;
	m_pHover.lock()->Set_Size(213.f, 213.f);
	m_pHover.lock()->Set_Texture("ItemSlot_Hover");
	m_pHover.lock()->Set_Depth(0.7);

	Add_Child(m_pMain);
	Add_Child(m_pFrame);
	Add_Child(m_pHover);
}

void CUI_ItemSlot::Create_TextInfo()
{
}

_bool CUI_ItemSlot::Check_IsInInventoryFrame()
{
	return _bool();
}
