#include "stdafx.h"
#include "BossBat/BatBossState_Stun_End.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_Stun_End);
CLONE_C(CBatBossState_Stun_End, CComponent)

HRESULT CBatBossState_Stun_End::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Stun_End::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Stun_End::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_StunEnd");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Stun_End::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Stun_End::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Stun_End::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_Stun_End::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif

}	


void CBatBossState_Stun_End::OnStateEnd()
{
	__super::OnStateEnd();

}

void CBatBossState_Stun_End::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Stun_End::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Stun_End::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Stun_End::Free()
{

}

_bool CBatBossState_Stun_End::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}

