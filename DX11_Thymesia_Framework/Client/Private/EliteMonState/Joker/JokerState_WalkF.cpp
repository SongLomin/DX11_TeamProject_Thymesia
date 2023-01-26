#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_WalkF.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "PhysXController.h"



GAMECLASS_C(CJokerState_WalkF);
CLONE_C(CJokerState_WalkF, CComponent)

HRESULT CJokerState_WalkF::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_WalkF::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_WalkF::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_WalkF");


	/*m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_WalkF::Call_AnimationEnd, this, placeholders::_1);*/
}

void CJokerState_WalkF::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Rotation_TargetToLookDir();

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}


void CJokerState_WalkF::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}



void CJokerState_WalkF::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CJokerState_WalkF::OnStateEnd()
{
	__super::OnStateEnd();


}

void CJokerState_WalkF::Call_AnimationEnd(_uint iEndAnimIndex)
{
}

void CJokerState_WalkF::OnDestroy()
{
}

void CJokerState_WalkF::Free()
{

}

_bool CJokerState_WalkF::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리




	if (fPToMDistance < 4.f)
	{
		int iRand = rand() % 2;

		while (true)
		{
			if (iRand == m_iPreCount)
			{
				iRand = rand() % 2;
				continue;
			}
			else
			{
				break;
			}


		}

		switch (iRand)
		{
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_WheelAtkStart>(0.05f);
			break;
		case 2:
			Get_OwnerCharacter().lock()->Change_State<CJokerState_ComboA1>(0.05f);
			break;

		}
		return true;
	}

	if (fPToMDistance >= 6.f && fPToMDistance < 8.f)
	{
		Get_OwnerCharacter().lock()->Change_State<CJokerState_RunAttackStart>(0.05f);
		return true;
	}


	



	return false;
}

