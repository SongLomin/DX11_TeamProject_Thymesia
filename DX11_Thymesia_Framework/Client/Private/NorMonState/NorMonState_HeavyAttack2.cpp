#include "stdafx.h"
#include "NorMonState/NorMonState_HeavyAttack2.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "MobWeapon.h"
#include "PhysXController.h"


GAMECLASS_C(CNorMonState_HeavyAttack2);
CLONE_C(CNorMonState_HeavyAttack2, CComponent)

HRESULT CNorMonState_HeavyAttack2::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_HeavyAttack2::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_HeavyAttack2::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Attack05|BaseLayer|Arm");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_Attack05");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_ComboB01");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_ComboB01");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Attack05|BaseLayer|Arm");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_Attack02_2");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_Attack02_2");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_ComboA03");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_ComboA03");
		break;
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_HeavyAttack2::Call_AnimationEnd, this, placeholders::_1);


}

void CNorMonState_HeavyAttack2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	PxControllerFilters Filters;

	//Turn_Transform(fTimeDelta);
	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::GARDENER:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 20 &&
			m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() <= 51)
		{
			m_fCurrentSpeed += m_fAccel * fTimeDelta;
			m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

			m_pModelCom.lock()->Play_Animation(fTimeDelta);
			m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
		}
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 20 &&
			m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() <= 51)
		{
			m_fCurrentSpeed += m_fAccel * fTimeDelta;
			m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

			m_pModelCom.lock()->Play_Animation(fTimeDelta);
			m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
		}
		break;
	}


	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_HeavyAttack2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();


}




void CNorMonState_HeavyAttack2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif

		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::AXEMAN:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
			}
		}
			m_pModelCom.lock()->Set_AnimationSpeed(2.f);
			break;
		case Client::MONSTERTYPE::KNIFEWOMAN:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
			}
			m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
		}
			
			m_bAttackLookAtLimit = true;
			break;
		case Client::MONSTERTYPE::SKULL:
			break;
		case Client::MONSTERTYPE::GARDENER:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
			}
		}
		m_pModelCom.lock()->Set_AnimationSpeed(2.f);
		break;
		case Client::MONSTERTYPE::ENHANCE_GARDENER:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
			}
		}
		m_pModelCom.lock()->Set_AnimationSpeed(2.f);
		break;
		case Client::MONSTERTYPE::SHIELDAXEMAN:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
			}
		}
			m_pModelCom.lock()->Set_AnimationSpeed(2.f);
			break;
		case Client::MONSTERTYPE::ARMORSPEARMAN:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NOPARRYATTACK, 1.3f);
			}
		}
		m_pModelCom.lock()->Set_AnimationSpeed(2.f);
		break;
		case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NOPARRYATTACK, 1.3f);
			}
		}
		m_pModelCom.lock()->Set_AnimationSpeed(2.f);
		break;
		}

		m_bAttackLookAtLimit = true;

}

void CNorMonState_HeavyAttack2::OnStateEnd()
{
	__super::OnStateEnd();

	
	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}


void CNorMonState_HeavyAttack2::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_HeavyAttack2::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_HeavyAttack2::Free()
{

}

_bool CNorMonState_HeavyAttack2::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		m_bAttackLookAtLimit = false;
	}

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::GARDENER:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 51)
		{
			m_bAttackLookAtLimit = false;
		}
	break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 51)
		{
			m_bAttackLookAtLimit = false;
		}
		break;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 51)
	{
		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::GARDENER:
		m_bAttackLookAtLimit = false;
		break;
		case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_bAttackLookAtLimit = false;
		break;
		}
		return true;
	}

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		{
			int iRand = rand() % 3;
			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack3>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
				break;
			}	
			return true;
		}
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		{
			int iRand = rand() % 3;
			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack3>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
				break;
			}
			return true;
		}
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
			return true;
		}
		break;
	default:
		break;
	}



	return false;

}

void CNorMonState_HeavyAttack2::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



