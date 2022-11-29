#include "stdafx.h"
#include "CorvusStates/Talent.h"
#include "GameManager.h"
#include "Player.h"

GAMECLASS_C(CTalent)
CLONE_C(CTalent, CGameObject)

HRESULT CTalent::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTalent::Initialize(void* pArg)
{
    //eak_ptr<CTalent> pNorSwordLv1 = GAMEINSTANCE->Add_GameObject(CTalent);

   return S_OK;
}

HRESULT CTalent::Start()
{
    return E_NOTIMPL;
}


void CTalent::Set_TalentInfo(_bool In_bActive,
    weak_ptr<CTalent_Effect> In_pEffet, weak_ptr<CTalent> In_pParent)
{
    m_bActive = In_bActive;
    m_pEffect = In_pEffet;
    m_pParent = In_pParent;
}



void CTalent::Tick(_float fTimeDelta)
{
    // 클릭하는 쪽에서 넣어주세요.
    // 특성이 개방이 될 때 호출
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



void CTalent::Free()
{
}

void CTalent::OnEnable(void* pArg)
{
}

void CTalent::OnDisable()
{
}
