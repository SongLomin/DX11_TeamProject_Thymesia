#include "stdafx.h"
#include "UI_MonsterFocus.h"
#include "UI_Utils.h"
#include "GameManager.h"
#include "Monster.h"
#include "Transform.h"
#include "Model.h"
#include "BoneNode.h"
#include "Preset_UIDesc.h"
#include "EasingComponent_Alpha.h"
#include "NorMonState/NorMonState_GroggyStart.h"
#include "NorMonState/NorMonState_GroggyEnd.h"


GAMECLASS_C(CUI_MonsterFocus)
CLONE_C(CUI_MonsterFocus, CGameObject)

HRESULT CUI_MonsterFocus::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    m_pEasingAlphaCom = Add_Component<CEasingComponent_Alpha>();

    return S_OK;
}

HRESULT CUI_MonsterFocus::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    CPreset_UIDesc::Set_CUI_MonsterFocus(Weak_StaticCast<CUI_MonsterFocus>(m_this));

    Bind_Callback();

    SetUp_Component();

    Set_Enable(false);
    return S_OK;
}

HRESULT CUI_MonsterFocus::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_MonsterFocus::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta(fTimeDelta);

    __super::Tick(fTimeDelta);

}

void CUI_MonsterFocus::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta(fTimeDelta);

    __super::LateTick(fTimeDelta);

    //틱에서 하면 순서에 의해 전프레임 위치를 받아올 수 있음
    FollowTargetBone();

}

HRESULT CUI_MonsterFocus::Render()
{
    __super::Render();

    return S_OK;
}

void CUI_MonsterFocus::Call_FocusInTarget()
{
    m_pTargetMonster = GET_SINGLE(CGameManager)->Get_TargetMonster();

    m_pEasingAlphaCom.lock()->Set_Lerp(0.f, 1.f, 1.f, EASING_TYPE::QUAD_IN, CEasingComponent::LOOP_GO_AND_BACK, false);

    Set_Enable(true);
}

void CUI_MonsterFocus::Call_GroggyStart()
{
    m_pEasingAlphaCom.lock()->Set_Lerp(0.f, 1.f, 1.f, EASING_TYPE::QUAD_IN, CEasingComponent::LOOP_GO_AND_BACK, false);
}

void CUI_MonsterFocus::Call_FocusOutTarget()
{
    m_pTargetMonster = weak_ptr<CMonster>();

    Set_Enable(false);
}

void CUI_MonsterFocus::Bind_Callback()
{
    GET_SINGLE(CGameManager)->CallBack_FocusInMonster += bind(&CUI_MonsterFocus::Call_FocusInTarget, this);
    GET_SINGLE(CGameManager)->CallBack_FocusOutMonster += bind(&CUI_MonsterFocus::Call_FocusOutTarget, this);

}

void CUI_MonsterFocus::SetUp_Component()
{
    m_pEasingAlphaCom = Add_Component<CEasingComponent_Alpha>();
}

void CUI_MonsterFocus::FollowTargetBone()
{
   weak_ptr<CBoneNode> pBoneNode =  m_pTargetMonster.lock()->Get_Model().lock()->
       Find_BoneNode(m_strTargetMonsterBone);

   if (!pBoneNode.lock())//찾은 본이 없다면 그냥 출력 X.
   {
       Call_FocusOutTarget();
       return;
   }

   _matrix  matTargetCombined = pBoneNode.lock()->Get_CombinedMatrix();
   _vector vTargetWorldPos =    m_pTargetMonster.lock()->Get_WorldPosition();
   _vector  vTargetCombinedWorldPos = XMVector3TransformCoord(vTargetWorldPos, matTargetCombined);

   _float2   fMyPos = CUI_Utils::ConvertWorldPosToUIPos(vTargetCombinedWorldPos);

   Set_UIPosition(fMyPos.x, fMyPos.y);
}

void CUI_MonsterFocus::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    if (!m_pTargetMonster.lock())//타겟이 없다면 다시 꺼버림.
    {
        Set_Enable(false);
    }

}
