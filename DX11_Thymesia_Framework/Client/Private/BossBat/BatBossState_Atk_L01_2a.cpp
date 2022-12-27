#include "stdafx.h"
#include "BossBat/BatBossState_Atk_L01_2a.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "MobWeapon.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_Atk_L01_2a);
CLONE_C(CBatBossState_Atk_L01_2a, CComponent)

HRESULT CBatBossState_Atk_L01_2a::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Atk_L01_2a::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Atk_L01_2a::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_AttackL_01_2a");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Atk_L01_2a::Call_AnimationEnd, this);
}

void CBatBossState_Atk_L01_2a::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bRootStop)
	{
		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

		PxControllerFilters Filters = Filters;
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);	
	}

	
	if (m_bTurnTuning)
	{
		_float fTurnValue = 1.77f / 2.4;

		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -fTurnValue);
	}
	

	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Atk_L01_2a::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_Atk_L01_2a::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bRootStop = true;

	m_bOne = true;

	m_bTurnTuning = false;
	
	m_bAttackLookAtLimit = true;

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.5f);
	}

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_Atk_L01_2a::OnStateEnd()
{
	__super::OnStateEnd();

}



void CBatBossState_Atk_L01_2a::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_Owner().lock()->Get_Component<CBatBossState_AttackIdle>().lock()->Set_AttackCount(1);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Atk_L01_2a::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Atk_L01_2a::Call_AnimationEnd, this);
}

void CBatBossState_Atk_L01_2a::Free()
{

}

_bool CBatBossState_Atk_L01_2a::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	_float fPToMDistance = Get_DistanceWithPlayer();

	if (fPToMDistance <= 7.f && m_bOne)
	{
		m_bRootStop = false;
		m_bOne = false;
	}


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 144 &&
		m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() < 288)
	{
		m_bTurnTuning = true;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 288)
	{
		m_bTurnTuning = false;
	}



	return false;
}

