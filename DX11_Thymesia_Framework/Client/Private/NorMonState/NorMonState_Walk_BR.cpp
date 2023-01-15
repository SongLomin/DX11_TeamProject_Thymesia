#include "stdafx.h"
#include "NorMonState/NorMonState_Walk_BR.h"
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


GAMECLASS_C(CNorMonState_Walk_BR);
CLONE_C(CNorMonState_Walk_BR, CComponent)

HRESULT CNorMonState_Walk_BR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Walk_BR::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	
	return S_OK;
}

void CNorMonState_Walk_BR::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkBL|BaseLayer|Armat");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_WalkB");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_WalkBR");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_WalkBR");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkBL|BaseLayer|Armat");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_WalkBR");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_WalkBR");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_WalkBR");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_WalkBR");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_WalkBR");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_WalkBR");
		break;
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Walk_BR::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_Walk_BR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	

	PxControllerFilters Filters;
	//m_pTransformCom.lock()->Add_PositionWithRotation(XMVectorSet(m_fCurrentSpeed * fTimeDelta , 0.f, -m_fCurrentSpeed * fTimeDelta,1.f), m_pNaviCom);
	m_pPhysXControllerCom.lock()->MoveWithRotation({ m_fCurrentSpeed * fTimeDelta, 0.f, -m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}

void CNorMonState_Walk_BR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_Walk_BR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif


}

void CNorMonState_Walk_BR::OnStateEnd()
{
	__super::OnStateEnd();

	
}



void CNorMonState_Walk_BR::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}

void CNorMonState_Walk_BR::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Walk_BR::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_Walk_BR::Free()
{

}

_bool CNorMonState_Walk_BR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}

