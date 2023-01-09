#include "stdafx.h"
#include "BossUrd/UrdBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_Skill_Idle);
CLONE_C(CUrdBossState_Skill_Idle, CComponent)

HRESULT CUrdBossState_Skill_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Skill_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Skill_Idle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Idle|BaseLayer");


}

void CUrdBossState_Skill_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Skill_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Skill_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Skill_Idle::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_Skill_Idle::Free()
{

}

_bool CUrdBossState_Skill_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	//거리가 가까우면 뒤로 거리가멀면 

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	if (fPToMDistance <= 4.5f)
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill02_1>(0.05f);
		return true;
	}
	
	if (fPToMDistance > 3.5f)
	{
		

		_uint  iRand = rand() % 1;

		switch (iRand)
		{
		case 0:
		{
			_int iResulut = ComputeDirectionToPlayer();

			switch (iResulut)
			{
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill03_L>(0.05f);
				break;
			case -1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill03_R>(0.05f);
				break;
			}
		}
			break;
	case 1:
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill01>(0.05f);
		break;
		}


		return true;
	}

	return false;
}

