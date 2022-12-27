#include "stdafx.h"
#include "BossBat/BatBossState_Bite_2.h"
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

GAMECLASS_C(CBatBossState_Bite_2);
CLONE_C(CBatBossState_Bite_2, CComponent)

HRESULT CBatBossState_Bite_2::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Bite_2::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Bite_2::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Bite_2");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Bite_2::Call_AnimationEnd, this);
}

void CBatBossState_Bite_2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_bRootStop)
	{
		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

		PxControllerFilters Filters = Filters;
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
	}

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Bite_2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_Bite_2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_bRootStop = true;

	m_bOne = true;


	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.5f);
	}
	
	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Bite_1>().lock())
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 88);
	}
	else
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	}

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif


}	


void CBatBossState_Bite_2::OnStateEnd()
{
	__super::OnStateEnd();


}



void CBatBossState_Bite_2::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CBatBossState_AttackIdle>().lock()->Set_AttackCount(1);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Bite_2::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Bite_2::Call_AnimationEnd, this);
}

void CBatBossState_Bite_2::Free()
{

}

_bool CBatBossState_Bite_2::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer();

	if (fPToMDistance <= 8.f && m_bOne)
	{
		m_bRootStop = false;
		m_bOne = false;
	}


	if (ComputeAngleWithPlayer() > 0.98f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
		m_bAttackLookAtLimit = false;
	}

	return false;
}

