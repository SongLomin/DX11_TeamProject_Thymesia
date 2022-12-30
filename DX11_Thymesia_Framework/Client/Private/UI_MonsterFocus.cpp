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
#include "Shader.h"


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
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

}

void CUI_MonsterFocus::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);

    //틱에서 하면 순서에 의해 전프레임 위치를 받아올 수 있음
    if (!m_pTargetMonster.lock())
    {
        Call_FocusOutTarget();
        m_pBoneNode = weak_ptr< CBoneNode>();
        return;
    }

    if (m_pBoneNode.lock())
    {
        FollowTargetBone();
    }

}

HRESULT CUI_MonsterFocus::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);

    return S_OK;
}

void CUI_MonsterFocus::Call_FocusInTarget()
{
    m_pTargetMonster = GET_SINGLE(CGameManager)->Get_TargetMonster();

    m_fAlphaColor = 1.f;

    m_vColor = CUI_Utils::GET_COLOR(COLOR_PALETTE::COLOR_WHITE);

    Bind_Callback_LockOnTriger(m_pTargetMonster);

    m_pBoneNode = FindTargetBone(m_pTargetMonster.lock()->Get_Model());
    if (!m_pBoneNode.lock())//찾은 본이 없다면 그냥 출력 X.
    {
        Call_FocusOutTarget();
        return;
    }

    Set_Enable(true);
}



void CUI_MonsterFocus::Call_FocusOutTarget()
{
    if (m_pTargetMonster.lock())
        m_pTargetMonster.lock()->Callback_OnChangeState -= bind(&CUI_MonsterFocus::Call_ChangeState, this, placeholders::_1);

    m_pTargetMonster = weak_ptr<CMonster>();

    m_pBoneNode = weak_ptr<CBoneNode>();

    m_pEasingAlphaCom.lock()->Stop();
    m_fAlphaColor = 1.f;
    Set_Enable(false);
}

void CUI_MonsterFocus::Call_ChangeState(weak_ptr<class CStateBase> pChangedState)
{
    //여기서 상태에 따라 변화시켜줌.
    //상태를 검사.
    /*
       몬스터를 처형 가능? 불가능?으로만 알고싶은데
       근데 이러면 장점은있긴하다.
       나중에 위험한 공격같은거 캐치할 때, 
       락온이 초록색으로 빛나게 할 수 있음.
    */
    if (Compare_StateFlag(pChangedState, STATE_FLAG::EXECUTABLE))
    {
        m_vColor = CUI_Utils::GET_COLOR(COLOR_PALETTE::COLOR_RED);
        m_pEasingAlphaCom.lock()->Set_Lerp(0.f, 1.f, 0.3f, EASING_TYPE::QUAD_IN, CEasingComponent::LOOP_GO_AND_BACK, false);

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

HRESULT CUI_MonsterFocus::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pShaderCom.lock()->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));

    return S_OK;
}

weak_ptr<CBoneNode> CUI_MonsterFocus::FindTargetBone(weak_ptr<class CModel> pTargetModel)
{
    weak_ptr<CBoneNode> pBoneNode;
    
    pBoneNode = pTargetModel.lock()->Find_BoneNode("Spine01");
    if (!pBoneNode.lock())
    {
        pBoneNode = pTargetModel.lock()->Find_BoneNode("Bip001-Spine1");
    }
    if (!pBoneNode.lock())
    {
        pBoneNode = pTargetModel.lock()->Find_BoneNode("spine_01");
    }
	if (!pBoneNode.lock())
	{
		pBoneNode = pTargetModel.lock()->Find_BoneNode("Bone");
	}

    return pBoneNode;
}

void CUI_MonsterFocus::FollowTargetBone()
{
  
   _matrix  matTargetCombined = m_pBoneNode.lock()->Get_CombinedMatrix();
   // _vector vTargetWorldPos =    m_pTargetMonster.lock()->Get_WorldPosition();

   _float4x4 matTransformation = m_pTargetMonster.lock()->Get_Component<CModel>().lock()->Get_TransformationMatrix();
   _matrix  matTargetMonsterWorld = XMLoadFloat4x4(&matTransformation);

   _matrix BoneMatrix = matTargetCombined * matTargetMonsterWorld;

   BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
   BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
   BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

   _matrix WorldMatrix = BoneMatrix * m_pTargetMonster.lock()->Get_Transform()->Get_UnScaledWorldMatrix();

   // _vector  vTargetCombinedWorldPos = XMVector3TransformCoord(vTargetWorldPos, BoneMatrix);

   _float2   fMyPos = CUI_Utils::ConvertWorldPosToUIPos(WorldMatrix.r[3], XMVectorSet(0.f, 0.f, 0.f, 1.f));

   Set_UIPosition(fMyPos.x, fMyPos.y);
}

_bool CUI_MonsterFocus::Compare_StateFlag(weak_ptr<CStateBase> pStateBase, STATE_FLAG Checkflag)
{
    return pStateBase.lock()->Get_StateFlag() & (_flag)Checkflag;
}

void CUI_MonsterFocus::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    if (!m_pTargetMonster.lock())//타겟이 없다면 다시 꺼버림.
    {
        Set_Enable(false);
    }

}
