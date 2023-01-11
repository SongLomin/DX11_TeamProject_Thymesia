#include "stdafx.h"
#include "UI_OptionElement_Slider.h"
#include "CustomUI.h"
#include "UI_Utils.h"
#include "UI_Slider.h"
#include "UI.h"

GAMECLASS_C(CUI_OptionElement_Slider)
CLONE_C(CUI_OptionElement_Slider, CGameObject)


HRESULT CUI_OptionElement_Slider::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    SetUp_ArrowPos();

    m_pSlider = GAMEINSTANCE->Add_GameObject<CUI_Slider>(LEVEL_STATIC);

    m_pSlider.lock()->Callback_ChangeSliderAmount += bind(&CUI_OptionElement_Slider::Call_ChangeSliderAmount, this, placeholders::_1);

    m_iAmount = 50;

    m_tAmountInfo.bAlways = false;
    m_tAmountInfo.bCenterAlign = true;
    m_tAmountInfo.eRenderGroup = RENDERGROUP::RENDER_UI;
    m_tAmountInfo.fRotation = 0.f;
    m_tAmountInfo.szText = L"";
    m_tAmountInfo.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tAmountInfo.vPosition = _float2(304.f, 229.f);
    m_tAmountInfo.vScale = _float2(0.6f, 0.6f);

    Add_Child(m_pSlider.lock()->Get_This());
    return S_OK;
}

HRESULT CUI_OptionElement_Slider::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_OptionElement_Slider::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

}

void CUI_OptionElement_Slider::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

    m_tAmountInfo.szText = to_wstring(m_iAmount);
    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_tAmountInfo);
}

void CUI_OptionElement_Slider::Call_OnClickLeftArrow()
{
    m_pSlider.lock()->DecreaseAmount(1);

    ClickArrow();
}

void CUI_OptionElement_Slider::Call_OnClickRightArrow()
{
    m_pSlider.lock()->IncreaseAmount(1);

    ClickArrow();
}

void CUI_OptionElement_Slider::Set_Y(_float fY)
{
    __super::Set_Y(fY);

    m_tAmountInfo.vPosition.y = fY - 10;
    //ÆùÆ® Desc Æ÷Áö¼Ç Y ¸ÂÃçÁÜ.
}

void CUI_OptionElement_Slider::Call_ChangeSliderAmount(_int iAmount)
{
    m_iAmount = iAmount;

    Callback_OnChangeOption(iAmount);
}

void CUI_OptionElement_Slider::SetUp_ArrowPos()
{
    m_pLeftArrow.lock()->Set_UIPosition
    (
        327.f,
        392.f,
        20.f,
        50.f,
        ALIGN_LEFTTOP
    );

    m_pRightArrow.lock()->Set_UIPosition
    (
        610.f,
        392.f,
        20.f,
        50.f,
        ALIGN_LEFTTOP
    );
}

void CUI_OptionElement_Slider::ClickArrow()
{
    
    Callback_OnChangeOption(m_iAmount);
}

void CUI_OptionElement_Slider::Free()
{

}
