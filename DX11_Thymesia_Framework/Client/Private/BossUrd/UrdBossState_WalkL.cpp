#include "stdafx.h"
#include "BossUrd/UrdBossState_WalkL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "PhysXController.h"

GAMECLASS_C(CUrdBossState_WalkL);
CLONE_C(CUrdBossState_WalkL, CComponent)

HRESULT CUrdBossState_WalkL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_WalkL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_WalkL::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_WalkL|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_WalkL::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_WalkL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Rotation_TargetToLookDir();
	
	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);
	_vector vDirLook = Get_CurMonToStartMonDir();

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	PxControllerFilters Filters;

	m_pPhysXControllerCom.lock()->MoveWithRotation({ -m_fCurrentSpeed * fTimeDelta, 0.f, 0.f }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}


void CUrdBossState_WalkL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_WalkL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_WalkL::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_WalkL::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_WalkL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_WalkL::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_WalkL::Free()
{

}

_bool CUrdBossState_WalkL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

