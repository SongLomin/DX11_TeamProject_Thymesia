#include "stdafx.h"
#include "NorMonState/NorMonState_Awake.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_Awake);
CLONE_C(CNorMonState_Awake, CComponent)

HRESULT CNorMonState_Awake::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Awake::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Awake::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_SP_Idle1_End|BaseLayer");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_SP_Idle1_End");
		break;
	case Client::MONSTERTYPE::SKULL:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_SP_Idle1_End");
		break;
	case Client::MONSTERTYPE::GARDENER:
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_SP_Idle2_End");
			break;
		case Client::NORMONSTERIDLETYPE::SPIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_SP_Idle1_End");
			break;
		}
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmor_Halberds_SP_Idle2_End");
			break;
		case Client::NORMONSTERIDLETYPE::SPIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmor_Halberds_SP_Idle1_End");
			break;
		}
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_SP_Idle2_End");
			break;
		case Client::NORMONSTERIDLETYPE::SPIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_SP_Idle1_End");
			break;
		}
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao||LArmor_Shield_SP_Idle2_End");
			break;
		case Client::NORMONSTERIDLETYPE::SPIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao||LArmor_Shield_SP_Idle1_End");
			break;
		}
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmor_Halberds_SP_Idle2_End");
			break;
		case Client::NORMONSTERIDLETYPE::SPIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmor_Halberds_SP_Idle1_End");
			break;
		}
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmor_Halberds_SP_Idle2_End");
			break;
		case Client::NORMONSTERIDLETYPE::SPIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmor_Halberds_SP_Idle1_End");
			break;
		}
		break;

	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Awake::Call_AnimationEnd, this, placeholders::_1);

}

void CNorMonState_Awake::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_Awake::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();


}



void CNorMonState_Awake::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif


}

void CNorMonState_Awake::OnStateEnd()
{
	__super::OnStateEnd();


}


void CNorMonState_Awake::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Awake::Call_AnimationEnd, this, placeholders::_1);
}


void CNorMonState_Awake::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;
	
	Get_OwnerCharacter().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_AnimIndex(0);
	Get_OwnerCharacter().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_MonIdleType(NORMONSTERIDLETYPE::NORIDLE);
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



_bool CNorMonState_Awake::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}

void CNorMonState_Awake::Free()
{

}