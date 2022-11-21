#include "stdafx.h"
#include "NorMonState/NorMonState_HurtL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_HurtL);
CLONE_C(CNorMonState_HurtL, CComponent)

HRESULT CNorMonState_HurtL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_HurtL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_HurtL::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtS_FL|BaseLayer|Arm");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_HurtM_FL");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtM_FL");
		break;
	}



	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_HurtL::Call_AnimationEnd, this);


}

void CNorMonState_HurtL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_HurtL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	Check_AndChangeNextState();

}




void CNorMonState_HurtL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(2.f);

	m_bAttackLookAtLimit = false;
}

void CNorMonState_HurtL::OnStateEnd()
{
	__super::OnStateEnd();

	m_bAttackLookAtLimit = false;
	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}


void CNorMonState_HurtL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_HurtL::Call_AnimationEnd, this);
}

void CNorMonState_HurtL::Free()
{

}

_bool CNorMonState_HurtL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.1f)
	{
		m_bAttackLookAtLimit = true;
	}

	return false;

}

void CNorMonState_HurtL::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



