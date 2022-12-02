#include "stdafx.h"
#include "CorvusStates/Talent.h"
#include "GameManager.h"
#include "Player.h"
#include "CorvusStates/Talent_Effect.h"
// #include "UI_EvolveMenu_TalentButton.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "Fader.h"
#include "GameManager.h"
#include "Client_GameObjects.h"
#include "CustomUI.h"
#include "Status_Player.h"
#include "UI_EvolveMenu_Level.h"
#include "UI_Button.h"
#include "UI_EvolveMenu_Talent.h"
GAMECLASS_C(CTalent)
CLONE_C(CTalent, CGameObject)

HRESULT CTalent::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTalent::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    
    if (pArg == nullptr)
    {
        m_tUIDesc.fX = 800.f;
        m_tUIDesc.fY = 600.f;
        m_tUIDesc.fSizeX = 40.f;
        m_tUIDesc.fSizeY = 40.f;
        m_tUIDesc.fDepth = 0.f;
    }
    m_pButtonFrame = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);

    m_pButtonFrame.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, 80.f, 80.f);
    m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame");
    m_pButtonFrame.lock()->Set_Depth(0.2f);

    m_pButtonActive = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);

    m_pButtonActive.lock()->Set_UIPosition
    (
        m_tUIDesc.fX, m_tUIDesc.fY,
        180.f, 180.f
    );
    m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_None");
    m_pButtonActive.lock()->Set_Depth(0.3f);


    m_pIcon = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);

    m_pIcon.lock()->Set_UIPosition
    (
        m_tUIDesc.fX, m_tUIDesc.fY,
        180.f, 180.f
    );
    m_pIcon.lock()->Set_Texture("EvolveMenu_PW_None");
    m_pIcon.lock()->Set_Depth(0.1f);


    m_bActive = false;
    m_iNodeLevel = 0;

    Add_Child(m_pButtonFrame);
    Add_Child(m_pIcon);
    Add_Child(m_pButtonActive);

    Set_Enable(false);

   return S_OK;
}

HRESULT CTalent::Start()
{
    __super::Start();

    m_pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
    m_pEffect = Get_ComponentByType<CTalent_Effect>();


   
    return S_OK;
}

void CTalent::Set_TalentInfo(_bool In_bActive,
    weak_ptr<CTalent_Effect> In_pEffet, weak_ptr<CTalent> In_pParent)
{
    m_bActive = In_bActive;
    m_pEffect = In_pEffet;
    m_pParent = In_pParent;
}

void CTalent::Add_TalentChild(weak_ptr<CTalent> In_pChild)
{
    m_pChilds.push_back(In_pChild);
}

void CTalent::Update_ChildUI()
{
    for (auto& elem : m_vecChildUI)//자식들은 버튼의 알파를 따라감.
    {
        elem.lock()->Set_UIPosition
        (
            m_tUIDesc.fX,
            m_tUIDesc.fY
        );
        elem.lock()->Set_AlphaColor(m_fAlphaColor);
    }
}

void CTalent::OnMouseOver()
{
    __super::OnMouseOver();

    Callback_OnMouseOver(m_eTalentName);
    if (m_bActive)
    {
        CheckMouseOver();
 
        list<weak_ptr<CTalent>> listChild;
        int i = 0;
        Find_ActiveChild_Recursive(Weak_StaticCast<CTalent>(m_this), listChild, i);
        for (auto& elem : listChild)
        {
            elem.lock()->CheckMouseOver();
        }
    }
    else
    {
        UnCheckMouseOver();
        list<weak_ptr<CTalent>> listParents;

        int i = 0;
        Find_ActiveParent_Recursive(m_pParent, listParents, i);
        for (auto& elem : listParents)
        {
            if (elem.lock()->m_bActive)
            {
                elem.lock()->CheckMouseOver();
            }
            else
            {
                elem.lock()->UnCheckMouseOver();
            }
        }
    }
}

void CTalent::OnMouseOut()
{
    __super::OnMouseOut();
    Callback_OnMouseOut();
    if (m_bActive)
    {
        CheckMouseOut();

        list<weak_ptr<CTalent>> listChild;
        int i = 0;
        Find_ActiveChild_Recursive(Weak_StaticCast<CTalent>(m_this), listChild, i);
        for (auto& elem : listChild)
        {
            elem.lock()->CheckMouseOut();
        }
        weak_ptr<CTalent> pParent = Get_RootNode();

        if(pParent.lock())
            pParent.lock()->CheckMouseOut();
    }
    else
    {
        list<weak_ptr<CTalent>> listParent;//나도 포함임

        Find_AllParent_Recursive(Weak_StaticCast<CTalent>(m_this), listParent);

        for (auto& elem : listParent)
        {
            elem.lock()->m_bActive ? elem.lock()->CheckMouseOut() : elem.lock()->UnCheckMouseOut();
        }
    }
}

