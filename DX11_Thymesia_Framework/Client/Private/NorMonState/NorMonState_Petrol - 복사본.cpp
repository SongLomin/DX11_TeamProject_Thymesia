#include "stdafx.h"
#include "NorMonState/NorMonState_Suprise_Attack.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "PhysXController.h"



GAMECLASS_C(CNorMonState_Suprise_Attack);
CLONE_C(CNorMonState_Suprise_Attack, CComponent)



HRESULT CNorMonState_Suprise_Attack::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Suprise_Attack::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	

	return S_OK;
}

void CNorMonState_Suprise_Attack::Start()
{
	__super::Start();



	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkF|BaseLayer|Armatu");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_WalkF");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_WalkF");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_WalkF");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkF|BaseLayer|Armatu");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_WalkF");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_WalkF");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_WalkF");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_WalkF");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_WalkF");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_WalkF");
		break;

	}

	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Suprise_Attack::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_Suprise_Attack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);


}

void CNorMonState_Suprise_Attack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_Suprise_Attack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: RunStart -> Walk_F" << endl;
#endif
#endif



	
}

void CNorMonState_Suprise_Attack::OnStateEnd()
{
	__super::OnStateEnd();

	
}



_bool CNorMonState_Suprise_Attack::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fDistance = Get_DistanceWithPlayer();

	if (fDistance <= 2.f)
	{
		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::AXEMAN:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::KNIFEWOMAN:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::SKULL:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::GARDENER:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::SHIELDAXEMAN:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::ENHANCE_GARDENER:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::SKULLSHIELDMAN:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::SKULLSPEARMAN:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::ARMORSHIELDMAN:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::ARMORSPEARMAN:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;
		case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
		}
			break;

		}
		return true;
	}



	return false;
}


void CNorMonState_Suprise_Attack::Free()
{

}


