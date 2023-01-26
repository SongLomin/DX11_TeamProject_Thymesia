#include "stdafx.h"
#include "UI_InventoryConfirmWindowButton.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "Preset_UIDesc.h"
#include "EasingComponent_Alpha.h"

GAMECLASS_C(CUI_InventoryConfirmWindowButton)
CLONE_C(CUI_InventoryConfirmWindowButton, CGameObject)



HRESULT CUI_InventoryConfirmWindowButton::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_eButtonLevel = BUTTON_LEVEL::LEVEL2;
    m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;


    Create_UI();
    SetUp_Component();

    return S_OK;
}

HRESULT CUI_InventoryConfirmWindowButton::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_InventoryConfirmWindowButton::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

 
}

void CUI_InventoryConfirmWindowButton::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

}

void CUI_InventoryConfirmWindowButton::Create_UI()
{
    m_pFontImage = ADD_STATIC_CUSTOMUI;

    m_pFontImage.lock()->Set_Size(74.f, 23.f);
    m_pFontImage.lock()->Set_Depth(0.45f);
    m_pFontImage.lock()->Set_Texture("None");

    m_pBG = ADD_STATIC_CUSTOMUI;

    m_pBG.lock()->Set_Size(205.f, 27.f);
    m_pBG.lock()->Set_Depth(0.46f);
    m_pBG.lock()->Set_Texture("None");

    Add_Child(m_pFontImage);
    Add_Child(m_pBG);
}

void CUI_InventoryConfirmWindowButton::SetUp_Component()
{
    m_pEasingAlpha = Add_Component<CEasingComponent_Alpha>();
}
