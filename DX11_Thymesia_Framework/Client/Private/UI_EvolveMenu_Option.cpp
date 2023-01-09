#include "stdafx.h"
#include "UI_EvolveMenu_Option.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "Texture.h"
#include "UI_Scroll.h"
#include "EasingComponent_Alpha.h"
#include "UIManager.h"
#include "UI_OptionItem.h"

GAMECLASS_C(CUI_EvolveMenu_Option)
CLONE_C(CUI_EvolveMenu_Option, CGameObject)


HRESULT CUI_EvolveMenu_Option::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_eRenderGroup = RENDERGROUP::RENDER_UI;

    m_fOptionItemOffsetY = 85.f;

    Init_UI();

    Init_OptionItem();



    m_pEasingAlpha = Add_Component<CEasingComponent_Alpha>();

    Set_Enable(false);

    return S_OK;
}

HRESULT CUI_EvolveMenu_Option::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EvolveMenu_Option::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();
    __super::Tick(fTimeDelta);

}

void CUI_EvolveMenu_Option::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);
}

void CUI_EvolveMenu_Option::Call_OnChangeOption(OPTION_TYPE eOption, _uint iAmount)
{
    Call_OnChangeOption(eOption, iAmount);
}

void CUI_EvolveMenu_Option::Call_OnWheelMove(_float fScrollOffsetY)
{
    for (auto& elem : m_vecOptionItem)
    {
        elem.lock()->Set_ScrollOffsetY(-fScrollOffsetY);
    }
}

void CUI_EvolveMenu_Option::Init_UI()
{
    Set_UIPosition(26.f, 51.f, 686.f, 769.f, ALIGN_LEFTTOP);

    m_pBackground_Head = ADD_STATIC_CUSTOMUI;
    
    m_pBackground_Head.lock()->Set_UIPosition
    (
        24.f,
        0.f,
        693.f,
        200.f,
        ALIGN_LEFTTOP
    );

    m_pBackground_Head.lock()->Set_Texture("EvolveMenu_Option_BGHead");
    m_pBackground_Head.lock()->Set_Depth(0.9f);
    m_pBackground_Head.lock()->Set_AlphaColor(1.f);

    
    m_pBackground_Body = ADD_STATIC_CUSTOMUI;
    m_pBackground_Body.lock()->Set_UIPosition
    (
        24.f,
        200.f,
        693.f,
        700.f,
        ALIGN_LEFTTOP
    );
    m_pBackground_Body.lock()->Set_Texture("EvolveMenu_Option_BGBody");
    m_pBackground_Body.lock()->Set_Depth(0.9f);
    m_pBackground_Body.lock()->Set_AlphaColor(1.f);

    m_pScroll = GAMEINSTANCE->Add_GameObject<CUI_Scroll>(LEVEL_STATIC);
    m_pScroll.lock()->SetUp_ScrollFromLeftTop(666.f, 206.f, 510.f, 510.f, 1360.f);

    m_pScroll.lock()->Callback_OnWheelMove += bind(&CUI_EvolveMenu_Option::Call_OnWheelMove, this, placeholders::_1);

    Add_Child(m_pScroll);
    Add_Child(m_pBackground_Head);
    Add_Child(m_pBackground_Body);

    //이 위로 올라가는 아이템이 가려지도록 헤드는 렌더그룹을 다르게 준다.
    m_pBackground_Head.lock()->Set_RenderGroup(RENDERGROUP::RENDER_AFTER_UI);

}

void CUI_EvolveMenu_Option::Init_OptionItem()
{
    weak_ptr<CUI_OptionItem> pItem;

    //Todo : Test For
    /*
        206 35
        241
        327
    */
    for (_uint i = 0; i < 5; i++)
    {
        pItem = GAMEINSTANCE->Add_GameObject<CUI_OptionItem>(LEVEL_STATIC);
     
        pItem.lock()->Set_UIPosition
        (
            350.f,
            241.f + (i * m_fOptionItemOffsetY),
            600.f,
            70.f
        );
        pItem.lock()->Set_OriginCenterPosFromThisPos();
        
        m_vecOptionItem.push_back(pItem);
        Add_Child(pItem);
    }
}

void CUI_EvolveMenu_Option::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);
    
    //m_pEasingAlpha.lock()->Set_Lerp_once(0.f, 1.f, 0.5f, EASING_TYPE::QUAD_IN, false);


    GET_SINGLE(CUIManager)->EnableCursor();
}

void CUI_EvolveMenu_Option::OnDisable()
{
    __super::OnDisable();

    GET_SINGLE(CUIManager)->DisableCursor();

}

void CUI_EvolveMenu_Option::Free()
{

}
