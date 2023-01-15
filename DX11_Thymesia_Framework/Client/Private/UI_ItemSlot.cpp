#include "stdafx.h"
#include "UI_ItemSlot.h"
#include "CustomUI.h"
#include "UI_Utils.h"
#include "GameManager.h"
#include "EasingComponent_Alpha.h"
#include "Preset_ItemData.h"
#include "Item.h"
#include "EasingComponent_Transform.h"
#include "UIManager.h"

GAMECLASS_C(CUI_ItemSlot)
CLONE_C(CUI_ItemSlot, CGameObject)

HRESULT CUI_ItemSlot::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_UI;
		
	Set_Size(70.f, 70.f);

	Create_ItemSlot();
	Create_TextInfo();
	SetUp_Component();


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


	if (m_bRenderIcon)//아이콘이 없을 때는 버튼처리도 무시함.
	{
		__super::Tick(fTimeDelta);
	}
	m_tTextInfo.vPosition.y = m_tUIDesc.fY;

	if (m_pEasingTransform.lock()->Is_Lerping())
	{
		_float2	LerpedPos = m_pEasingTransform.lock()->Get_Lerp();
		
		Set_UIPosition(LerpedPos.x, LerpedPos.y);
	}
	m_pIcon.lock()->Set_Render(m_bRenderIcon);
}

void CUI_ItemSlot::LateTick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();
	__super::LateTick(fTimeDelta);

	if (m_bRenderIcon == false)
		return;
	
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

void CUI_ItemSlot::Set_ScrollOffsetY(_float fOffsetY)
{
	m_fScrollOffsetY = fOffsetY;
	Set_UIPosition(m_fOriginCenterPos.x, m_fOriginCenterPos.y + m_fScrollOffsetY, CUI::ALIGN_CENTER);
}

void CUI_ItemSlot::Add_ScrollOffsetY(_float fOffsetY)
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

void CUI_ItemSlot::Set_RenderGroup(RENDERGROUP eRenderGroup)
{
	__super::Set_RenderGroup(eRenderGroup);


	for (auto& elem : m_vecChildUI)
	{
		elem.lock()->Set_RenderGroup(eRenderGroup);
	}
	m_tTextInfo.eRenderGroup = m_eRenderGroup;
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

void CUI_ItemSlot::OffMouseHover()
{
	m_pHover.lock()->Get_Component<CEasingComponent_Alpha>().lock()->Stop();
	m_pHover.lock()->Set_AlphaColor(0.f);
}




void CUI_ItemSlot::Lerp_Transform(_float2 vTargetPos, _float fLerpTime)
{
	_float2 vMyPos;

	vMyPos.x = m_tUIDesc.fX;
	vMyPos.y = m_tUIDesc.fY;

	m_pEasingTransform.lock()->Set_Lerp(vMyPos, vTargetPos, fLerpTime, EASING_TYPE::QUAD_IN,
		CEasingComponent::ONCE, true);


}

void CUI_ItemSlot::Create_ItemSlot()
{
	m_pIcon = ADD_STATIC_CUSTOMUI;
	m_pIcon.lock()->Set_Size(64.f, 64.f);
	m_pIcon.lock()->Set_Depth(0.30f);
	m_pIcon.lock()->Set_Texture("None");

	m_pMain = ADD_STATIC_CUSTOMUI;
	m_pMain.lock()->Set_Size(66.f, 66.f);
	m_pMain.lock()->Set_Texture("ItemSlot_Main");
	m_pMain.lock()->Set_Depth(0.31f);


	m_pFrame = ADD_STATIC_CUSTOMUI;
	m_pFrame.lock()->Set_Size(70.f, 70.f);
	m_pFrame.lock()->Set_Texture("ItemSlot_Frame");
	m_pFrame.lock()->Set_Depth(0.32f);

	m_pHover = ADD_STATIC_CUSTOMUI;
	m_pHover.lock()->Set_Size(213.f, 213.f);
	m_pHover.lock()->Set_Texture("ItemSlot_Hover");
	m_pHover.lock()->Set_Depth(0.33f);
	m_pHover.lock()->Set_AlphaColor(0.f);
	m_pHover.lock()->Add_Component<CEasingComponent_Alpha>();



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

void CUI_ItemSlot::SetUp_Component()
{
	m_pEasingTransform = Add_Component<CEasingComponent_Transform>();
}
