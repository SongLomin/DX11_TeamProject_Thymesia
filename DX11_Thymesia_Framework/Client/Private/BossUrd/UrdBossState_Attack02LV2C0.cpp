#include "stdafx.h"
#include "BossUrd/UrdBossState_Attack02LV2C0.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_Attack02LV2C0);
CLONE_C(CUrdBossState_Attack02LV2C0, CComponent)

HRESULT CUrdBossState_Attack02LV2C0::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Attack02LV2C0::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Attack02LV2C0::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Attack02LV2C0|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Attack02LV2C0::Call_AnimationEnd, this);
}

void CUrdBossState_Attack02LV2C0::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Attack02LV2C0::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Attack02LV2C0::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Attack02LV2C0::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_Attack02LV2C0::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Attack02LV2C0::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Attack02LV2C0::Call_AnimationEnd, this);
}

void CUrdBossState_Attack02LV2C0::Free()
{

}

_bool CUrdBossState_Attack02LV2C0::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

