#include "stdafx.h"
#include "UI_EffectGroup_SkillOpen.h"
#include "Texture.h"
#include "Shader.h"
#include "EasingComponent_Transform.h"
#include "EasingComponent_Alpha.h"
#include "EasingComponent_Bezier.h"
#include "EasingComponent_Float.h"
#include "UI_CustomEffect.h"
#include "UI_Utils.h"
#include "GameManager.h"
#include "UIManager.h"

GAMECLASS_C(CUI_EffectGroup_SkillOpen)
CLONE_C(CUI_EffectGroup_SkillOpen, CGameObject)

HRESULT CUI_EffectGroup_SkillOpen::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

    m_pEasingFloat = Add_Component<CEasingComponent_Float>();

    m_pEasingFloat.lock()->Callback_OnConditionEvent += bind(&CUI_EffectGroup_SkillOpen::Call_OnEasingFloatConditionEvent,
        this, placeholders::_1);

    return S_OK;
}

HRESULT CUI_EffectGroup_SkillOpen::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EffectGroup_SkillOpen::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);
}

void CUI_EffectGroup_SkillOpen::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);
}

void CUI_EffectGroup_SkillOpen::Init_EffectUI()
{
    m_pBackGround = GAMEINSTANCE->Add_GameObject<CUI_CustomEffect>(LEVEL_STATIC);

    m_pBackGround.lock()->Set_PassIndex(0);
    m_pBackGround.lock()->SetMaskIndex(0);
    m_pBackGround.lock()->Set_DeffuseIndex(0);
    m_pBackGround.lock()->Set_Size(1600.f, 900.f);
    m_pBackGround.lock()->Set_UIPosition(800.f, 450.f);
    m_pBackGround.lock()->Set_Texture("NoneBlack");
    m_pBackGround.lock()->Set_Depth(0.9f);
    m_pBackGround.lock()->Set_Enable(false);

    m_pTitleBG = GAMEINSTANCE->Add_GameObject<CUI_CustomEffect>(LEVEL_STATIC);

    m_pTitleBG.lock()->Set_PassIndex(0);
    m_pTitleBG.lock()->SetMaskIndex(0);
    m_pTitleBG.lock()->Set_DeffuseIndex(0);

    m_pTitleBG.lock()->Set_Size(1600.f, 110.f);
    m_pTitleBG.lock()->Set_UIPosition(800.f, 450.f);
    m_pTitleBG.lock()->Set_Texture("MainMenu_SelectableButton_2");
    m_pTitleBG.lock()->Set_Depth(0.8f);
    m_pTitleBG.lock()->Set_Enable(false);

    m_pTitleText = GAMEINSTANCE->Add_GameObject<CUI_CustomEffect>(LEVEL_STATIC);

    m_pTitleText.lock()->Set_PassIndex(0);
    m_pTitleText.lock()->SetMaskIndex(0);
    m_pTitleText.lock()->Set_Size(207, 44.f);
    m_pTitleText.lock()->Set_DeffuseIndex(0);
    m_pTitleText.lock()->Set_UIPosition(800.f, 420.f);
    m_pTitleText.lock()->Set_Texture("SkillOpen_TitleText");
    m_pTitleText.lock()->Set_Depth(0.7f);
    m_pTitleText.lock()->Set_Enable(false);

 
    m_pSkillName = GAMEINSTANCE->Add_GameObject<CUI_CustomEffect>(LEVEL_STATIC);

    m_pSkillName.lock()->Set_PassIndex(0);
    m_pSkillName.lock()->SetMaskIndex(0);
    m_pSkillName.lock()->Set_Size(983, 69.f);
    m_pSkillName.lock()->Set_UIPosition(800.f, 461.f);
    m_pSkillName.lock()->Set_Texture("SkillOpen_SkillName");
    m_pSkillName.lock()->Set_DeffuseIndex(0);
    m_pSkillName.lock()->Set_Depth(0.7f);
    m_pSkillName.lock()->Set_Enable(false);
  

    m_pStartCircleEffect = GAMEINSTANCE->Add_GameObject<CUI_CustomEffect>(LEVEL_STATIC);

    m_pStartCircleEffect.lock()->Set_PassIndex(1);
    m_pStartCircleEffect.lock()->SetMaskIndex(1);
    m_pStartCircleEffect.lock()->Set_Size(600, 600.f);
    m_pStartCircleEffect.lock()->Set_UIPosition(800.f, 450.f);
    m_pStartCircleEffect.lock()->Set_Texture("HighLight");
    m_pStartCircleEffect.lock()->Set_DeffuseIndex(0);
    m_pStartCircleEffect.lock()->Set_Depth(0.7f);
    m_pStartCircleEffect.lock()->Set_Enable(false);


}

