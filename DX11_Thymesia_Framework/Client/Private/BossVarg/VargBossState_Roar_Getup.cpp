#include "stdafx.h"
#include "BossVarg/VargBossState_Roar_Getup.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "Status_Monster.h"

GAMECLASS_C(CVargBossState_SPA_Roar_Getup);
CLONE_C(CVargBossState_SPA_Roar_Getup, CComponent)

HRESULT CVargBossState_SPA_Roar_Getup::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_SPA_Roar_Getup::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_SPA_Roar_Getup::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_SPAttack1_Roar_GetUp");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_SPA_Roar_Getup::Call_AnimationEnd, this);
}

void CVargBossState_SPA_Roar_Getup::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_SPA_Roar_Getup::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_SPA_Roar_Getup::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CStatus_Monster> pStatus = m_pOwner.lock()->Get_Component<CStatus_Monster>();

	pStatus.lock()->Set_FullHp(100.f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: SPA_Getup -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_SPA_Roar_Getup::OnStateEnd()
{
	__super::OnStateEnd();


}



void CVargBossState_SPA_Roar_Getup::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_SPA_Run>(0.05f);
}

void CVargBossState_SPA_Roar_Getup::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_SPA_Roar_Getup::Call_AnimationEnd, this);
}

void CVargBossState_SPA_Roar_Getup::Free()
{

}

_bool CVargBossState_SPA_Roar_Getup::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}

