#include "stdafx.h"
#include "NorMonState/NorMonState_SpecialAttackEnd.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "MobWeapon.h"


GAMECLASS_C(CNorMonState_SpecialAttackEnd);
CLONE_C(CNorMonState_SpecialAttackEnd, CComponent)

HRESULT CNorMonState_SpecialAttackEnd::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_SpecialAttackEnd::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_SpecialAttackEnd::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_SP_Attack01_End");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_SP_Attack01_End");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_SP_Attack01_End");
		break;
	}



	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_SpecialAttackEnd::Call_AnimationEnd, this, placeholders::_1);


}

void CNorMonState_SpecialAttackEnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		Turn_ToThePlayer(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_SpecialAttackEnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	

	Check_AndChangeNextState();

}




void CNorMonState_SpecialAttackEnd::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_

#endif
#endif
	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);


	m_bAttackLookAtLimit = true;
}

void CNorMonState_SpecialAttackEnd::OnStateEnd()
{
	__super::OnStateEnd();


	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}


void CNorMonState_SpecialAttackEnd::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_SpecialAttackEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_SpecialAttackEnd::Free()
{

}

_bool CNorMonState_SpecialAttackEnd::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f && m_bAttackLookAtLimit)
	{
		m_bAttackLookAtLimit = false;
	}


	return false;

}

void CNorMonState_SpecialAttackEnd::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



