#include "stdafx.h"
#include "NorMonState/NorMonState_GroggyLoop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "PhysXController.h"

GAMECLASS_C(CNorMonState_GroggyLoop);
CLONE_C(CNorMonState_GroggyLoop, CComponent)

HRESULT CNorMonState_GroggyLoop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_GroggyLoop::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_StateFlag |= (_uint)STATE_FLAG::EXECUTABLE;

	return S_OK;
}

void CNorMonState_GroggyLoop::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtStunLoop|BaseLayer");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV2Villager01_F_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtStunLoop|BaseLayer");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_HurtStunLoop");
		break;
	}



	m_fLoopEndTime = 0.f;

}

void CNorMonState_GroggyLoop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fLoopEndTime += fTimeDelta;

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_GroggyLoop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();


}



void CNorMonState_GroggyLoop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: Stop -> StopStop" << endl;
#endif
#endif


}

void CNorMonState_GroggyLoop::OnStateEnd()
{

	m_bNextStatepause = false;

	__super::OnStateEnd();
	
}





_bool CNorMonState_GroggyLoop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_bNextStatepause)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() > m_iCurrentKeyIndex)
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_TakeExecution>(0.05f);
			return true;
		}
	}


	if (m_fLoopEndTime >= 7.f)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_GroggyEnd>(0.05f);
		m_fLoopEndTime = 0.f; 
		return true;
	}


	return false;
}

void CNorMonState_GroggyLoop::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if (!Get_Enable())
		return;

	if ((_uint)EVENT_TYPE::ON_ARMOREXECUTIONSTART == iArg)
	{
		m_bNextStatepause = true;
		m_iCurrentKeyIndex = m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex();
		m_iCurrentKeyIndex += 1;
		//Get_OwnerCharacter().lock()->Change_State<CNorMonState_TakeExecution>(0.05f);
	}
}

void CNorMonState_GroggyLoop::Free()
{

}