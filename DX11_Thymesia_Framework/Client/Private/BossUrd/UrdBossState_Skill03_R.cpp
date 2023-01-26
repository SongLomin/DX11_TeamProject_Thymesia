#include "stdafx.h"
#include "BossUrd/UrdBossState_Skill03_R.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "JavelinWeapon.h"
#include "MobWeapon.h"
#include "UrdWeapon.h"

GAMECLASS_C(CUrdBossState_Skill03_R);
CLONE_C(CUrdBossState_Skill03_R, CComponent)

void CUrdBossState_Skill03_R::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_iKeyIndex)
	{
	case 54:
		m_bHandAttachKeyIndexCheck = true;
		break;
	case 56:
		m_bHandDettachKeyIndexCheck = true;
		break;
	}
}

HRESULT CUrdBossState_Skill03_R::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Skill03_R::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Skill03_R::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Skill03_R|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Skill03_R::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Skill03_R::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAtkPermitAndNoHurt)
	{
		TurnAttack(fTimeDelta * 2.f);
	}
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Skill03_R::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Skill03_R::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(2.f, 2.f, 2.f));


	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_PhaseTwoJavlinCount(1);

	m_bAttackLookAtLimit = true;

	m_bHandAttachKeyIndexCheck = false;

	m_bHandDettachKeyIndexCheck = false;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CUrdBossState_Skill03_R::Call_NextAnimationKey, this, placeholders::_1);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Skill03_R::OnStateEnd()
{
	__super::OnStateEnd();


	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CUrdBossState_Skill03_R::Call_NextAnimationKey, this, placeholders::_1);

	m_bHandAttachKeyIndexCheck = false;

	m_bHandDettachKeyIndexCheck = false;

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
}



void CUrdBossState_Skill03_R::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	if (m_iAnimIndex != iEndAnimIndex)
		return;
	
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(false);
	
	switch (m_eReuslt)
	{
	case Client::CUrdBossState_Skill03_R::RESULT_LEFT:
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Equip_L>(0.05f);
		break;
	case Client::CUrdBossState_Skill03_R::RESULTR_RIGHT:
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Equip_R>(0.05f);
		break;
	}

}

void CUrdBossState_Skill03_R::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CBossStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CUrdBossState_Skill03_R::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Skill03_R::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Skill03_R::Free()
{

}

_bool CUrdBossState_Skill03_R::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();


	if (m_bHandAttachKeyIndexCheck)
	{
		//여기서 첫번쨰꺼 안보이게하고 두번쨰거 여기서 바인딩하면될듯
		m_bHandAttachKeyIndexCheck = false;

		pJavelinWeapon = GAMEINSTANCE->Get_GameObject_UseMemoryPool<CJavelinWeapon>(Get_Owner().lock()->Get_CreatedLevel());


		if (!pJavelinWeapon.lock())
		{
			DEBUG_ASSERT;
			return false;
		}


		
		//Get_OwnerMonster()->Get_JavelinWeapon().push_back(GAMEINSTANCE->Add_GameObject<CJavelinWeapon>(m_CreatedLevel));
		pJavelinWeapon.lock()->Set_JavelinState(CJavelinWeapon::JAVELIN_STATE::BIND_HAND);
		pJavelinWeapon.lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
		pJavelinWeapon.lock()->Set_Enable(true);
		pJavelinWeapon.lock()->Set_RenderCheck(true);
		//Get_OwnerMonster()->Get_JavelinWeapon().back().lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.0f));

	}

	if (m_bHandDettachKeyIndexCheck)
	{
		if (!pJavelinWeapon.lock())
		{
			DEBUG_ASSERT;
			return false;
		}


		pJavelinWeapon.lock()->Set_JavelinState(CJavelinWeapon::JAVELIN_STATE::THROW);
		m_bHandDettachKeyIndexCheck = false;
		//pJavelinWeapon.lock()->LookAt_Player();
	

		//weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
		weak_ptr<CUrd> pUrd = Weak_StaticCast<CUrd>(pMonster).lock();
		list<weak_ptr<CMobWeapon>>	pDecoWeapons = pUrd.lock()->Get_DecoWeapons();

		pWeapons.front().lock()->Set_RenderOnOff(false);


		for (auto& elem : pDecoWeapons)
		{
 			if (!Weak_StaticCast<CUrdWeapon>(elem).lock()->Get_UsingCheck())
			{
				_uint WeaponNum = Weak_StaticCast<CUrdWeapon>(elem).lock()->Get_WeaponNum();

				switch (WeaponNum)
				{
				case 1:
					m_eReuslt = RESULTR_RIGHT;
					break;
				case 2:
					m_eReuslt = RESULT_LEFT;
					break;
				case 3:
					m_eReuslt = RESULT_LEFT;
					break;
				}
				break;
			}
			
		}
	
	}
	return false;
}

