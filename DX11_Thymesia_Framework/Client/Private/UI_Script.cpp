#include "stdafx.h"
#include "UI_Script.h"
#include "UI_Elements.h"
#include "EasingComponent_Alpha.h"

GAMECLASS_C(CUI_Script)
CLONE_C(CUI_Script, CGameObject)


HRESULT CUI_Script::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pEasingAlphaCom = Add_Component<CEasingComponent_Alpha>();
    
    m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;
    m_fLifeTime = 0.f;
    m_fLifeTick = 0.f;
    m_bLerp = true;


    Set_Enable(false);


    m_eScriptPlaying = SCRIPTPLAYINGTYPE::SCRIPT_END;
    return S_OK;
}

void CUI_Script::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    
    if (m_eScriptPlaying == CUI_Script::SCRIPTPLAYINGTYPE::SCRIPT_PLAYING)
    {
        if (m_fLifeTick >= m_fLifeTime)
        {
            if (m_bLerp)//서서히 사라지게
            {
                m_pEasingAlphaCom.lock()->Set_Lerp(1.f, 0.f, 1.f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);
                m_pEasingAlphaCom.lock()->Callback_LerpEnd += bind(
                    &CUI_Script::Call_LerpEnd_FadeOut, this);
                m_eScriptPlaying = CUI_Script::SCRIPTPLAYINGTYPE::SCRIPT_FADEOUT;
            }
            else
            {
                Call_LerpEnd_FadeOut();
            }
        }
        m_fLifeTick += fTimeDelta;
    }
}
void CUI_Script::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
  
}

void CUI_Script::ReadyScript(SCRIPTDESC tScriptDesc)
{
    /*
    if (!m_pScriptImage.lock())
    {
        m_pScriptImage = ADD_STATIC_CUSTOMUI;
        m_vecChildUI.push_back(m_pScriptImage);
    }
    */
    Set_Enable(true);
    Set_Texture(tScriptDesc.strScriptKey.c_str());
    Set_UIPosition
    (
        tScriptDesc.fPos.x,
        tScriptDesc.fPos.y,
        tScriptDesc.fSize.x,
        tScriptDesc.fSize.y
    );
   Set_Depth(0.f);
    m_fLifeTick = 0.f;
    m_fLifeTime = tScriptDesc.fLifeTime;
    m_bLerp = tScriptDesc.bLerp;
    if (m_bLerp)
    {
        m_pEasingAlphaCom.lock()->Set_Lerp(0.f, 1.f, 1.f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);
        m_pEasingAlphaCom.lock()->Callback_LerpEnd += bind(
            &CUI_Script::Call_LerpEnd_FadeIn, this);
    }
    else
        m_eScriptPlaying = CUI_Script::SCRIPTPLAYINGTYPE::SCRIPT_PLAYING;
}

void CUI_Script::Call_LerpEnd_FadeIn()
{
    m_eScriptPlaying = CUI_Script::SCRIPTPLAYINGTYPE::SCRIPT_PLAYING;
}

void CUI_Script::Call_LerpEnd_FadeOut()
{
    m_fLifeTick = 0.f;
    m_fLifeTime = 0.f;
    m_eScriptPlaying = SCRIPTPLAYINGTYPE::SCRIPT_END;
    Set_Enable(false);
}