void CTalent::OnLButtonDown()
{
}

void CTalent::OnLButtonClick()
{
    __super::OnLButtonClick();


    CStatus_Player::PLAYERDESC tPlayerDesc;

    if (!GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock())
    {
        tPlayerDesc.m_iTalent = 5;
    }
    else
    {
        GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Status().lock()->Get_Desc(&tPlayerDesc);
    }

    list<weak_ptr<CTalent>> visitNodes;

    TALENT_RESULT eResult = Check_Requiment(tPlayerDesc.m_iTalent,m_iNodeLevel, visitNodes);

    switch (eResult)
    {
    case Client::TALENT_RESULT::FAILED:
        break;
    case Client::TALENT_RESULT::NOT_ENOUGHTPOINT:
        break;
    case Client::TALENT_RESULT::USING_ATHORTREE:
        return;
        break;
    case Client::TALENT_RESULT::SUCCESS:
    {
        weak_ptr<CTalent> pTalent;
        for (auto& elem : visitNodes)
        {
            pTalent = elem;
            elem.lock()->CheckLButtonClick();
#ifndef _ONLY_UI_
            m_pPlayer.lock()->Bind_TalentEffects(elem.lock()->Get_Effect());
#endif
        }
        if (pTalent.lock()->m_pParent.lock())
            pTalent.lock()->m_pParent.lock()->CheckLButtonClick();
        break;
    }
    case Client::TALENT_RESULT::SUBSCRIPTPOINT:
        for (auto& elem : visitNodes)
        {
            if (elem.lock()->m_pParent.lock())
            {
                elem.lock()->UnCheckLButtonClick();
#ifndef _ONLY_UI_
                m_pPlayer.lock()->UnBind_TalentEffects(elem.lock()->Get_Effect());
#endif // !_ONLY_UI_
            }
        }
        break;
    case Client::TALENT_RESULT::RESULT_END:
        break;
    default:
        break;
    }
    /*
    m_bActive = !m_bActive;
    if (m_bActive)
    {
        CheckLButtonClick();
    }
    else
    {
        UnCheckLButtonClick();
    }
    */
    /*
    if (true)//모종의 조건이 성립한다면
    {
        if (m_bActive)//켜진거면 내 위로 모든 부모들 클릭된상태로
        {
            weak_ptr<CTalent>   pParent = m_pParent;
            while (pParent.lock())
            {
                pParent.lock()->CheckLButtonClick();
                pParent = pParent.lock()->m_pParent;
            }
            if (pParent.lock())
                pParent.lock()->CheckLButtonClick();
        }
        else
        {
            weak_ptr<CTalent>   pChild;

            for (auto& elem : m_pChilds)//활성화된 자식 찾기
            {
                if (elem.lock()->m_bActive)
                {
                    pChild = elem;
                    break;
                }
            }
            if (!pChild.lock())
                return;
            list<weak_ptr<CTalent>> listChild;
            int i = 0;
            Find_ActiveChild_Recursive(pChild, listChild, i);
            
            pChild.lock()->UnCheckLButtonClick();
            for (auto& elem : listChild)
            {
                elem.lock()->UnCheckLButtonClick();
            }
        }
    }*/
}

void CTalent::CheckMouseOver()//체크되어 있는 상태에서 마우스오버
{

    m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_Active_Hover");
    m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame_Active");

}

void CTalent::CheckMouseOut()//체크되어 있는 상태에서 마우스아웃
{
    m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_Active");
    m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame_Active");
}

void CTalent::CheckLButtonClick()//마우스 버튼 켜진시점
{
    if (!m_pParent.lock())
        return;
    m_bActive = true;
    m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_Active");
    m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame_Active");
}

void CTalent::UnCheckMouseOver()//클릭 X일떄 마우스오버
{
    if (!m_pParent.lock())
        return;
    m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_Frame_Hover");
    m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame");

}

