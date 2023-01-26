#include "stdafx.h"
#include "UI_Interaction.h"
#include "EasingComponent_Alpha.h"
#include "Interaction_Prop.h"
#include "UI_Utils.h"
#include "Collider.h"

GAMECLASS_C(CUI_Interaction);
CLONE_C(CUI_Interaction, CGameObject);


HRESULT CUI_Interaction::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_Interaction::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Set_Texture("Interaction_Open");

    m_tUIDesc.fSizeX = 200.f;
    m_tUIDesc.fSizeY = 50.f;

    m_pEasingAlphaCom = Add_Component<CEasingComponent_Alpha>();
    m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    Set_Enable(false);
    m_bActiveAble = false;
    return S_OK;
}

HRESULT CUI_Interaction::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Interaction::Tick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

    __super::Tick(fTimeDelta);


    if (m_pCollisionCollider.lock())
    {
        _float2     m_fCollsionPos = CUI_Utils::ConvertWorldPosToUIPos(m_pCollisionCollider.lock()->Get_CurrentPosition(), 
            XMVectorSet(0.f, 1.f,0.f,1.f));
        Set_UIPosition(m_fCollsionPos.x, m_fCollsionPos.y);
    }


}

void CUI_Interaction::LateTick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

    __super::LateTick(fTimeDelta);
}


void CUI_Interaction::Call_CollisionEnter(weak_ptr<CCollider> pMyCollider, _uint _iInteractionType)
{
    Set_Enable(true);

  
    m_pEasingAlphaCom.lock()->Set_Lerp(0.f, 1.f, 0.3f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);
    m_pCollisionCollider = pMyCollider;
    CInteraction_Prop::INTERACTIONTYPE eInteractionType = (CInteraction_Prop::INTERACTIONTYPE)_iInteractionType;
    

    m_bActiveAble = true;
}

void CUI_Interaction::Call_CollisionExit()
{
    m_pCollisionCollider = weak_ptr<CCollider>();
    m_pEasingAlphaCom.lock()->Set_Lerp(m_fAlphaColor, 0.f, 0.3f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);
    m_bActiveAble = false;
}

void CUI_Interaction::Call_ActionStart()
{
    m_pEasingAlphaCom.lock()->Set_Lerp(m_fAlphaColor, 0.f, 0.3f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);
}

void CUI_Interaction::Call_ActionEnd()
{
    //만약에, 닿은 상태에서 애니메이션이 실행되고, 끝날때까짖 Exit하지 않은 경우, UI다시 떠라
    if (m_bActiveAble)
    {
        m_pEasingAlphaCom.lock()->Set_Lerp(0.f, 1.f, 0.3f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);
    }
}

