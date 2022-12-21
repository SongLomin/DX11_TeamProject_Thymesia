#include "stdafx.h"
#include "UI_ItemInformation.h"
#include "UI_Utils.h"
#include "GameManager.h"
#include "Item.h"
#include "UI_ItemSlot.h"
#include "Preset_UIDesc.h"


GAMECLASS_C(CUI_ItemInformation);
CLONE_C(CUI_ItemInformation, CGameObject)

HRESULT CUI_ItemInformation::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_ItemInformation::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

	CPreset_UIDesc::Set_CUI_ItemInformation(Weak_StaticCast<CUI_ItemInformation>(m_this));

	Add_Child(m_pItemTitle);
	Add_Child(m_pItemInformation);
	Add_Child(m_pItemType);
	Add_Child(m_pItemQuantity);
	Add_Child(m_pIcon);
	Add_Child(m_pItemTitleDecoration);
	Add_Child(m_pItemQuantityDecoration);
	Add_Child(m_pItemTypeDecoration);

	return S_OK;
}

HRESULT CUI_ItemInformation::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_ItemInformation::Tick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();

	__super::Tick(fTimeDelta);

	Update_Quantity();

}

void CUI_ItemInformation::LateTick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();

	__super::LateTick(fTimeDelta);

	if (m_pBindedItem.lock())
	{
		GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_tTextInfoQuantity);
	}

}

void CUI_ItemInformation::Update_Quantity()
{
	if (m_pBindedItem.lock())
	{
		m_tTextInfoQuantity.szText = to_wstring(m_pBindedItem.lock()->Get_CurrentQuantity());
		m_tTextInfoQuantity.szText += '/';
		m_tTextInfoQuantity.szText += to_wstring(m_pBindedItem.lock()->Get_MaxQuantity());
	}
}


void CUI_ItemInformation::Bind_Item(weak_ptr<CItem> pItem)
{
	m_pBindedItem = pItem;

	CPreset_UIDesc::Set_CUI_ItemInformaiton_BindItem(Weak_StaticCast<CUI_ItemInformation>(m_this), pItem);
}

void CUI_ItemInformation::UnBind_Item()
{
	m_pBindedItem = weak_ptr<CItem>();

	for (auto& elem : m_vecChildUI)
	{
		elem.lock()->Set_Texture("None");
	}
}

void CUI_ItemInformation::OnDisable()
{
	__super::OnDisable();

	UnBind_Item();

}
