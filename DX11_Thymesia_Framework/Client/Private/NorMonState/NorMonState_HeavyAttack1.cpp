#include "stdafx.h"
#include "NorMonState/NorMonState_HeavyAttack1.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "MobWeapon.h"




GAMECLASS_C(CNorMonState_HeavyAttack1);
CLONE_C(CNorMonState_HeavyAttack1, CComponent)

HRESULT CNorMonState_HeavyAttack1::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_HeavyAttack1::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_HeavyAttack1::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Attack04|BaseLayer|Arm");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_Attack04");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_ComboA01");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_ComboA01");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Attack04|BaseLayer|Arm");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_Attack03");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmor_Halberds_ComboG01");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_Attack02_1");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_Attack02_1");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmor_Halberds_ComboD01");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmor_Halberds_ComboD01");
		break;

	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_HeavyAttack1::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_HeavyAttack1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_HeavyAttack1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_bAttackLookAtLimit)
	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}




void CNorMonState_HeavyAttack1::OnStateStart(const _float& In_fAnimationBlendTime)
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
				elem.lock()->Set_WeaponDesc(HIT_TYPE::UPPER_HIT, 1.f);
			}
		}
			
			m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
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
		m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
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
		m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
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
		case Client::MONSTERTYPE::SKULLSHIELDMAN:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
			}
		}
		m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
		break;
		case Client::MONSTERTYPE::SKULLSPEARMAN:
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
		case Client::MONSTERTYPE::ARMORSHIELDMAN:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
			}
		}
		m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
		break;
		case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
			}
		}
		m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
		break;
		case Client::MONSTERTYPE::ARMORSPEARMAN:
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
		case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
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
		}


	m_bAttackLookAtLimit = true;
}

void CNorMonState_HeavyAttack1::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);

}


void CNorMonState_HeavyAttack1::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_HeavyAttack1::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_HeavyAttack1::Free()
{

}

_bool CNorMonState_HeavyAttack1::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	// ¶¥->Âî¸£±â




	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
			return true;
		}
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
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

void CNorMonState_HeavyAttack1::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



