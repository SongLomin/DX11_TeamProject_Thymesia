#include "stdafx.h"
#include "UI_EvolveMenu_Talent.h"
#include "UI_Containers.h"
#include "UI_Elements.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "UI_EvolveMenu.h"
#include "State_Player.h"
#include "Player.h"
#include "FadeMask.h"
#include "Talent_Effects.h"
#include "CorvusStates/Talent.h"
#include "UI_EvolveTalent_Active.h"
#include "UI_Utils.h"
#include "UIManager.h"


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

    _float      fOffsetX = 150.f;

    for (_uint i = 0; i < m_pRootList[m_iTapIndex].size(); i++)
    {
        tUIDesc.fX += fOffsetX * i;
        SetUp_TalentNode(m_pRootList[m_iTapIndex][i], tUIDesc);
    }

    return S_OK;
}

void CUI_EveolveMenu_Talent::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CUI_EveolveMenu_Talent::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

    Update_UI();
}

void CUI_EveolveMenu_Talent::SetRootTalent(weak_ptr<CTalent> In_pTalent, TALENT_TAP eRootType)
{
    m_pRootList[(_uint)eRootType].push_back(In_pTalent);
}

void CUI_EveolveMenu_Talent::Create_Background()
{
    m_pPauseMenuBackground = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pPauseMenuBackground.lock()->Set_Texture("PauseMenu_Background0");
    m_pPauseMenuBackground.lock()->Set_Depth(0.9f);

    m_pPauseMenuBackground_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pPauseMenuBackground_Main.lock()->Set_Texture("PauseMenu_Background1");
    m_pPauseMenuBackground_Main.lock()->Set_Depth(0.8f);

    m_pPauseMenuBackground_Main2 = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pPauseMenuBackground_Main2.lock()->Set_Texture("PauseMenu_Background2");
    m_pPauseMenuBackground_Main2.lock()->Set_Depth(0.75f);

    m_pPauseMenuBackground_Top = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pPauseMenuBackground_Top.lock()->Set_Texture("PauseMenu_Background3");
    m_pPauseMenuBackground_Top.lock()->Set_Depth(0.7f);

    m_vecChildUI.push_back(m_pPauseMenuBackground);
    m_vecChildUI.push_back(m_pPauseMenuBackground_Main);
    m_vecChildUI.push_back(m_pPauseMenuBackground_Main2);
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

    m_pTalentImage = ADD_STATIC_CUSTOMUI;
    m_pTalentImage.lock()->Set_UIPosition
    (
        992.f,
        136.f,
        479.f,
        268.f,
        ALIGN_LEFTTOP
    );
    m_pTalentImage.lock()->Set_Texture("UVMask");
    m_pTalentImage.lock()->Set_DeffuseIndex(703);
    m_pTalentImage.lock()->Set_PassIndex(14);
    m_pTalentImage.lock()->Set_Depth(0.1f);
    Add_Child(m_pTalentImage);


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


    m_pTalentTitle = ADD_STATIC_CUSTOMUI;
    m_pTalentTitle.lock()->Set_UIPosition
    (
        982.f,
        428.f,
        497.f,
        50.f,
        ALIGN_LEFTTOP
    );
    m_pTalentTitle.lock()->Set_Texture("None");
    m_pTalentImage.lock()->Set_Depth(0.1f);

    Add_Child(m_pTalentImage);

    m_pTalentInformation = ADD_STATIC_CUSTOMUI;
    m_pTalentInformation.lock()->Set_UIPosition
    (
        1024.f,
        496.f,
        425.f,
        257.f,
        ALIGN_LEFTTOP
    );
    m_pTalentInformation.lock()->Set_Texture("None");
    m_pTalentInformation.lock()->Set_Depth(0.1f);

    Add_Child(m_pTalentInformation);

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
  
    GET_SINGLE(CUIManager)->EnableCursor();

    if (!m_pFadeMask.lock())
        m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

    for (_uint i = 0; i < m_pRootList[m_iTalentIndex].size(); i++)
    {
        if (m_pRootList[m_iTalentIndex][i].lock())
        {
            m_pRootList[m_iTalentIndex][i].lock()->Set_Enable(true);
            m_pRootList[m_iTalentIndex][i].lock()->OnActive();//루트들은 기본적으로 켜져 있다.
        }
    }

}
void CUI_EveolveMenu_Talent::OnDisable()
{
    __super::OnDisable();

    GET_SINGLE(CUIManager)->DisableCursor();

    for (_uint i = 0; i < m_pRootList[m_iTalentIndex].size(); i++)
    {
        if (m_pRootList[m_iTalentIndex][i].lock())
        {
            m_pRootList[m_iTalentIndex][i].lock()->Set_Enable(false);
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


void CUI_EveolveMenu_Talent::TalentAnimation_MouseOver(weak_ptr<CTalent> pSelectedTalent)
{
    if (pSelectedTalent.lock()->Is_Active())
    {
        pSelectedTalent.lock()->CheckMouseOver();
    }
    else
    { 
        list<weak_ptr<CTalent>> listParentNode;
        pSelectedTalent.lock()->Find_AllParent_Recursive(pSelectedTalent, listParentNode);

        for (auto& elem : listParentNode)
        {
            elem.lock()->CheckMouseOver();
        }
    }
    
}

void CUI_EveolveMenu_Talent::TalentAnimation_MouseOut(weak_ptr<CTalent> pSelectedTalent)
{
    
    pSelectedTalent.lock()->CheckMouseOut();
    
    list<weak_ptr<CTalent>> listParentNode;
    pSelectedTalent.lock()->Find_AllParent_Recursive(pSelectedTalent, listParentNode);

    for (auto& elem : listParentNode)
    {
        elem.lock()->CheckMouseOut();
    }

}

void CUI_EveolveMenu_Talent::TalentViewInformaiton_MouseOver(TALENT_NAME eTalentName)
{
    switch (eTalentName)
    {
    case Client::TALENT_NAME::NORSWORDLV1:
        m_pTalentTitle.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Basic0_Title");
        m_pTalentInformation.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Basic0_Information");
        break;
    case Client::TALENT_NAME::NORSWORDLV2:
        m_pTalentTitle.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Basic1_Title");
        m_pTalentInformation.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Basic1_Information");
        break;
    case Client::TALENT_NAME::AVOIDSLASHLV1:
        m_pTalentTitle.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Slash0_Title");
        m_pTalentInformation.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Slash0_Information");

        break;
    case Client::TALENT_NAME::AVOIDSLASHLV2:
        m_pTalentTitle.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Slash1_Title");
        m_pTalentInformation.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Slash1_Information");

        break;
    case Client::TALENT_NAME::AVOIDTHRUSTLV1:
        m_pTalentTitle.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Stab0_Title");
        m_pTalentInformation.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Stab0_Information");

        break;
    case Client::TALENT_NAME::AVOIDTHRUSTLV2:
        m_pTalentTitle.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Stab1_Title");
        m_pTalentInformation.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Stab1_Information");
        break;
    case Client::TALENT_NAME::JUMPATTACKLV1:
        break;
    case Client::TALENT_NAME::JUMPATTACKLV2:
        break;
    case Client::TALENT_NAME::JUMPATTACKLV3:
        break;
    case Client::TALENT_NAME::EXECUTION:
        m_pTalentTitle.lock()->Set_Texture("EvolveMenu_Talent_Icon_Execution_Title");
        m_pTalentInformation.lock()->Set_Texture("EvolveMenu_Talent_Icon_Execution_Information");
        break;
    case Client::TALENT_NAME::HEALINGEXECUTIONLV1:
        m_pTalentTitle.lock()->Set_Texture("EvolveMenu_Talent_Icon_HealingExecution0_Title");
        m_pTalentInformation.lock()->Set_Texture("EvolveMenu_Talent_Icon_HealingExecution0_Information");
        break;
    case Client::TALENT_NAME::HEALINGEXECUTIONLV2:
        m_pTalentTitle.lock()->Set_Texture("EvolveMenu_Talent_Icon_HealingExecution1_Title");
        m_pTalentInformation.lock()->Set_Texture("EvolveMenu_Talent_Icon_HealingExecution1_Information");
        break;
    case Client::TALENT_NAME::SHARPWEAPONLV1:
        break;
    case Client::TALENT_NAME::SHARPWEAPONLV2:
        break;
    case Client::TALENT_NAME::ENERGISEDWEAPONLV1:
        break;
    case Client::TALENT_NAME::ENERGISEDWEAPONLV2:
        break;
    case Client::TALENT_NAME::TALENT_NAME_END:
        break;
    default:
        break;
    }
}

void CUI_EveolveMenu_Talent::Call_TalentMouseOver(weak_ptr<CTalent> pSelectedTalent)
{
    m_pSelectedTalent = pSelectedTalent;
    TALENT_NAME eTalent_Name = m_pSelectedTalent.lock()->Get_TalentName();

    TalentViewInformaiton_MouseOver(eTalent_Name);// 뷰 인포메이션 정보 갱신

    //지금 마우스오버한 상태에 따라 분기를 가름.
    TALENT_RESULT eTalentResult;

    CStatus_Player::PLAYERDESC tPlayerDesc;

    if (!GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock())
    {
        tPlayerDesc.m_iTalent = 5;
    }
    else
    {
        GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Status().lock()->Get_Desc(&tPlayerDesc);
    }
    _int        iCost = 0;
    list<weak_ptr<CTalent>> visitNodes;
    eTalentResult = m_pSelectedTalent.lock()->Check_Requiment(tPlayerDesc.m_iTalent, iCost, visitNodes);
    
    switch (eTalentResult)
    {
    case Client::TALENT_RESULT::FAILED:
        break;
    case Client::TALENT_RESULT::NOT_ENOUGHTPOINT:
        break;
    case Client::TALENT_RESULT::USING_ATHORTREE:
        break;
    case Client::TALENT_RESULT::SUCCESS:
        break;
    case Client::TALENT_RESULT::SUBSCRIPTPOINT:
        break;
    case Client::TALENT_RESULT::RESULT_END:
        break;
    default:
        break;
    }
    TalentAnimation_MouseOver(m_pSelectedTalent);
}


void CUI_EveolveMenu_Talent::Call_TalentMouseOut(weak_ptr<CTalent> pSelectedTalent)
{
    m_pTalentTitle.lock()->Set_Texture("None");
    m_pTalentInformation.lock()->Set_Texture("None");

    m_pSelectedTalent = pSelectedTalent;


    CStatus_Player::PLAYERDESC tPlayerDesc;

    GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Status().lock()->Get_Desc(&tPlayerDesc);

    _int        iCost = 0;
    list<weak_ptr<CTalent>> visitNodes;
    m_pSelectedTalent.lock()->Check_Requiment(tPlayerDesc.m_iTalent, iCost, visitNodes);

    TalentAnimation_MouseOut(m_pSelectedTalent);
}

void CUI_EveolveMenu_Talent::Call_TalentButtonClick(weak_ptr<CTalent> PSelectTalent)
{

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

void CUI_EveolveMenu_Talent::Free()
{
}

