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

GAMECLASS_C(CVargBossState_Stun_Exe_Start);
CLONE_C(CVargBossState_Stun_Exe_Start, CComponent)

HRESULT CVargBossState_Stun_Exe_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Stun_Exe_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Stun_Exe_Start::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TakeExecution_Start_FIX");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Stun_Exe_Start::Call_AnimationEnd, this);
}

void CVargBossState_Stun_Exe_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Stun_Exe_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Stun_Exe_Start::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CBossStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CVargBossState_Stun_Exe_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Exe_Start -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Stun_Exe_Start::OnStateEnd()
{
	__super::OnStateEnd();


}



void CVargBossState_Stun_Exe_Start::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Stun_Exe_Dead>(0.05f);
}

void CVargBossState_Stun_Exe_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Stun_Exe_Start::Call_AnimationEnd, this);
}

void CVargBossState_Stun_Exe_Start::Free()
{

}

_bool CVargBossState_Stun_Exe_Start::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	//여서조건을줘봐요 어떻게 ? 라이프가 2보다클떄 즉목숨이하나잇으면 여기로들어오고 안들어오면 ㅇㅇ
	


	if (m_bDieType)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() == 154)
		{
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Exe_NoDeadEnd>(0.05f);
			return true;
		}
	}


	return false;
}

