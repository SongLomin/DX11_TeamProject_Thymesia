#include "stdafx.h"
#include "UI_ItemSlot.h"
#include "CustomUI.h"
#include "UI_Utils.h"
#include "GameManager.h"
#include "EasingComponent_Alpha.h"
#include "Preset_ItemData.h"
#include "Item.h"


GAMECLASS_C(CUI_ItemSlot)
CLONE_C(CUI_ItemSlot, CGameObject)

HRESULT CUI_ItemSlot::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_UI;
		
	Set_Size(70.f, 70.f);

	Create_ItemSlot();
	Create_TextInfo();
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

	m_tTextInfo.vPosition.y = m_tUIDesc.fY;
}

void CUI_ItemSlot::LateTick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();
	__super::LateTick(fTimeDelta);

	GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_tTextInfo);

}

void CUI_ItemSlot::Set_UIPosition(const _float fX, const _float fY, UI_ALIGN_TYPE eType)
{
	__super::Set_UIPosition(fX, fY, eType);

	m_pMain.lock()->Set_UIPosition(fX, fY, eType);
	m_pFrame.lock()->Set_UIPosition(fX, fY, eType);
	m_pHover.lock()->Set_UIPosition(fX, fY, eType);
	m_pIcon.lock()->Set_UIPosition(fX, fY, eType);
}

void CUI_ItemSlot::Set_OriginCenterPosFromThisPos()
{
	m_fOriginCenterPos.x = m_tUIDesc.fX;
	m_fOriginCenterPos.y = m_tUIDesc.fY;
}

void CUI_ItemSlot::Set_ScroolOffsetY(_float fOffsetY)
{
	m_fScrollOffsetY = fOffsetY;
	Set_UIPosition(m_fOriginCenterPos.x, m_fOriginCenterPos.y + m_fScrollOffsetY, CUI::ALIGN_CENTER);
}

void CUI_ItemSlot::Add_ScroolOffsetY(_float fOffsetY)
{
	m_fScrollOffsetY += fOffsetY;
	Set_UIPosition(m_fOriginCenterPos.x, m_fOriginCenterPos.y + m_fScrollOffsetY, CUI::ALIGN_CENTER);
}

void CUI_ItemSlot::ResetPos()
{
	Set_UIPosition(m_fOriginCenterPos.x, m_fOriginCenterPos.y, CUI::ALIGN_CENTER);
}

void CUI_ItemSlot::Bind_Item(weak_ptr<CItem> pItem)
{
	m_pBindedItem = pItem;

	CPreset_ItemData::SetUITextureFromItemName(m_pIcon, pItem.lock()->Get_Name());
	Update_TextInfo();
}

_bool CUI_ItemSlot::Is_Bind()
{
	if (m_pBindedItem.lock())
	{
		return true;
	}
	
	return false;
}

void CUI_ItemSlot::UnBind_Item()
{
	m_pIcon.lock()->Set_Texture("None");
	m_tTextInfo.szText = TEXT("");
	m_pBindedItem = weak_ptr<CItem>();
}

void CUI_ItemSlot::OnMouseOver()
{
	m_pHover.lock()->Get_Component<CEasingComponent_Alpha>().lock()->Set_Lerp(0.f, 1.f, 0.2f, EASING_TYPE::QUAD_IN, 
		CEasingComponent::ONCE, false);

	if (m_pBindedItem.lock())
	{
		Callback_OnMouseOver(m_pBindedItem);
	}
}

void CUI_ItemSlot::OnMouseOut()
{
	_float fAlphaColor = m_pHover.lock()->Get_AlphaColor();

	m_pHover.lock()->Get_Component<CEasingComponent_Alpha>().lock()->Set_Lerp(fAlphaColor, 0.f, 0.2f, EASING_TYPE::QUAD_OUT,
		CEasingComponent::ONCE, false);

	Callback_OnMouseOut();
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
	m_pHover.lock()->Set_AlphaColor(0.f);
	m_pHover.lock()->Add_Component<CEasingComponent_Alpha>();

	m_pIcon = ADD_STATIC_CUSTOMUI;
	m_pIcon.lock()->Set_Size(64.f, 64.f);
	m_pIcon.lock()->Set_Depth(0.4f);
	m_pIcon.lock()->Set_Texture("None");

	Add_Child(m_pMain);
	Add_Child(m_pFrame);
	Add_Child(m_pHover);
	Add_Child(m_pIcon);
}

void CUI_ItemSlot::Create_TextInfo()
{
	m_tTextInfo.eRenderGroup = RENDERGROUP::RENDER_UI;
	m_tTextInfo.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_tTextInfo.vScale = _float2(1.f, 1.f);
	m_tTextInfo.bAlways = false;
	m_tTextInfo.szText = TEXT("");
}

_bool CUI_ItemSlot::Check_IsInInventoryFrame()
{
	return _bool();
}

void CUI_ItemSlot::Update_TextInfo()
{
	m_tTextInfo.szText = to_wstring(m_pBindedItem.lock()->Get_CurrentQuantity());

	m_tTextInfo.vPosition.x = m_tUIDesc.fX - (max((unsigned long long)0, (m_tTextInfo.szText.size() - 1) ) * 10.f);
	m_tTextInfo.vPosition.y = m_tUIDesc.fY;

}
