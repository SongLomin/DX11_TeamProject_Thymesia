#include "stdafx.h"
#include "UI_OptionItem.h"
#include "CustomUI.h"
#include "UIManager.h"
#include "UI_Utils.h"
#include "UI_OptionElementBase.h"
#include "UI_OptionElement_OnOff.h"
#include "UI_OptionElement_Quality.h"
#include "UI_OptionElement_Slider.h"


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
    
    for (auto& elem : m_vecChildUI)
    {
        elem.lock()->Set_Y(m_fOriginCenterPos.y + m_fScrollOffsetY);
    }
}

void CUI_OptionItem::Add_ScrollOffsetY(_float fOffsetY)
{
    m_fScrollOffsetY += fOffsetY;
    Set_UIPosition(m_fOriginCenterPos.x, m_fOriginCenterPos.y + m_fScrollOffsetY, CUI::ALIGN_CENTER);

    for(auto& elem : m_vecChildUI)
    {
        elem.lock()->Set_Y(m_fOriginCenterPos.y + m_fScrollOffsetY);
    }

}

void CUI_OptionItem::ResetPos()
{
    Set_UIPosition(m_fOriginCenterPos.x, m_fOriginCenterPos.y, CUI::ALIGN_CENTER);
}

void CUI_OptionItem::Create_Element(OPTION_TYPE eOptionType)
{
    m_eOptionType = eOptionType;
  

    switch (m_eOptionType)
    {
    case Client::OPTION_TYPE::SOUND_TYPE_BG:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_Slider>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::SOUND_TYPE_EFFECT:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_Slider>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::SSAA:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_OnOff>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::AMBIENT_OCCLUSION:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_OnOff>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::NVIDIA_IMAGE_SCALING:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_OnOff>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::SCALING_SHARPNESS:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_Slider>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::WATER_REFLECTION_QUALITY:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_Quality>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::PBR:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_OnOff>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::MOTION_BLUR:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_OnOff>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::GODRAY:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_OnOff>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::CHROMATIC_ABRRATION:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_OnOff>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::RADIAL_BLUR:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_OnOff>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::DYNAMIC_SHADOW:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_OnOff>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::STATIC_SHADOW:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_OnOff>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::BRIGHTLESS:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_Slider>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::SATURATION:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_Slider>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::CONTRAST:
        m_pElement = GAMEINSTANCE->Add_GameObject<CUI_OptionElement_Slider>(LEVEL_STATIC);
        break;
    case Client::OPTION_TYPE::OPTION_TYPE_END:
        break;
    default:
        break;
    }

    if (m_pElement.lock())
    {
        m_pElement.lock()->Set_Y(m_tUIDesc.fY);

        m_pElement.lock()->Callback_OnChangeOption += bind(&CUI_OptionItem::Call_OnChangeOption, this, placeholders::_1);

        Add_Child(m_pElement);
    }
    m_pOptionName = ADD_STATIC_CUSTOMUI;
    if (m_pOptionName.lock())
    {
        m_pOptionName.lock()->Set_UIPosition(77.f, 223.f, 303.f, 35.f, ALIGN_LEFTTOP);
        m_pOptionName.lock()->Set_Depth(0.6f);
        m_pOptionName.lock()->Set_Y(m_tUIDesc.fY);
        m_pOptionName.lock()->Set_Texture("EvolveMenu_Option_TypeText");
        m_pOptionName.lock()->Set_DeffuseIndex((_uint)eOptionType);
        Add_Child(m_pOptionName);
    }
}

void CUI_OptionItem::Call_OnChangeOption(_int iAmount)
{
    if (m_iAmount == iAmount)
        return;

    m_iAmount = iAmount;
    Callback_OnChangeOption(m_eOptionType, (_uint)iAmount);
}

void CUI_OptionItem::OnMouseOver()
{
    Set_Texture("EvolveMenu_Option_Select");
}

void CUI_OptionItem::OnMouseOut()
{
    Set_Texture("EvolveMenu_Option_Defualt");
}
