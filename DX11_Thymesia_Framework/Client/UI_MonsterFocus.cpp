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
#include "StateBase.h"
#include "UI_Utils.h"

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

    //ƽ���� �ϸ� ������ ���� �������� ��ġ�� �޾ƿ� �� ����
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

    Bind_Callback_LockOnTriger(m_pTargetMonster);

    Set_Enable(true);
}

void CUI_MonsterFocus::Call_GroggyStart()
{
    m_pEasingAlphaCom.lock()->Set_Lerp(0.f, 1.f, 1.f, EASING_TYPE::QUAD_IN, CEasingComponent::LOOP_GO_AND_BACK, false);
}

void CUI_MonsterFocus::Call_FocusOutTarget()
{
    if (m_pTargetMonster.lock())
        m_pTargetMonster.lock()->Callback_OnChangeState -= bind(&CUI_MonsterFocus::Call_ChangeState, this, placeholders::_1);

    m_pTargetMonster = weak_ptr<CMonster>();

    Set_Enable(false);
}

void CUI_MonsterFocus::Call_ChangeState(weak_ptr<class CStateBase> pChangedState)
{
    //���⼭ ���¿� ���� ��ȭ������.
    //���¸� �˻�.
    /*
       ���͸� ó�� ����? �Ұ���?���θ� �˰������
       �ٵ� �̷��� �������ֱ��ϴ�.
       ���߿� ������ ���ݰ����� ĳġ�� ��, 
       ������ �ʷϻ����� ������ �� �� ����.
    */
    if (Compare_StateFlag(pChangedState, STATE_FLAG::EXECUTABLE))
    {
        m_vColor = CUI_Utils::GET_COLOR(COLOR_PALETTE::COLOR_RED);
    }
    else if (Compare_StateFlag(pChangedState, STATE_FLAG::READY_SPECIAL_ATK))
    {
        m_vColor = CUI_Utils::GET_COLOR(COLOR_PALETTE::COLOR_GREEN);
    }
    else
    {
        m_vColor = CUI_Utils::GET_COLOR(COLOR_PALETTE::COLOR_WHITE);
    }
}

void CUI_MonsterFocus::Bind_Callback()
{
    GET_SINGLE(CGameManager)->CallBack_FocusInMonster += bind(&CUI_MonsterFocus::Call_FocusInTarget, this);
    GET_SINGLE(CGameManager)->CallBack_FocusOutMonster += bind(&CUI_MonsterFocus::Call_FocusOutTarget, this);

}

void CUI_MonsterFocus::Bind_Callback_LockOnTriger(weak_ptr<CMonster> pTargetMonster)
{
    weak_ptr<CStateBase> pCurrentStateBase = pTargetMonster.lock()->Get_CurState();

    if (pTargetMonster.lock()->Get_CurState().lock()->Get_StateFlag() & (_uint)STATE_FLAG::EXECUTABLE)
    { 
        Call_ChangeState(pCurrentStateBase);
    }
    else
    {
        pTargetMonster.lock()->Callback_OnChangeState +=
            bind(&CUI_MonsterFocus::Call_ChangeState, this, placeholders::_1);
    }

  
}

void CUI_MonsterFocus::SetUp_Component()
{
    m_pEasingAlphaCom = Add_Component<CEasingComponent_Alpha>();
}

weak_ptr<CBoneNode> CUI_MonsterFocus::FindTargetBone(weak_ptr<class CModel> pTargetModel)
{
    weak_ptr<CBoneNode> pBoneNode;
    
    pBoneNode = pTargetModel.lock()->Find_BoneNode("Spine01");
    if (!pBoneNode.lock())
    {
        pBoneNode = pTargetModel.lock()->Find_BoneNode("Bip001-Spine");
    }

    return pBoneNode;
}

void CUI_MonsterFocus::FollowTargetBone()
{
    weak_ptr<CBoneNode> pBoneNode = FindTargetBone(m_pTargetMonster.lock()->Get_Model());
   if (!pBoneNode.lock())//ã�� ���� ���ٸ� �׳� ��� X.
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

_bool CUI_MonsterFocus::Compare_StateFlag(weak_ptr<CStateBase> pStateBase, STATE_FLAG Checkflag)
{
    return pStateBase.lock()->Get_StateFlag() & (_flag)Checkflag;
}

void CUI_MonsterFocus::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    if (!m_pTargetMonster.lock())//Ÿ���� ���ٸ� �ٽ� ������.
    {
        Set_Enable(false);
    }

}
