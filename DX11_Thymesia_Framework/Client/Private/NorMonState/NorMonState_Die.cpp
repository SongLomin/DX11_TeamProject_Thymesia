#include "stdafx.h"
#include "NorMonState/NorMonState_Die.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_Die);
CLONE_C(CNorMonState_Die, CComponent)

HRESULT CNorMonState_Die::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Die::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Die::Start()
{
	__super::Start();

	if (m_eNorMonType == NORMONSTERTYPE::AXEMAN)
	{
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Die01|BaseLayer|Armatu");
	}
	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Die::Call_AnimationEnd, this);

}

void CNorMonState_Die::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_Die::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();


}



void CNorMonState_Die::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: Stop -> StopStop" << endl;
#endif
#endif


}

void CNorMonState_Die::OnStateEnd()
{
	__super::OnStateEnd();


}


//void CNorMonState_Die::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Die::Call_AnimationEnd, this);
//}
//
//
//void CNorMonState_Die::Call_AnimationEnd()
//{
//	if (!Get_Enable())
//		return;
//
//	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
//}
//


_bool CNorMonState_Die::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

void CNorMonState_Die::Free()
{

}