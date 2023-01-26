#include "stdafx.h"
#include "NorMonState/NorMonState_Parry.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_Parry);
CLONE_C(CNorMonState_Parry, CComponent)

HRESULT CNorMonState_Parry::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Parry::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Parry::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_Parry|BaseLayer|Armature");
		break;
	}



	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Parry::Call_AnimationEnd, this, placeholders::_1);


}

void CNorMonState_Parry::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_Parry::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Rotation_TargetToLookDir();

	Check_AndChangeNextState();

}

void CNorMonState_Parry::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	__super::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}




void CNorMonState_Parry::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(6.f);
}

void CNorMonState_Parry::OnStateEnd()
{
	__super::OnStateEnd();


	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}


void CNorMonState_Parry::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Parry::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_Parry::Free()
{

}

_bool CNorMonState_Parry::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.2f)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack3>(0.05f);
	}

	return false;

}

void CNorMonState_Parry::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	
}



