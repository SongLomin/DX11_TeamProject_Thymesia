#include "stdafx.h"
#include "NorMonState/NorMonState_SitToIdle.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_SitToIdle);
CLONE_C(CNorMonState_SitToIdle, CComponent)

HRESULT CNorMonState_SitToIdle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_SitToIdle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_SitToIdle::Start()
{
	__super::Start();



	switch (m_eNorMonType)
	{
	case Client::NORMONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_SitToIdle|BaseLayer|Ar");
		break;
	case Client::NORMONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV1Villager_F.ao|SK_C_LV1Villager_F.ao|SK_C_LV1Villager_F.ao|SitToIdle|SK_C_LV1V");
		break;
	case Client::NORMONSTERTYPE::SKULL:
		break;
	case Client::NORMONSTERTYPE::GARDENER:
		break;
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_SitToIdle::Call_AnimationEnd, this);


}

void CNorMonState_SitToIdle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_SitToIdle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();


}



void CNorMonState_SitToIdle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif


}

void CNorMonState_SitToIdle::OnStateEnd()
{
	__super::OnStateEnd();

	

}


void CNorMonState_SitToIdle::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_AnimIndex(0);
	Get_OwnerCharacter().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_MonIdleType(NORMONSTERIDLETYPE::NORIDLE);
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}

void CNorMonState_SitToIdle::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_SitToIdle::Call_AnimationEnd, this);
}

void CNorMonState_SitToIdle::Free()
{

}

_bool CNorMonState_SitToIdle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}