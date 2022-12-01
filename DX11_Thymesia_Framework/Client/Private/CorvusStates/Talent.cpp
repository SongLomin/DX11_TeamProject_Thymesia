#include "stdafx.h"
#include "CorvusStates/Talent.h"
#include "GameManager.h"
#include "Player.h"
#include "CorvusStates/Talent_Effect.h"

GAMECLASS_C(CTalent)
CLONE_C(CTalent, CGameObject)

HRESULT CTalent::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTalent::Initialize(void* pArg)
{
    CGameObject::Initialize(pArg);

    //eak_ptr<CTalent> pNorSwordLv1 = GAMEINSTANCE->Add_GameObject(CTalent);

   return S_OK;
}

HRESULT CTalent::Start()
{
    CGameObject::Start();

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

void CTalent::Set_Parent(weak_ptr<CTalent> In_Parent)
{
    m_pParent = In_Parent;
    m_pParent.lock()->Add_TalentChild(Weak_StaticCast<CTalent>(m_this));
    
    Start();
}

void CTalent::Tick(_float fTimeDelta)
{
    // Ŭ���ϴ� �ʿ��� �־��ּ���.
    // Ư���� ������ �� �� ȣ��
  //  GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Bind_TalentEffects(m_pEffect);

}

void CTalent::LateTick(_float fTimeDelta)
{
}

HRESULT CTalent::Render()
{
    return E_NOTIMPL;
}

TALENT_RESULT CTalent::Check_Requiment(const int In_iPoint, int& Out_iCost, list<weak_ptr<CTalent>>& Out_pVisitNodes)
{
    int iDepth = 0;

  

    if (m_bActive)
    {
        Find_ActiveChild_Recursive(Weak_StaticCast<CTalent>(m_this), Out_pVisitNodes, iDepth);
        Out_iCost = iDepth;

        return TALENT_RESULT::SUBSTARICTPOINT;


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
            // �ڽĵ�� ���� �Դ� ��尡 ������ Ȯ��
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

void  CTalent::TestTalentCheck()
{
    

    int iCost = 0;
    int iMyTalentPoint = 5;

    TALENT_RESULT eResult;

    //�ӽ�
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
    case Client::TALENT_RESULT::SUBSTARICTPOINT:
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

void CTalent::Free()
{
}

void CTalent::OnEnable(void* pArg)
{
}

void CTalent::OnDisable()
{
}
