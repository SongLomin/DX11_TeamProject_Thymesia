#include "UI_EvolveMenu_Talent.h"
#include "UI_Containers.h"
#include "UI_Elements.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "UI_EvolveMenu.h"
#include "EasingTransform.h"
#include "State_Player.h"
#include "Player.h"
#include "FadeMask.h"

GAMECLASS_C(CUI_EveolveMenu_Talent)
CLONE_C(CUI_EveolveMenu_Talent, CGameObject)

HRESULT CUI_EveolveMenu_Talent::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Create_Background();


    m_pTitle = ADD_STATIC_CUSTOMUI;
    m_pTitle.lock()->Set_UIPosition
    (
        64.f,
        65.f,
        72.f,
        49.f,
        ALIGN_LEFTTOP

    );
    m_pTitle.lock()->Set_Texture("EvolveMenu_Talent_Title");
    m_pTitle.lock()->Set_Depth(0.1f);
    Add_Child(m_pTitle);
    
    m_iTalentIndex = 0;

    Set_Enable(false);
    return S_OK;
}

HRESULT CUI_EveolveMenu_Talent::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EveolveMenu_Talent::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);


    if (KEY_INPUT(KEY::T, KEY_STATE::TAP))
    {
        FaderDesc tFaderDesc;
        tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
        tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
        tFaderDesc.fFadeMaxTime = 0.2f;
        tFaderDesc.fDelayTime = 0.f;
        tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

        m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
        m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_EveolveMenu_Talent::Call_ReturnToEvolveMenu, this);
    }

}

void CUI_EveolveMenu_Talent::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

void CUI_EveolveMenu_Talent::Create_Background()
{
    m_pPauseMenuBackground = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pPauseMenuBackground.lock()->Set_Texture("PauseMenu_Background0");
    m_pPauseMenuBackground.lock()->Set_Depth(0.9f);

    m_pPauseMenuBackground_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pPauseMenuBackground_Main.lock()->Set_Texture("PauseMenu_Background1");
    m_pPauseMenuBackground_Main.lock()->Set_Depth(0.8f);

    m_pPauseMenuBackground_Top = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pPauseMenuBackground_Top.lock()->Set_Texture("PauseMenu_Background3");
    m_pPauseMenuBackground_Top.lock()->Set_Depth(0.7f);

    m_vecChildUI.push_back(m_pPauseMenuBackground);
    m_vecChildUI.push_back(m_pPauseMenuBackground_Main);
    m_vecChildUI.push_back(m_pPauseMenuBackground_Top);
}
void CUI_EveolveMenu_Talent::Init_Tap()
{
//    m_pTap[(_uint)TALENT_TAP::TALENT_SWORD]

}
void CUI_EveolveMenu_Talent::Update_UI()
{


}



void CUI_EveolveMenu_Talent::OnEnable(void* pArg)
{
    if (!m_pFadeMask.lock())
        m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
}
void CUI_EveolveMenu_Talent::UI_ChangeTap()
{
    if (m_pTap[m_iTalentIndex].lock())
    {
        for (auto& elem : m_pTap)
        {
            if (elem.lock())
            {
                elem.lock()->Set_Enable(false);
            }
        }
        m_pTap[m_iTalentIndex].lock()->Set_Enable(true);
    }
}
void CUI_EveolveMenu_Talent::Call_ReturnToEvolveMenu()
{
    Set_Enable(false);
    GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu>(LEVEL_STATIC).front().lock()->Set_Enable(true);
}

