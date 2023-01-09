#include "stdafx.h"
#include "BossUrd/UrdBossState_Skill01.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "MobWeapon.h"
#include "JavelinWeapon.h"
#include "UrdWeapon.h"
#include "BossUrd/Urd.h"


GAMECLASS_C(CUrdBossState_Skill01);
CLONE_C(CUrdBossState_Skill01, CComponent)

HRESULT CUrdBossState_Skill01::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Skill01::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Skill01::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Skill01|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Skill01::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Skill01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAtkPermitAndNoHurt)
	{
		TurnAttack(fTimeDelta * 2.f);
	}
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Skill01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Skill01::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bOne = true;

	m_bDisableWeaponCheck = false;

	m_bAttackLookAtLimit = true;

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_PhaseTwoJavlinCount(1);

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(2.5f, 2.5f, 2.5f));

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Skill01::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
}



void CUrdBossState_Skill01::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	if (m_iAnimIndex != iEndAnimIndex)
		return;

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(false);
	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Skill01::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CBossStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CUrdBossState_Skill01::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Skill01::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Skill01::Free()
{

}

_bool CUrdBossState_Skill01::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
		m_bAttackLookAtLimit = false;
	}


	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 42 && m_bOne)
	{
		//여기서 첫번쨰꺼 안보이게하고 두번쨰거 여기서 바인딩하면될듯
		m_bOne = false;
		m_bDisableWeaponCheck = true;

		//for (auto& elem : pWeapons)
		//{
		//	if (elem.lock()->Get_WeaponNum() == 1)
		//	{
		//		elem.lock()->Set_RenderOnOff(false);
		//	}
		//}
		

		pJavelinWeapon = GAMEINSTANCE->Get_GameObject_UseMemoryPool<CJavelinWeapon>(Get_Owner().lock()->Get_CreatedLevel());

		if (!pJavelinWeapon.lock())
		{
			DEBUG_ASSERT;
			return false;
		}


		//땅에꽂는거
		//Get_OwnerMonster()->Get_JavelinWeapon().push_back(GAMEINSTANCE->Add_GameObject<CJavelinWeapon>(m_CreatedLevel));
		pJavelinWeapon.lock()->Set_JavelinState(CJavelinWeapon::JAVELIN_STATE::BIND_HAND);
		pJavelinWeapon.lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_l");
		pJavelinWeapon.lock()->Set_Enable(true);
		pJavelinWeapon.lock()->Set_RenderCheck(true);


	}	

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 44 && !m_bOne && m_bDisableWeaponCheck)
	{
		if (!pJavelinWeapon.lock())
		{
			DEBUG_ASSERT;
			return false;
		}


		pJavelinWeapon.lock()->Set_JavelinState(CJavelinWeapon::JAVELIN_STATE::STAKE);
		pJavelinWeapon.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.0f));
		pJavelinWeapon.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-90.0f));
		
		//weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
		weak_ptr<CUrd> pUrd = Weak_StaticCast<CUrd>(pMonster).lock();
		list<weak_ptr<CMobWeapon>>	pDecoWeapons = pUrd.lock()->Get_DecoWeapons();
		

		for (auto& elem : pDecoWeapons)
		{
			if (!Weak_StaticCast<CUrdWeapon>(elem).lock()->Get_UsingCheck())
			{
				elem.lock()->Set_RenderOnOff(false);
				Weak_StaticCast<CUrdWeapon>(elem).lock()->Set_UsingCheck(true);
				m_bDisableWeaponCheck = false;
				break;
			}
			
		}
			

	
	}
	

	

	return false;
}

