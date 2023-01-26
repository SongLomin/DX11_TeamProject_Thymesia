#include "stdafx.h"
#include "BossVarg/VargBossState_Exe_Start.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "Status_Boss.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CVargBossState_Exe_Start);
CLONE_C(CVargBossState_Exe_Start, CComponent)

HRESULT CVargBossState_Exe_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Exe_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Exe_Start::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TakeExecution_Start_FIX");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Exe_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Exe_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Exe_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Exe_Start::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CBossStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CVargBossState_Exe_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Get_Owner().lock()->Get_Component<CVargBossState_Exe_NoDeadEnd>().lock()->Set_DeadChoice(false);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	if (Check_RequirementIsTargeted())
		GET_SINGLE(CGameManager)->Release_Focus();

	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::PLAYERHUD);
	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::BATTLEUI);

	GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->OnEventMessage((_uint)EVENT_TYPE::ON_EXIT_SECTION);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CVargBossState_Exe_Start::Call_NextKeyFrame, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Exe_Start -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Exe_Start::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CVargBossState_Exe_Start::Call_NextKeyFrame, this, placeholders::_1);
}



void CVargBossState_Exe_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);
	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Exe_Dead>(0.05f);
}

void CVargBossState_Exe_Start::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		m_bDieType = false;
	}
}

void CVargBossState_Exe_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Exe_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Exe_Start::Free()
{

}

_bool CVargBossState_Exe_Start::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//여서조건을줘봐요 어떻게 ? 라이프가 2보다클떄 즉목숨이하나잇으면 여기로들어오고 안들어오면 ㅇㅇ
	if (m_bDieType)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 154)
		{
			m_pOwner.lock()->Get_Component<CStatus_Boss>().lock()->Set_NextPhase();
			Get_Owner().lock()->Get_Component<CVargBossState_Exe_NoDeadEnd>().lock()->Set_DeadChoice(false);
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Exe_NoDeadEnd>(0.05f);
			return true;
		}
	}
	return false;
}

void CVargBossState_Exe_Start::Call_NextKeyFrame(const _uint& In_iKeyIndex)
{
	switch (In_iKeyIndex)
	{
	case 154:


	default:
		break;
	}
}

