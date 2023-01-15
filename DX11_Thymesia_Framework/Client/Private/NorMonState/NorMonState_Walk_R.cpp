#include "stdafx.h"
#include "NorMonState/NorMonState_Walk_R.h"
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


GAMECLASS_C(CNorMonState_Walk_R);
CLONE_C(CNorMonState_Walk_R, CComponent)

HRESULT CNorMonState_Walk_R::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Walk_R::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Walk_R::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkR|BaseLayer|Armatu");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_WalkR");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_WalkR");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_WalkR");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkR|BaseLayer|Armatu");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_WalkR");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_WalkR");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_WalkR");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_WalkR");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_WalkR");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_WalkR");
		break;

	}



	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Walk_R::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_Walk_R::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	//m_pTransformCom.lock()->Add_PositionWithRotation(XMVectorSet(m_fCurrentSpeed * fTimeDelta, 0.f, 0.f, 1.f), m_pNaviCom);

	PxControllerFilters Filters;

	m_pPhysXControllerCom.lock()->MoveWithRotation({ m_fCurrentSpeed * fTimeDelta, 0.f, 0.f }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}

void CNorMonState_Walk_R::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_bAttackLookAtLimit)
	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_Walk_R::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);

	m_bAttackLookAtLimit = true;
}

void CNorMonState_Walk_R::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}



void CNorMonState_Walk_R::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}

void CNorMonState_Walk_R::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Walk_R::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_Walk_R::Free()
{

}

_bool CNorMonState_Walk_R::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.99f)
	{
		m_bAttackLookAtLimit = false;
	}

	return false;

}

