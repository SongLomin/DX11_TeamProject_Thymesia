#include "stdafx.h"
#include "BossVarg/VargBossState_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_Start);
CLONE_C(CVargBossState_Start, CComponent)

HRESULT CVargBossState_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Start::Start()
{
	__super::Start();

	switch (m_eBossStartType)
	{
	case Client::BOSSSTARTTYPE::BEGINSTART:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_Seq_TutorialBossFightStart");
		break;
	case Client::BOSSSTARTTYPE::NORMALSTART:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_Seq_BossFightStart");
		break;
	}


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Start::Call_AnimationEnd, this);
}

void CVargBossState_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif
	m_pModelCom.lock()->Set_AnimationSpeed(4.f);

}

void CVargBossState_Start::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}



void CVargBossState_Start::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;
	
	
}

void CVargBossState_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Start::Call_AnimationEnd, this);
}

void CVargBossState_Start::Free()
{

}

_bool CVargBossState_Start::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	if (fPToMDistance <= 8.f)
	{
		m_bNextState = true;
	}


	

	switch (m_eBossStartType)
	{
	case Client::BOSSSTARTTYPE::BEGINSTART:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.15f && !m_bNextState)
		{
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Start>(0.05f);
			return true;
		}
		break;
	case Client::BOSSSTARTTYPE::NORMALSTART:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.15f && !m_bNextState)
		{
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Start>(0.05f);
			return true;
		}
		break;
	}

	if (m_bNextState)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.99f)
		{
			TurnMechanism();
			return true;
		}
		
	}

	

	return false;
}

