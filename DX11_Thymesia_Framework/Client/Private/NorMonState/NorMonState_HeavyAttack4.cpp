#include "stdafx.h"
#include "NorMonState/NorMonState_HeavyAttack4.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "MobWeapon.h"


GAMECLASS_C(CNorMonState_HeavyAttack4);
CLONE_C(CNorMonState_HeavyAttack4, CComponent)

HRESULT CNorMonState_HeavyAttack4::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_HeavyAttack4::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_HeavyAttack4::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_ComboC02");
		break;
	}



	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_HeavyAttack4::Call_AnimationEnd, this, placeholders::_1);


}

void CNorMonState_HeavyAttack4::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_HeavyAttack4::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();

}




void CNorMonState_HeavyAttack4::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif

		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::ARMORSPEARMAN:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.5f);
			}
		}
		m_pModelCom.lock()->Set_AnimationSpeed(2.5f);
		break;
		}


	m_bAttackLookAtLimit = true;
}

void CNorMonState_HeavyAttack4::OnStateEnd()
{
	__super::OnStateEnd();


	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}


void CNorMonState_HeavyAttack4::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_HeavyAttack4::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_HeavyAttack4::Free()
{

}

_bool CNorMonState_HeavyAttack4::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		m_bAttackLookAtLimit = false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.55f)
	{
		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::ARMORSPEARMAN:
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack5>(0.05f);
			break;
		}
		return true;
	}



	return false;

}

void CNorMonState_HeavyAttack4::Call_AnimationEnd(_uint iEndAnmIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



