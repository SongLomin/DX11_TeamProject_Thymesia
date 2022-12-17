#include "stdafx.h"
#include "BossBat/BatBossState_SonicBoom.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_SonicBoom);
CLONE_C(CBatBossState_SonicBoom, CComponent)

HRESULT CBatBossState_SonicBoom::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_SonicBoom::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_SonicBoom::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|BossBat_SonicBoom");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_SonicBoom::Call_AnimationEnd, this);
}

void CBatBossState_SonicBoom::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
	}
	else
	{
		TurnAttack(fTimeDelta);
	}


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_SonicBoom::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_SonicBoom::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_SonicBoom>().lock())
	{
		m_iSonicBoomCount -= 1;
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 229);
	}
	else
	{
		m_iSonicBoomCount = 3;
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	}
	
	m_bAttackLookAtLimit = true;
	


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif


}	


void CBatBossState_SonicBoom::OnStateEnd()
{
	__super::OnStateEnd();

}



void CBatBossState_SonicBoom::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_AttackCount(1);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
}

void CBatBossState_SonicBoom::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_SonicBoom::Call_AnimationEnd, this);
}

void CBatBossState_SonicBoom::Free()
{

}

_bool CBatBossState_SonicBoom::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	
	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.1f)
	{
		m_bAttackLookAtLimit = false;
	}

	if (m_iSonicBoomCount > 0 && m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() == 312)
	{	
		Get_OwnerCharacter().lock()->Change_State<CBatBossState_SonicBoom>(0.05f);
		return true;
	}


	return false;
}