void CTalent::UnCheckMouseOut()//클릭 X일때 마우스아웃
{
    if (!m_pParent.lock())
        return;

    m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_None");
    m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame");
}


void CTalent::UnCheckLButtonClick()//눌러서 버튼 꺼진시점
{
    if (!m_pParent.lock())
        return;
    m_bActive = false;
    m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame");
    m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_None");
}

_uint CTalent::GetActiveNodeCount(weak_ptr<CTalent> In_Talent)
{
    _uint   iActiveNode = 0;


    if (m_bActive == true)
        ++iActiveNode;


    for (auto& elem : m_pChilds)
    {
        if (elem.lock()->m_bActive)
            iActiveNode += elem.lock()->GetActiveNodeCount(elem);
    }

    return iActiveNode;
}

weak_ptr<CTalent> CTalent::Get_RootNode()
{
    weak_ptr<CTalent>   pParent = m_pParent;

    if (m_pParent.lock())
    {
        while (pParent.lock()->Get_Parent().lock())
        {
            pParent = pParent.lock()->Get_Parent();
        }
    }
    return pParent;
}

void CTalent::Set_Parent(weak_ptr<CTalent> In_Parent)
{
    m_pParent = In_Parent;
    m_pParent.lock()->Add_TalentChild(Weak_StaticCast<CTalent>(m_this));
    
    m_iNodeLevel = m_pParent.lock()->Get_NodeLevel() + 1;

    Start();
}

void CTalent::Set_Active(_bool Active)
{
    m_bActive = Active;

    if (m_bActive)
    {
        m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_Active");
        m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame_Active");
    }
    else
    {
        m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame");
        m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_None");
    }
}

void CTalent::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    // 클릭하는 쪽에서 넣어주세요.
    // 특성이 개방이 될 때 호출
  //  GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Bind_TalentEffects(m_pEffect);

    Update_ChildUI();
}

void CTalent::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    /*
        스킬 한번에 찍기 따라라릭
        내가 건담이 된다   
    */
    //만약에, 내 버튼 상태가 MouseOver라면.
    if (Get_ButtonState() == CUI_Button_Base::UI_BUTTON_OVER)
    {
       
    }
    else if (Get_ButtonState() == CUI_Button_Base::UI_BUTTON_OUT)
    {
     
    }
}


TALENT_RESULT CTalent::Check_Requiment(const int In_iPoint, int& Out_iCost, list<weak_ptr<CTalent>>& Out_pVisitNodes)
{
    int iDepth = 0;

    if (m_bActive)
    {
        Find_ActiveChild_Recursive(Weak_StaticCast<CTalent>(m_this), Out_pVisitNodes, iDepth);
        Out_iCost = iDepth;

        return TALENT_RESULT::SUBSCRIPTPOINT;


    }
    else
    {
        Out_pVisitNodes.push_back(Weak_StaticCast<CTalent>(m_this));
        Find_ActiveParent_Recursive(m_pParent, Out_pVisitNodes, iDepth);
        Out_iCost = iDepth;
        if (In_iPoint < iDepth)
        {
            return TALENT_RESULT::NOT_ENOUGHTPOINT;
        }

        weak_ptr<CTalent> pActivatedParent = Out_pVisitNodes.back();
        Out_pVisitNodes.pop_back();


        for (auto& elem : pActivatedParent.lock()->m_pChilds)
        {
            // 자식들과 내가 왔던 노드가 같은지 확인
            if (elem.lock() == Out_pVisitNodes.back().lock())
            {
                continue;
            }

            if (elem.lock()->m_bActive)
            {
                return TALENT_RESULT::USING_ATHORTREE;
            }
        }
        return TALENT_RESULT::SUCCESS;
    }
}

void CTalent::Find_ActiveParent_Recursive(weak_ptr<CTalent> In_pTalent, list<weak_ptr<CTalent>>& out_pActiveParent, int& out_iDepth)
{
    if (!In_pTalent.lock())
    {
        return;
    }

    ++out_iDepth;
    out_pActiveParent.push_back(In_pTalent);

    if (In_pTalent.lock()->m_bActive)
    {
        return;
    }

    Find_ActiveParent_Recursive(In_pTalent.lock()->m_pParent, out_pActiveParent, out_iDepth);
}

