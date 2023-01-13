#include "stdafx.h"
#include "NorMonState/NorMonState_HurtR.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_HurtR);
CLONE_C(CNorMonState_HurtR, CComponent)

HRESULT CNorMonState_HurtR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_HurtR::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_HurtR::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtS_FR|BaseLayer|Arm");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_HurtM_FR");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtM_FR");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtM_FR");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtS_FR|BaseLayer|Arm");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_HurtS_FR");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_HurtS_FR");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_HurtS_FR");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_HurtS_FR");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_HurtS_FR");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_HurtS_FR");
		break;
		
	}



	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_HurtR::Call_AnimationEnd, this, placeholders::_1);


}

void CNorMonState_HurtR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_HurtR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();

}




void CNorMonState_HurtR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
	{
		_uint iRand = rand() % 3 + 4;
		m_iParryCount += iRand;
		Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ParryCount(iRand);
	}	
	break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
	{
		_uint iRand = rand() % 3 + 4;
		m_iParryCount += iRand;
		Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ParryCount(iRand);
	}
	break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
	{
		_uint iRand = rand() % 3 + 4;
		m_iParryCount += iRand;
		Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ParryCount(iRand);
		
	}
	break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
	{
		_uint iRand = rand() % 3 + 4;
		m_iParryCount += iRand;
		Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ParryCount(iRand);
	
	}
	break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
	{
		_uint iRand = rand() % 3 + 4;
		m_iParryCount += iRand;
		Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ParryCount(iRand);

	}
	break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN :
	{
		_uint iRand = rand() % 3 + 4;
		m_iParryCount += iRand;
		Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ParryCount(iRand);

	}
	break;
	}


	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(2.f);
}

void CNorMonState_HurtR::OnStateEnd()
{
	__super::OnStateEnd();
	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}


void CNorMonState_HurtR::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_HurtR::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_HurtR::Free()
{

}

_bool CNorMonState_HurtR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		if (m_iParryCount >= 10)
		{
			switch (m_eMonType)
			{
			case Client::MONSTERTYPE::AXEMAN:
				Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ZeroParryCount(0);
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
				m_iParryCount = 0;
				break;
			case Client::MONSTERTYPE::SHIELDAXEMAN:
				Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ZeroParryCount(0);
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Parry>(0.05f);
				m_iParryCount = 0;
				break;
			case Client::MONSTERTYPE::ARMORSHIELDMAN:
				Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ZeroParryCount(0);
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack3>(0.05f);
				m_iParryCount = 0;
				break;
			case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
				Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ZeroParryCount(0);
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack3>(0.05f);
				m_iParryCount = 0;
				break;
			case Client::MONSTERTYPE::ARMORSPEARMAN:
				Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ZeroParryCount(0);
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
				m_iParryCount = 0;
				break;
			case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
				Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock()->Set_ZeroParryCount(0);
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
				m_iParryCount = 0;
				break;

			}

		}
		return true;
	}

	return false;

}

void CNorMonState_HurtR::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



