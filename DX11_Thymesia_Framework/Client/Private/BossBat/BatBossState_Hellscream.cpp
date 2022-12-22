#include "stdafx.h"
#include "BossBat/BatBossState_Hellscream.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_Hellscream);
CLONE_C(CBatBossState_Hellscream, CComponent)

HRESULT CBatBossState_Hellscream::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Hellscream::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Hellscream::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Hellscream");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Hellscream::Call_AnimationEnd, this);
}

void CBatBossState_Hellscream::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Hellscream::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}



void CBatBossState_Hellscream::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_Hellscream::OnStateEnd()
{
	__super::OnStateEnd();

}



void CBatBossState_Hellscream::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_HeelScream(true);
	Get_Owner().lock()->Get_Component<CBatBossState_Charge>().lock()->Set_HellScream(0);
	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_ZeroAttackCount(0);
	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_ZeroChargeCount(0);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Hellscream::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Hellscream::Call_AnimationEnd, this);
}

void CBatBossState_Hellscream::Free()
{

}

_bool CBatBossState_Hellscream::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (ComputeAngleWithPlayer() > 0.97f)
	{
		Rotation_TargetToLookDir();
		m_bAttackLookAtLimit = false;
	}

	

	return false;
}

