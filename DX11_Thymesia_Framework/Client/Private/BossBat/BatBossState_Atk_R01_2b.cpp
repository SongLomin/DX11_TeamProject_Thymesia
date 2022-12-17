#include "stdafx.h"
#include "BossBat/BatBossState_Atk_R01_2b.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_Atk_R01_2b);
CLONE_C(CBatBossState_Atk_R01_2b, CComponent)

HRESULT CBatBossState_Atk_R01_2b::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Atk_R01_2b::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Atk_R01_2b::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|BossBat_AttackR_01_2b");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Atk_R01_2b::Call_AnimationEnd, this);
}
	


void CBatBossState_Atk_R01_2b::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Atk_R01_2b::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	

	Check_AndChangeNextState();
}


void CBatBossState_Atk_R01_2b::OnStateStart(const _float& In_fAnimationBlendTime)
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


void CBatBossState_Atk_R01_2b::OnStateEnd()
{
	__super::OnStateEnd();

}



void CBatBossState_Atk_R01_2b::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_AttackCount(1);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Atk_R01_2b::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Atk_R01_2b::Call_AnimationEnd, this);
}

void CBatBossState_Atk_R01_2b::Free()
{

}

_bool CBatBossState_Atk_R01_2b::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	
	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.5f)
	{
		m_bAttackLookAtLimit = false;
	}


	return false;
}