void CUI_EffectGroup_SkillOpen::Play(SKILL_NAME eSkillName)
{
    Init_EffectUI();

    m_pBackGround.lock()->Set_Enable(true);
    m_pTitleBG.lock()->Set_Enable(true);
    m_pTitleText.lock()->Set_Enable(true);
    m_pStartCircleEffect.lock()->Set_Enable(true);


    m_pSkillName.lock()->Set_DeffuseIndex((_uint)eSkillName);



    m_pEasingFloat.lock()->Set_Lerp(0.f, 3.f, 3.f, EASING_TYPE::LINEAR, CEasingComponent::ONCE);

    m_pEasingFloat.lock()->Add_Condition(0.f, EASING_FLOAT_CONDITION::BIGGER, 0);
    m_pEasingFloat.lock()->Add_Condition(1.0f, EASING_FLOAT_CONDITION::BIGGER, 1);
    m_pEasingFloat.lock()->Add_Condition(2.5f, EASING_FLOAT_CONDITION::BIGGER, 2);
    m_pEasingFloat.lock()->Add_Condition(3.f, EASING_FLOAT_CONDITION::BIGGER, 2);


}

void CUI_EffectGroup_SkillOpen::Call_OnEasingFloatConditionEvent(_uint iEventNumber)
{
    if (iEventNumber == 0)
    {
        m_pBackGround.lock()->Set_LerpAlpha(0.f, 8.f, 0.1f, EASING_TYPE::LINEAR);
        m_pTitleBG.lock()->Set_AlphaColor(0.8f);


        _float2 fTileTextSize = m_pTitleText.lock()->Get_Size();
        _float2 fTitleTextStartSize = { 342.f, 71.f };

        m_pTitleText.lock()->Set_LerpAlpha(0.1f, 1.f, 0.5f, EASING_TYPE::LINEAR);
        m_pTitleText.lock()->Set_LerpSize(fTitleTextStartSize, fTileTextSize, 0.4f, EASING_TYPE::LINEAR);

        m_pStartCircleEffect.lock()->Set_LerpAlpha(1.f, 0.f, 0.5f, EASING_TYPE::SINE_OUT);
        m_pStartCircleEffect.lock()->Set_LerpSize(m_pStartCircleEffect.lock()->Get_Size(), _float2(1351.f, 1351.f), 0.4f, EASING_TYPE::SINE_OUT);

    }
    if (iEventNumber == 1)
    {
        GAMEINSTANCE->PlaySound2D("UnlockSkill.mp3", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_EFFECT));
        m_pSkillName.lock()->Set_Enable(true);
        m_pSkillName.lock()->Set_LerpAlpha(0.1f, 1.f, 0.2f, EASING_TYPE::QUAD_OUT);
        m_pSkillName.lock()->Set_LerpTransform(_float2(800.f, 420.f), _float2(800.f, 461.f), 0.2f, EASING_TYPE::QUAD_OUT);

      //  m_pTitleBG.lock()->Set_LerpTransform(_float2(800.f, 450.f), _float2(800.f, 330.f), 0.2, EASING_TYPE::LINEAR);
       // m_pTitleText.lock()->Set_LerpTransform(m_pTitleText.lock()->GetPos(), _float2(800.f, 300.f), 0.2, EASING_TYPE::LINEAR);

    }
    if (iEventNumber == 2)
    {
        m_pBackGround.lock()->Set_LerpAlpha(0.8f, 0.f, 0.3f, EASING_TYPE::LINEAR);
        m_pTitleBG.lock()->Set_LerpAlpha(0.8f, 0.f, 0.3f, EASING_TYPE::LINEAR);
        m_pTitleText.lock()->Set_LerpAlpha(0.8f, 0.f, 0.3f, EASING_TYPE::LINEAR);
        m_pSkillName.lock()->Set_LerpAlpha(0.8f, 0.f, 0.3f, EASING_TYPE::LINEAR);

    }

    if (iEventNumber == 3)
    {
        m_pBackGround.lock()->Set_Enable(false);
        m_pTitleBG.lock()->Set_Enable(false);
        m_pTitleText.lock()->Set_Enable(false);
        m_pStartCircleEffect.lock()->Set_Enable(false);
        m_pSkillName.lock()->Set_Enable(false);
        Set_Enable(false);
    }
}

void CUI_EffectGroup_SkillOpen::Free()
{
}
