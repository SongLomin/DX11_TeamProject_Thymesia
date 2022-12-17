#include "stdafx.h"
#include "BossBat/BatBossState_Sp.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_Sp);
CLONE_C(CBatBossState_Sp, CComponent)

HRESULT CBatBossState_Sp::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Sp::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Sp::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|BossBat_SP01");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Sp::Call_AnimationEnd, this);
}

void CBatBossState_Sp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Sp::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_Sp::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_Sp::OnStateEnd()
{
	__super::OnStateEnd();


}



void CBatBossState_Sp::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_ZeroChargeCount(0);
	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_ZeroAttackCount(0);
	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_HeelScream(false);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Sp::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Sp::Call_AnimationEnd, this);
}

void CBatBossState_Sp::Free()
{

}

_bool CBatBossState_Sp::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}

