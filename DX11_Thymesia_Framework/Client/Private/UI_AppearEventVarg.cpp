#include "stdafx.h"
#include "UI_AppearEventVarg.h"
#include "CustomUI.h"
#include "EasingComponent_Alpha.h"
#include "EasingComponent_Float.h"
#include "EasingComponent_Transform.h"
#include "UI_Utils.h"


GAMECLASS_C(CUI_AppearEventVarg)
CLONE_C(CUI_AppearEventVarg, CGameObject)



HRESULT CUI_AppearEventVarg::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_AppearEventVarg::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    
    m_eRenderGroup = RENDERGROUP::RENDER_UI;

    CreateUI();

    SetUp_Component();

    Set_Enable(false);
    return S_OK;
}

HRESULT CUI_AppearEventVarg::Start()
{
    __super::Start();
    






    return S_OK;
}

void CUI_AppearEventVarg::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

}

void CUI_AppearEventVarg::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

}

void CUI_AppearEventVarg::Start_Event()
{
    Set_Enable(true);

    Reset();

    weak_ptr<CEasingComponent_Transform> pSliceTopTransform = m_pSliceTop.lock()->Get_Component<CEasingComponent_Transform>();

    _float2 fSliceTopPos = m_pSliceTop.lock()->GetPos();

    pSliceTopTransform.lock()->Set_Lerp(fSliceTopPos, _float2((g_iWinCX>> 1) +   143.5f, (g_iWinCY >> 1)  + 83.f), 1.f, EASING_TYPE::QUART_OUT,
        CEasingComponent::ONCE, false, false);

    pSliceTopTransform.lock()->Callback_LerpEnd
        += bind(&CUI_AppearEventVarg::Call_SliceEnd, this);

}

void CUI_AppearEventVarg::CreateUI()
{
    m_pSliceTop = ADD_STATIC_CUSTOMUI;
    m_pSliceTop.lock()->Set_Texture("Varg_Appear_SliceTop");
    m_pSliceTop.lock()->Set_Depth(0.3f);
    m_pSliceTop.lock()->Add_Component<CEasingComponent_Transform>();
    m_pSliceTop.lock()->Add_Component<CEasingComponent_Alpha>();


    m_pSliceBottom = ADD_STATIC_CUSTOMUI;
    m_pSliceBottom.lock()->Set_Texture("Varg_Appear_SliceBottom");
    m_pSliceBottom.lock()->Set_Depth(0.3f);
    m_pSliceBottom.lock()->Add_Component<CEasingComponent_Transform>();
    m_pSliceBottom.lock()->Add_Component<CEasingComponent_Alpha>();



    m_pSliceTopBG = ADD_STATIC_CUSTOMUI;
    m_pSliceTopBG.lock()->Set_Texture("Varg_Appear_SliceTop_BG");
    m_pSliceTopBG.lock()->Set_Depth(0.35f);
    m_pSliceTopBG.lock()->Add_Component<CEasingComponent_Transform>();
    m_pSliceTopBG.lock()->Add_Component<CEasingComponent_Alpha>();


    m_pSliceBottomBG = ADD_STATIC_CUSTOMUI;
    m_pSliceBottomBG.lock()->Set_Texture("Varg_Appear_SliceBottom_BG");
    m_pSliceBottomBG.lock()->Set_Depth(0.35f);
    m_pSliceBottomBG.lock()->Add_Component<CEasingComponent_Transform>();
    m_pSliceBottomBG.lock()->Add_Component<CEasingComponent_Alpha>();
     

    Add_Child(m_pSliceTop);
    Add_Child(m_pSliceBottom);

    Add_Child(m_pSliceTopBG);
    Add_Child(m_pSliceBottomBG);

}

void CUI_AppearEventVarg::SetUp_Component()
{
    
}

void CUI_AppearEventVarg::Reset()
{
    for (auto& elem : m_vecChildUI)
    {
        elem.lock()->Set_UIPosition(800.f, 450.f);
        elem.lock()->Set_AlphaColor(1.f);
        elem.lock()->Get_Component<CEasingComponent_Transform>().lock();
        elem.lock()->Get_Component<CEasingComponent_Alpha>().reset();
    }
}

void CUI_AppearEventVarg::Call_SliceEnd()
{
      weak_ptr<CEasingComponent_Transform> pSliceTopTransform = m_pSliceTop.lock()->Get_Component<CEasingComponent_Transform>();
      weak_ptr<CEasingComponent_Transform> pSliceBottomTransform = m_pSliceBottom.lock()->Get_Component<CEasingComponent_Transform>();
      weak_ptr<CEasingComponent_Transform> pSliceTopBGTransform = m_pSliceTopBG.lock()->Get_Component<CEasingComponent_Transform>();
      weak_ptr<CEasingComponent_Transform> pSliceBottomBGTransform = m_pSliceBottomBG.lock()->Get_Component<CEasingComponent_Transform>();





      _float2   fSliceTopPos = m_pSliceTop.lock()->GetPos();
      _float2   fSliceBottomPos = m_pSliceBottom.lock()->GetPos();


      _float2   fSliceTopBGPos = m_pSliceTopBG.lock()->GetPos();
      _float2   fSliceBottomBGPos = m_pSliceBottomBG.lock()->GetPos();




      pSliceTopTransform.lock()->Set_Lerp(fSliceTopPos, _float2(fSliceTopPos.x + 800, fSliceTopPos.y), 1.f, EASING_TYPE::QUART_OUT,
          CEasingComponent::ONCE, false, false);
      m_pSliceTop.lock()->Get_Component<CEasingComponent_Alpha>().lock()->Set_Lerp(1.f, 0.f, 1.f, EASING_TYPE::QUAD_OUT,
          CEasingComponent::ONCE, false);

      pSliceBottomTransform.lock()->Set_Lerp(fSliceBottomPos, _float2(fSliceBottomPos.x - 800, fSliceBottomPos.y), 1.f, EASING_TYPE::QUART_OUT,
          CEasingComponent::ONCE, false, false);
      m_pSliceBottom.lock()->Get_Component<CEasingComponent_Alpha>().lock()->Set_Lerp(1.f, 0.f, 1.f, EASING_TYPE::QUAD_OUT,
          CEasingComponent::ONCE, false);


      pSliceTopBGTransform.lock()->Set_Lerp(fSliceTopPos, _float2(fSliceTopPos.x + 800, fSliceTopPos.y), 1.f, EASING_TYPE::QUART_OUT,
          CEasingComponent::ONCE, false, false);
      m_pSliceTopBG.lock()->Get_Component<CEasingComponent_Alpha>().lock()->Set_Lerp(1.f, 0.f, 1.f, EASING_TYPE::QUAD_OUT,
          CEasingComponent::ONCE, false);

      pSliceBottomBGTransform.lock()->Set_Lerp(fSliceBottomPos, _float2(fSliceBottomPos.x - 800, fSliceBottomPos.y), 1.f, EASING_TYPE::QUART_OUT,
          CEasingComponent::ONCE, false, false);
      m_pSliceBottomBG.lock()->Get_Component<CEasingComponent_Alpha>().lock()->Set_Lerp(1.f, 0.f, 1.f, EASING_TYPE::QUAD_OUT,
          CEasingComponent::ONCE, false);

}
