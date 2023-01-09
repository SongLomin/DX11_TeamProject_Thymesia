#include "stdafx.h"
#include "BossVarg/VargBossState_SPA_CatchFail.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_SPA_CatchFail);
CLONE_C(CVargBossState_SPA_CatchFail, CComponent)

HRESULT CVargBossState_SPA_CatchFail::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_SPA_CatchFail::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_SPA_CatchFail::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_SPAttack1_Fail");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_SPA_CatchFail::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_SPA_CatchFail::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_SPA_CatchFail::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_SPA_CatchFail::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_SPA_Run>().lock())
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 29);
	}

	else
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	}

	

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: SPA_CatchFail -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_SPA_CatchFail::OnStateEnd()
{
	__super::OnStateEnd();


}



void CVargBossState_SPA_CatchFail::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_SPA_CatchFail::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_SPA_CatchFail::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_SPA_CatchFail::Free()
{

}

_bool CVargBossState_SPA_CatchFail::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	

	return false;
}

