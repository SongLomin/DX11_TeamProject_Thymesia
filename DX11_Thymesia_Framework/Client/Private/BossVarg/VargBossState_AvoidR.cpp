#include "stdafx.h"
#include "BossVarg/VargBossState_AvoidR.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_AvoidR);
CLONE_C(CVargBossState_AvoidR, CComponent)

HRESULT CVargBossState_AvoidR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_AvoidR::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_AvoidR::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_AvoidR");

	m_bAttackLookAtLimit = true;  // 애니메이션시작할떄 룩엣시작

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_AvoidR::Call_AnimationEnd, this);
}

void CVargBossState_AvoidR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		Turn_ToThePlayer(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_AvoidR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_AvoidR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_AvoidR::OnStateEnd()
{
	__super::OnStateEnd();


}



void CVargBossState_AvoidR::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidAttack>(0.05f);
}

void CVargBossState_AvoidR::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_AvoidR::Call_AnimationEnd, this);
}

void CVargBossState_AvoidR::Free()
{

}

_bool CVargBossState_AvoidR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		m_bAttackLookAtLimit = false;
	}

	return false;
}

