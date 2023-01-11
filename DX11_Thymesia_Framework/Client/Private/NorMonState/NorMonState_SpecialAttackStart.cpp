#include "stdafx.h"
#include "NorMonState/NorMonState_SpecialAttackStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "MobWeapon.h"


GAMECLASS_C(CNorMonState_SpecialAttackStart);
CLONE_C(CNorMonState_SpecialAttackStart, CComponent)

HRESULT CNorMonState_SpecialAttackStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_SpecialAttackStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_SpecialAttackStart::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_SP_Attack01_Start");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_SP_Attack01_Start");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_SP_Attack01_Start");
		break;
	}



	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_SpecialAttackStart::Call_AnimationEnd, this, placeholders::_1);


}

void CNorMonState_SpecialAttackStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Turn_ToThePlayer(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_SpecialAttackStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();

}




void CNorMonState_SpecialAttackStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_

#endif
#endif

	m_bAttackLookAtLimit = true;
}

void CNorMonState_SpecialAttackStart::OnStateEnd()
{
	__super::OnStateEnd();


	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}


void CNorMonState_SpecialAttackStart::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_SpecialAttackStart::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_SpecialAttackStart::Free()
{

}

_bool CNorMonState_SpecialAttackStart::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 29)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_SpecialAttackLoop>(0.05f);
		return true;
	}

	//29프레임에서 ㄱㄱ? ㅇㅋ
	return false;

}

void CNorMonState_SpecialAttackStart::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CNorMonState_SpecialAttackLoop>(0.05f);
}




