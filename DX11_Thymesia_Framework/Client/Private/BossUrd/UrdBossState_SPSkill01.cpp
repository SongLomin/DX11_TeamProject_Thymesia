#include "stdafx.h"
#include "BossUrd/UrdBossState_SPSkill01.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "MobWeapon.h"
#include "UrdWeapon.h"
#include "JavelinWeapon.h"

GAMECLASS_C(CUrdBossState_SPSkill01);
CLONE_C(CUrdBossState_SPSkill01, CComponent)

HRESULT CUrdBossState_SPSkill01::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_SPSkill01::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_SPSkill01::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_SPSkill01|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_SPSkill01::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_SPSkill01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_SPSkill01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_SPSkill01::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	m_iResetWeaponNum = 0;

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillCount(0);
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_ZeroPhaseTwoJavlinCount(0);
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_TurnCheck(true);
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SpecailAttack(false);
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_Attack(false);
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_WalkStart(false);
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(false);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_SPSkill01::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_SPSkill01::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_SPSkill01::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_SPSkill01::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_SPSkill01::Free()
{

}

_bool CUrdBossState_SPSkill01::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	weak_ptr<CUrd> pUrd = Weak_StaticCast<CUrd>(pMonster).lock();
	list<weak_ptr<CJavelinWeapon>> pJavelinWeapons = pUrd.lock()->Get_JavelinWepons();
	list<weak_ptr<CMobWeapon>>	pDecoWeapons = pUrd.lock()->Get_DecoWeapons();

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 250 &&
		m_iResetWeaponNum == 0)
	{
		m_iResetWeaponNum = 1;
			
		for (auto& elem : pJavelinWeapons)
		{
			if (elem.lock()->Get_RenderCheck())
			{
				elem.lock()->Set_RenderCheck(false);
				elem.lock()->Set_Enable(false);
				break;
			}
		
		}	
		
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 275 &&
		m_iResetWeaponNum == 1)
	{
		m_iResetWeaponNum = 2;

		for (auto& elem : pJavelinWeapons)
		{
			if (elem.lock()->Get_RenderCheck())
			{
				elem.lock()->Set_RenderCheck(false);
				elem.lock()->Set_Enable(false);
				break;
			}
		
		}

	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 300 &&
		m_iResetWeaponNum == 2)
	{
		m_iResetWeaponNum = 3;

		for (auto& elem : pJavelinWeapons)
		{
			if (elem.lock()->Get_RenderCheck())
			{
				elem.lock()->Set_RenderCheck(false);
				elem.lock()->Set_Enable(false);
				break;
			}
			
		}

	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 368 &&
		m_iResetWeaponNum == 3)
	{
		

		for (auto& elem : pDecoWeapons)
		{
			elem.lock()->Set_RenderOnOff(true);
			Weak_StaticCast<CUrdWeapon>(elem).lock()->Set_UsingCheck(false);

		}

	}



	return false;
}

