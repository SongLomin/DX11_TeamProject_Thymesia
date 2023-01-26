#include "stdafx.h"
#include "BossVarg/VargBossState_WalkF.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "PhysXController.h"
#include "GameManager.h"
#include "UI_FadeMask.h"

GAMECLASS_C(CVargBossState_WalkF);
CLONE_C(CVargBossState_WalkF, CComponent)

HRESULT CVargBossState_WalkF::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_WalkF::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_WalkF::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_WalkF");

	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_WalkF::Call_AnimationEnd, this, placeholders::_1);

}

void CVargBossState_WalkF::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Rotation_TargetToLookDir();

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}


void CVargBossState_WalkF::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_WalkF::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CVargBossState_Start>().lock())
	{
		m_bOneCheck = true;
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: WalkbB -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_WalkF::OnStateEnd()
{
	__super::OnStateEnd();

	m_bOneCheck = false;
}

//void CVargBossState_WalkF::Call_AnimationEnd(_uint iEndAnimIndex)
//{
//	if (!Get_Enable())
//		return;
//
//	if (m_bOneCheck)
//		Get_OwnerCharacter().lock()->Change_State<CVargBossState_Run>(0.05f);
//}
//
//
//
//
//void CVargBossState_WalkF::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_WalkF::Call_AnimationEnd, this, placeholders::_1);
//}

void CVargBossState_WalkF::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		 m_iPhase2 = 0;
		m_bOneCheck = false;
	}
}

void CVargBossState_WalkF::Free()
{

}

_bool CVargBossState_WalkF::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리


	if (m_bOneCheck)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.99f)
		{
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Run>(0.05f);
			return true;
		}
	}

	else
	{
		if (fPToMDistance > 5.f)
		{
			int iRand = rand() % 2 + m_iPhase2;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidL>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_AvoidR>(0.05f);
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_SPA_Roar>(0.05f);
				break;
			}

			return true;
		}
		else
		{
			int iRand = rand() % 2;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack1a>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack1b>(0.05f);
				break;
			}

			return true;
		}

	}

	
	



	return false;
}

