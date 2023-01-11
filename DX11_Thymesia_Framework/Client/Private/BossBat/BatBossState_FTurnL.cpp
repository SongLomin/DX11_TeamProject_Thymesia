#include "stdafx.h"
#include "BossBat/BatBossState_FTurnL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_FTurnL);
CLONE_C(CBatBossState_FTurnL, CComponent)

HRESULT CBatBossState_FTurnL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_FTurnL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_FTurnL::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_FTurn_L");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_FTurnL::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_FTurnL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(-vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
	
	if (m_bTurnCheck)
	{
		_float fTurnValue = 3.14f / 0.2f;
	
		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue * -0.5f);
		
	};
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_FTurnL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}



void CBatBossState_FTurnL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	//m_pPhysXControllerCom.lock()->Enable_Gravity(true);

	m_bTurnCheck = false;

	Rotation_TargetToLookDir();
	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif


}	


void CBatBossState_FTurnL::OnStateEnd()
{
	__super::OnStateEnd();

}



void CBatBossState_FTurnL::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_FTurnL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_FTurnL::Call_AnimationEnd, this, placeholders::_1);
}



void CBatBossState_FTurnL::Free()
{

}

_bool CBatBossState_FTurnL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;





	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() == 12)
	{
		m_bTurnCheck = true;
	}
	

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 22)
	{

		if (ComputeAngleWithPlayer() > 0.97f && m_bTurnCheck)
		{
			Rotation_TargetToLookDir();
			m_bTurnCheck = false;

		}
	}

	//12프레임부터 돌기시작
	//29프레임에서돌기끝


	return false;
}

