#include "stdafx.h"
#include "NorMonState/NorMonState_Idle2Fight.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_Idle2Fight);
CLONE_C(CNorMonState_Idle2Fight, CComponent)

HRESULT CNorMonState_Idle2Fight::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Idle2Fight::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Idle2Fight::Start()
{
	__super::Start();

	switch (m_eMonType)
	{

	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::RUNATTACKIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_IdleN2Fight");
			break;
		}
	}
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Idle2Fight::Call_AnimationEnd, this, placeholders::_1);

}

void CNorMonState_Idle2Fight::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_Idle2Fight::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();


}



void CNorMonState_Idle2Fight::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif


}

void CNorMonState_Idle2Fight::OnStateEnd()
{
	__super::OnStateEnd();


}


void CNorMonState_Idle2Fight::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Idle2Fight::Call_AnimationEnd, this, placeholders::_1);
}


void CNorMonState_Idle2Fight::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;
	

	Get_OwnerCharacter().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_MonIdleType(NORMONSTERIDLETYPE::NORIDLE);
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



_bool CNorMonState_Idle2Fight::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	//10에서붙혀주면됨

	return false;
}

void CNorMonState_Idle2Fight::Free()
{

}