void CTalent::Find_ActiveChild_Recursive(weak_ptr<CTalent> In_pTalent, list<weak_ptr<CTalent>>& out_pActiveChild, int& out_iDepth)
{
    if (!In_pTalent.lock())
    {
        return;
    }

    
    if (In_pTalent.lock()->m_bActive)
    {
        ++out_iDepth;
        out_pActiveChild.push_back(In_pTalent);

        for (auto& elem : In_pTalent.lock()->m_pChilds)
        {
            Find_ActiveChild_Recursive(elem, out_pActiveChild, out_iDepth);
        }
    }
    return;
}

_bool CTalent::Check_ActiveAble_ThisNode()
{
   

    return false;
}

void CTalent::Find_AllParent_Recursive(weak_ptr<CTalent> In_pTalent, list<weak_ptr<CTalent>>& out_pActiveParent)
{
    out_pActiveParent.push_back(In_pTalent);

    if (!In_pTalent.lock()->m_pParent.lock())
        return;
    
    Find_AllParent_Recursive(In_pTalent.lock()->m_pParent, out_pActiveParent);
}


void  CTalent::TestTalentCheck()
{
    

    int iCost = 0;
    int iMyTalentPoint = 5;

    TALENT_RESULT eResult;

    //임시
    //m_pPlayer.lock()->Bind_TalentEffects(pAvoidThrustLV1.lock()->Get_Effect());
    //m_pPlayer.lock()->Bind_TalentEffects(pSwordLV2.lock()->Get_Effect());
    list<weak_ptr<CTalent>> pVisitNodes;
    eResult = Check_Requiment(iMyTalentPoint, iCost, pVisitNodes);

    switch (eResult)
    {
    case Client::TALENT_RESULT::FAILED:
        break;
    case Client::TALENT_RESULT::NOT_ENOUGHTPOINT:
        break;
    case Client::TALENT_RESULT::USING_ATHORTREE:
        break;
    case Client::TALENT_RESULT::SUCCESS:
        iMyTalentPoint -= iCost;
        for (auto& elem : pVisitNodes)
        {
            elem.lock()->Set_Active(true);
            m_pPlayer.lock()->Bind_TalentEffects(elem.lock()->Get_Effect());
        }
        //m_pPlayer.lock()->Bind_TalentEffects(pSwordLV2.lock()->Get_Effect());
        //m_pPlayer.lock()->Bind_TalentEffects(pAvoidThrustLV1.lock()->Get_Effect());
        break;
    case Client::TALENT_RESULT::SUBSCRIPTPOINT:
        iMyTalentPoint += iCost;
        for (auto& elem : pVisitNodes)
        {
            elem.lock()->Set_Active(false);
            m_pPlayer.lock()->UnBind_TalentEffects(elem.lock()->Get_Effect());
        }

        break;
    }
    cout << "iTalent Point : " << iMyTalentPoint << endl;

}

void CTalent::Set_TALENT_NAME(TALENT_NAME TalentName)
{
    m_eTalentName = TalentName;

    switch (m_eTalentName)
    {
    case Client::TALENT_NAME::NORSWORDLV1:
        m_pIcon.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Basic");
        break;
    case Client::TALENT_NAME::NORSWORDLV2:
        m_pIcon.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Basic");
        break;
    case Client::TALENT_NAME::AVOIDSLASHLV1:
        m_pIcon.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Cross");
        break;
    case Client::TALENT_NAME::AVOIDSLASHLV2:
        m_pIcon.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Cross");
        break;
    case Client::TALENT_NAME::AVOIDTHRUSTLV1:
        m_pIcon.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Stab");
        break;
    case Client::TALENT_NAME::AVOIDTHRUSTLV2:
        m_pIcon.lock()->Set_Texture("EvolveMenu_Talent_Icon_LAttack_Stab");
        break;
    case Client::TALENT_NAME::JUMPATTACKLV1:
        break;
    case Client::TALENT_NAME::JUMPATTACKLV2:
        break;
    case Client::TALENT_NAME::JUMPATTACKLV3:
        break;
    case Client::TALENT_NAME::EXECUTION:
        break;
    case Client::TALENT_NAME::HEALINGEXECUTIONLV1:
        break;
    case Client::TALENT_NAME::HEALINGEXECUTIONLV2:
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



void CTalent::Free()
{
}

void CTalent::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    for (auto& elem : m_pChilds)
    {
        elem.lock()->Set_Enable(true);
    }

}

void CTalent::OnDisable()
{
    __super::OnDisable();

    for (auto& elem : m_pChilds)
    {
        elem.lock()->Set_Enable(false);
    }
}
