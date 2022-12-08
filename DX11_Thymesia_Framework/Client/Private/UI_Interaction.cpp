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

    Set_Texture("None");

    m_tUIDesc.fSizeX = 200.f;
    m_tUIDesc.fSizeY = 0.f;

    m_pEasingAlphaCom = Add_Component<CEasingComponent_Alpha>();
    m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    Set_Enable(false);

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
}

void CUI_Interaction::LateTick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

    __super::LateTick(fTimeDelta);
}


void CUI_Interaction::Call_CollisionEnter(weak_ptr<CCollider> pMyCollider, _uint _iInteractionType)
{
    Set_Enable(true);

    _float2     m_fCollsionPos = CUI_Utils::ConvertWorldPosToUIPos(pMyCollider.lock()->Get_CurrentPosition());

    Set_UIPosition(m_fCollsionPos.x, m_fCollsionPos.y);
    m_pEasingAlphaCom.lock()->Set_Lerp(0.f, 1.f, 1.f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);

    CInteraction_Prop::INTERACTIONTYPE eInteractionType = (CInteraction_Prop::INTERACTIONTYPE)_iInteractionType;
    
    switch (eInteractionType)
    {
    case CInteraction_Prop::INTERACTIONTYPE::INTERACTION_DOOR:
        Set_Texture("Interaction_Open");
        break;
    case CInteraction_Prop::INTERACTIONTYPE::INTERACTION_LADDER:
        Set_Texture("Interaction_Climb");
        break;
    case CInteraction_Prop::INTERACTIONTYPE::INTERACTION_ELEVATOR:
        Set_Texture("Interaction_Elevator");
        break;
    default:
        break;
    }
}

void CUI_Interaction::Call_CollisionExit()
{
    m_pEasingAlphaCom.lock()->Set_Lerp(m_fAlphaColor, 0.f, 1.f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE, false);

}

void CUI_Interaction::Call_ActionStart()
{
}

void CUI_Interaction::Call_ActionEnd()
{
}

