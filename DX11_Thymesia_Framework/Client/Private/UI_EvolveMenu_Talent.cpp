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
// #include "UI_EvolveMenu_TalentButton.h"
#include "Talent_Effects.h"
#include "CorvusStates/Talent.h"

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

    m_fTreeOffsetY = 100.f;
    m_fTreeOffsetX = 50.f;




    Create_TalentInformation();





    Set_Enable(false);




    return S_OK;
}

HRESULT CUI_EveolveMenu_Talent::Start()
{
    __super::Start();

    UI_DESC tUIDesc;

    tUIDesc.fX = 192.f;
    tUIDesc.fY = 317.f;;
    tUIDesc.fSizeX = 40.f;
    tUIDesc.fSizeY = 40.f;
    tUIDesc.fDepth = 0.1f;

    SetUp_TalentNode(m_pRoot[0], tUIDesc);

    int a = 10;

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

void CUI_EveolveMenu_Talent::SetRootTalent(weak_ptr<CTalent> In_pTalent, TALENT_TAP eRootType)
{
    m_pRoot[(_uint)eRootType] = In_pTalent;
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
void CUI_EveolveMenu_Talent::Create_TalentInformation()
{
    m_pMediaFrame = ADD_STATIC_CUSTOMUI;
    
    m_pMediaFrame.lock()->Set_UIPosition
    (
        982.f,
        127.f,
        497.f,
        288.f,
        ALIGN_LEFTTOP
    );
    m_pMediaFrame.lock()->Set_Texture("MediaFrame");
    m_pMediaFrame.lock()->Set_Depth(0.3f);

    Add_Child(m_pMediaFrame);


    m_pTalentInformationBG = ADD_STATIC_CUSTOMUI;

    m_pTalentInformationBG.lock()->Set_UIPosition
    (
        982.f,
        478.f,
        497.f,
        327.f,
        ALIGN_LEFTTOP
    );
    m_pTalentInformationBG.lock()->Set_Texture("Talent_Information_BG");
    m_pTalentInformationBG.lock()->Set_Depth(0.3f);
    m_pTalentInformationBG.lock()->Set_AlphaColor(0.2f);
    Add_Child(m_pTalentInformationBG);

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
    __super::OnEnable(pArg);

    ShowCursor(true);
  

    if (!m_pFadeMask.lock())
        m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

    for (_uint i = 0; i < 1; i++)
    {
        if (m_pRoot[i].lock())
        {
            m_pRoot[i].lock()->Set_Enable(true);
        }
    }

}
void CUI_EveolveMenu_Talent::OnDisable()
{
    __super::OnDisable();

    ShowCursor(false);


    for (_uint i = 0; i < (_uint)TALENT_TAP::TALENT_SWORD; i++)
    {
        if (m_pRoot[i].lock())
        {
            m_pRoot[i].lock()->Set_Enable(false);
        }
    }

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


void CUI_EveolveMenu_Talent::SetUp_TalentNode(weak_ptr<CTalent> pNode, UI_DESC tUIDesc)
{
    pNode.lock()->Set_UIPosition(tUIDesc.fX, tUIDesc.fY);

    list<weak_ptr<CTalent>> ChildList = pNode.lock()->Get_Child();
    
    /*
       여기에 탤런트 정보 넣어야함.
    */
    if (ChildList.empty())
        return;

    if (ChildList.size() == 1)
    {
        tUIDesc.fY += m_fTreeOffsetY;
        SetUp_TalentNode(ChildList.front(), tUIDesc);
    }
    else
    {
        int i = 0;
        tUIDesc.fY += m_fTreeOffsetY;
        for (auto& elem : ChildList)
        {
            UI_DESC childDesc = tUIDesc;
            if (i % 2 == 0)
            {
                childDesc.fX += (((i + 1) * -1) * m_fTreeOffsetX);
            }
            else
            {
                childDesc.fX += ((i) * m_fTreeOffsetX);
            }
            i++;
            SetUp_TalentNode(elem, childDesc);
          
        }
    }
}

void CUI_EveolveMenu_Talent::Call_ReturnToEvolveMenu()
{
    Set_Enable(false);
    GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu>(LEVEL_STATIC).front().lock()->Set_Enable(true);
}

