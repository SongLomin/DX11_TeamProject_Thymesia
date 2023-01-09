#include "stdafx.h"
#include "UI_OptionItem.h"
#include "CustomUI.h"
#include "UIManager.h"
#include "UI_Utils.h"

GAMECLASS_C(CUI_OptionItem)
CLONE_C(CUI_OptionItem, CGameObject)


HRESULT CUI_OptionItem::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Set_Texture("EvolveMenu_Option_Defualt");
    Set_Size(600.f, 70.f);
    Set_Depth(0.7f);

    return S_OK;
}

HRESULT CUI_OptionItem::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_OptionItem::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

}

void CUI_OptionItem::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

}

void CUI_OptionItem::Set_OriginCenterPosFromThisPos()
{
    m_fOriginCenterPos.x = m_tUIDesc.fX;
    m_fOriginCenterPos.y = m_tUIDesc.fY;
}

void CUI_OptionItem::Set_ScrollOffsetY(_float fOffsetY)
{
    m_fScrollOffsetY = fOffsetY;
    Set_UIPosition(m_fOriginCenterPos.x, m_fOriginCenterPos.y + m_fScrollOffsetY, CUI::ALIGN_CENTER);
}

void CUI_OptionItem::Add_ScrollOffsetY(_float fOffsetY)
{
    m_fScrollOffsetY += fOffsetY;
    Set_UIPosition(m_fOriginCenterPos.x, m_fOriginCenterPos.y + m_fScrollOffsetY, CUI::ALIGN_CENTER);
}

void CUI_OptionItem::ResetPos()
{
    Set_UIPosition(m_fOriginCenterPos.x, m_fOriginCenterPos.y, CUI::ALIGN_CENTER);
}

void CUI_OptionItem::OnMouseOver()
{
    Set_Texture("EvolveMenu_Option_Select");

}

void CUI_OptionItem::OnMouseOut()
{
    Set_Texture("EvolveMenu_Option_Defualt");
}
