#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_WalkR.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "PhysXController.h"



GAMECLASS_C(CJokerState_WalkR);
CLONE_C(CJokerState_WalkR, CComponent)

HRESULT CJokerState_WalkR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_WalkR::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_WalkR::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_WalkR");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_WalkR::Call_AnimationEnd, this, placeholders::_1);*/
}

void CJokerState_WalkR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	//m_pTransformCom.lock()->Add_PositionWithRotation(XMVectorSet(m_fCurrentSpeed * fTimeDelta, 0.f, 0.f, 1.f), m_pNaviCom);

	PxControllerFilters Filters;

	m_pPhysXControllerCom.lock()->MoveWithRotation({ m_fCurrentSpeed * fTimeDelta, 0.f, 0.f }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}


void CJokerState_WalkR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}



void CJokerState_WalkR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_WalkR::OnStateEnd()
{
	__super::OnStateEnd();


}



void CJokerState_WalkR::Free()
{

}

_bool CJokerState_WalkR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		int iRand = rand() % 3;

		while (true)
		{
			if (iRand == m_iPreCount)
			{
				iRand = rand() % 3;
				continue;
			}
			else
			{
				break;
			}


		}
		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_ComboA1>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_ShockAttack>(0.05f);
			break;
		case 2:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_WheelAtkStart>(0.05f);
			break;
		}
		return true;
	}


	return false;
}

