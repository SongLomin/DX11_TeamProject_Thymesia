#include "stdafx.h"
#include "NorMonState/NorMonState_LightAttack2.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "MobWeapon.h"



GAMECLASS_C(CNorMonState_LightAttack2);
CLONE_C(CNorMonState_LightAttack2, CComponent)

HRESULT CNorMonState_LightAttack2::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_LightAttack2::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_LightAttack2::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Attack03|BaseLayer|Arm");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_Attack03");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		break;
	}



	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_LightAttack2::Call_AnimationEnd, this);
}

void CNorMonState_LightAttack2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_LightAttack2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_bAttackLookAtLimit)
	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}




void CNorMonState_LightAttack2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Attack2 -> OnStateStart" << endl;
#endif
#endif

		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::AXEMAN:
		{
			weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

			list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();

			for (auto& elem : pWeapons)
			{
				elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);
			}
		}
			m_pModelCom.lock()->Set_AnimationSpeed(2.f);
			break;
		case Client::MONSTERTYPE::KNIFEWOMAN:
			m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
			break;
		case Client::MONSTERTYPE::SKULL:
			break;
		case Client::MONSTERTYPE::GARDENER:
			break;
		}


	m_bAttackLookAtLimit = true;
}

void CNorMonState_LightAttack2::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);

}


void CNorMonState_LightAttack2::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_LightAttack2::Call_AnimationEnd, this);
}

void CNorMonState_LightAttack2::Free()
{

}

_bool CNorMonState_LightAttack2::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		m_bAttackLookAtLimit = false;
	}

	return false;

}

void CNorMonState_LightAttack2::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}




